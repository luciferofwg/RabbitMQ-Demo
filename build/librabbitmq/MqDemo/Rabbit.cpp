#include "Rabbit.h"
#include <windows.h>

#include <iostream>
#include <thread>
#include <chrono>

#include "TimeStamp.hpp"

CRabbitmqClient::CRabbitmqClient():
    m_bExit(false)
    , m_bConnected(false)
    , m_strHostname("")
    , m_iPort(0)
    , m_strUser("")
    , m_strPasswd("")
    , m_iChannel(1) //默认用1号通道，通道无所谓 
    , m_pSock(NULL)
    , m_pConn(NULL) 
{

}

CRabbitmqClient::~CRabbitmqClient() 
{
    if (NULL != m_pConn) 
    {
        Disconnect();
        m_pConn = NULL;
    }
}

int CRabbitmqClient::InitMQ(
    const string &strHostname, 
    int iPort, 
    const string &strUser, 
    const string &strPasswd, 
    const std::string &strExchange,
    const std::string &strExchangeType,
    const std::string &strRoutekey,
    const std::string &strQueuename
    )
{
    int nRet = 0;
    if (strHostname.empty() || 
        strUser.empty() || 
        strPasswd.empty() || 
        strExchange.empty() ||
        strRoutekey.empty() ||
        strQueuename.empty() ||
        iPort < 0)
    {
        fprintf(stderr, "intilize RabbitMQ paramer failed!\n");
        return -1;
    }

    m_strHostname = strHostname;
    m_iPort = iPort;
    m_strUser = strUser;
    m_strPasswd = strPasswd;

    m_strExchange = strExchange;
    m_strExchangeType = strExchangeType;
    m_strRoutekey = strRoutekey;
    m_strQueuename = strQueuename;

    
    if ( 0 == (nRet = Connect()))
    {
        m_bConnected = true;
        //启动在线检测
        std::thread tOnline(&CRabbitmqClient::CheckOnline, this);
        tOnline.detach();
    }
    return nRet;
}

int CRabbitmqClient::ReleaseMQ()
{
    return Disconnect();
}

int CRabbitmqClient::OpenSock() 
{
    m_pConn = amqp_new_connection();
    if (NULL == m_pConn) 
    {
        fprintf(stderr, "amqp new connection failed\n");
        return -1;
    }

    m_pSock = amqp_tcp_socket_new(m_pConn);
    if (NULL == m_pSock) 
    {
        fprintf(stderr, "amqp tcp new socket failed\n");
        return -2;
    }

    int status = amqp_socket_open(m_pSock, m_strHostname.c_str(), m_iPort);
    if (status<0) 
    {
        fprintf(stderr, "amqp socket open failed\n");
        return -3;
    }

    if (0 != ErrorMsg(amqp_login(m_pConn, "/", 1, 131072, 1, AMQP_SASL_METHOD_PLAIN, m_strUser.c_str(), m_strPasswd.c_str()), "Logging in")) 
    {
        fprintf(stderr, "amqp socket login failed\n");
        return -4;
    }

    return 0;
}
int CRabbitmqClient::Connect()
{
    int nRet = 0;
    //连接MQ服务器
    nRet &= OpenSock();
    //打开通道
    OpenChannel();
    //申明交换机
    nRet &= ExchangeDeclare(m_strExchange, m_strExchangeType);
    //申明队列
    nRet &= QueueDeclare(m_strQueuename);
    //绑定队列
    nRet &= QueueBind(m_strQueuename, m_strExchange, m_strRoutekey);
    
    return nRet;
}

int CRabbitmqClient::Disconnect() 
{
    if (NULL != m_pConn) 
    {
        CloseChannel();
        if (0 != ErrorMsg(amqp_connection_close(m_pConn, AMQP_REPLY_SUCCESS), "Closing connection"))
            return -1;

        if (amqp_destroy_connection(m_pConn) < 0)
            return -2;

        m_pConn = NULL;
    }

    return 0;
}


int CRabbitmqClient::OpenChannel()
{
    amqp_channel_open(m_pConn, m_iChannel);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel"))
    {
        return -1;
    }
    return 0;
}

int CRabbitmqClient::CloseChannel()
{
    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "close channel"))
    {
        return -1;
    }
    return 0;
}

int CRabbitmqClient::ExchangeDeclare(const string &strExchange, const string &strType) 
{
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _type = amqp_cstring_bytes(strType.c_str());
    int _passive = 0;
    int _durable = true;      //是否持久化
    amqp_exchange_declare(m_pConn, m_iChannel, _exchange, _type, _passive, _durable, 0, 0, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "exchange_declare")) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }
    return 0;
}

int CRabbitmqClient::QueueDeclare(const string &strQueueName) 
{
    if (NULL == m_pConn) 
    {
        fprintf(stderr, "QueueDeclare m_pConn is null\n");
        return -1;
    }

    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    int32_t _passive = 0;
    int32_t _durable = true;    //是否持久化
    int32_t _exclusive = 0;
    int32_t _auto_delete = 0;
    amqp_queue_declare(m_pConn, m_iChannel, _queue, _passive, _durable, _exclusive, _auto_delete, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_declare"))
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    return 0;
}

int CRabbitmqClient::QueueBind(const string &strQueueName, const string &strExchange, const string &strBindKey) 
{
    if (NULL == m_pConn) 
    {
        fprintf(stderr, "QueueBind m_pConn is null\n");
        return -1;
    }

    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _routkey = amqp_cstring_bytes(strBindKey.c_str());

    amqp_queue_bind(m_pConn, m_iChannel, _queue, _exchange, _routkey, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_bind")) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    return 0;
}

int CRabbitmqClient::QueueUnbind(const string &strQueueName, const string &strExchange, const string &strBindKey)
{
    if (NULL == m_pConn) 
    {
        fprintf(stderr, "QueueUnbind m_pConn is null\n");
        return -1;
    }

    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _routkey = amqp_cstring_bytes(strBindKey.c_str());
    amqp_queue_unbind(m_pConn, m_iChannel, _queue, _exchange, _routkey, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_unbind")) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }
    return 0;
}

int CRabbitmqClient::QueueDelete(const string &strQueueName, int iIfUnused) 
{
    if (NULL == m_pConn) 
    {
        fprintf(stderr, "QueueDelete m_pConn is null\n");
        return -1;
    }

    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel"))
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -2;
    }

    amqp_queue_delete(m_pConn, m_iChannel, amqp_cstring_bytes(strQueueName.c_str()), iIfUnused, 0);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "delete queue")) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -3;
    }

    return 0;
}

int CRabbitmqClient::Publish(const string &strMessage) 
{
    if (false == m_bConnected)
    {
        fprintf(stderr, "publish connection is off, publish failed\n");
        return -4;
    }
    if (NULL == m_pConn) 
    {
        fprintf(stderr, "publish m_pConn is null, publish failed\n");
        m_bConnected = false;
        return -1;
    }

    amqp_bytes_t message_bytes;
    message_bytes.len = strMessage.length();
    message_bytes.bytes = (void *)(strMessage.c_str());

    amqp_bytes_t exchange = amqp_cstring_bytes(m_strExchange.c_str());
    amqp_bytes_t routekey = amqp_cstring_bytes(m_strRoutekey.c_str());
    if (0 != amqp_basic_publish(m_pConn, m_iChannel, exchange, routekey, 0, 0, NULL, message_bytes))
    {
        if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "amqp_basic_publish")) 
        {
            amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
            m_bConnected = false;
            return -3;
        }
    }

    //重置连接状态
    m_bConnected = true;
    return 0;
}

int CRabbitmqClient::Consumer(const string &strQueueName, vector<string> &message_array, int GetNum, struct timeval *timeout) 
{
    if (false == m_bConnected)
    {
        return -4;
    }
    if (NULL == m_pConn)
    {
        fprintf(stderr, "Consumer m_pConn is null, Consumer failed\n");
        m_bConnected = false;
        return -1;
    }

    amqp_basic_qos(m_pConn, m_iChannel, 0, GetNum, 0);
    int ack = true; // no_ack    是否需要确认消息后再从队列中删除消息
    amqp_bytes_t queuename = amqp_cstring_bytes(strQueueName.c_str());
    amqp_basic_consume(m_pConn, m_iChannel, queuename, amqp_empty_bytes, 0, ack, 0, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "Consuming")) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        m_bConnected = false;
        return -3;
    }

    int hasget = 0;
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    amqp_maybe_release_buffers(m_pConn);
    while (GetNum > 0) 
    {
        res = amqp_consume_message(m_pConn, &envelope, timeout, 0);
        if (AMQP_RESPONSE_NORMAL != res.reply_type) 
        {
            if (AMQP_RESPONSE_LIBRARY_EXCEPTION == res.reply_type && AMQP_STATUS_HEARTBEAT_TIMEOUT == res.library_error)
            {
                printf("heartbeat timeout \n");
            }

            fprintf(stderr, "Consumer amqp_channel_close failed\n");
            amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);

            if (0 == hasget)
                return -res.reply_type;
            else
                return 0;
        }

        string str((char *)envelope.message.body.bytes, (char *)envelope.message.body.bytes + envelope.message.body.len);
        message_array.push_back(str);

        GetNum--;
        hasget++;
        Sleep(1);
    }
    amqp_destroy_envelope(&envelope);
    m_bConnected = true;
    return 0;
}

int CRabbitmqClient::ErrorMsg(amqp_rpc_reply_t x, char const *context)
{
    switch (x.reply_type) 
    {
    case AMQP_RESPONSE_NORMAL:
        return 0;

    case AMQP_RESPONSE_NONE:
        fprintf(stderr, "%s: missing RPC reply type!\n", context);
        break;

    case AMQP_RESPONSE_LIBRARY_EXCEPTION:
        fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x.library_error));
        break;

    case AMQP_RESPONSE_SERVER_EXCEPTION:
        switch (x.reply.id) 
        {
        case AMQP_CONNECTION_CLOSE_METHOD: 
        {
            amqp_connection_close_t *m = (amqp_connection_close_t *)x.reply.decoded;
            fprintf(stderr, "%s: server connection error %uh, message: %.*s\n",
                context, m->reply_code, (int)m->reply_text.len,
                (char *)m->reply_text.bytes);
            break;
        }
        case AMQP_CHANNEL_CLOSE_METHOD:
        {
            amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
            fprintf(stderr, "%s: server channel error %uh, message: %.*s\n",
                context, m->reply_code, (int)m->reply_text.len,
                (char *)m->reply_text.bytes);
            break;
        }
        default:
            fprintf(stderr, "%s: unknown server error, method id 0x%08X\n",
                context, x.reply.id);
            break;
        }
        break;
    default:
        printf("++++++++++++++++++\nerror code:%d\n+++++++++++++++++++\n", x.reply_type);
        break;
    }

    return -1;
}

int CRabbitmqClient::CheckOnline()
{
    //定义一个时间戳对象
    Timestamp timeStamp;
    while (!m_bExit)
    {
        //获取当前经历的时间（秒），若达到判断时间且断开连接时，进行断线重连
        if (CHECK_ONLINE_TIME  <= timeStamp.getElapsedSecond() && false == m_bConnected)
        {
            timeStamp.update();
            std::cout << "begin try connect to server again" << std::endl;
            int nCount = 0;
            //关闭连接
            Disconnect();

            //启动重连成功
            while (true)
            {
                if (0 == Connect())
                {
                    std::cout << "reConnected secessed." << std::endl;
                    m_bConnected = true;
                    break;
                }
                else
                {
                    m_bConnected = false;
                    nCount++;
                    std::cout << "reconnected <"<< nCount <<"> times" << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));//sleep1秒
    }
    return  0;
}