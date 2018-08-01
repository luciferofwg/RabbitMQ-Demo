#ifndef RABBITMQ_CLIENT_H_
#define RABBITMQ_CLIENT_H_


#include <string>
#include <vector>
#include "amqp_tcp_socket.h"

using std::string;
using std::vector;


#define CHECK_ONLINE_TIME   5 //秒
typedef void(*CallBack)(const std::string& strMsg);


class CRabbitmqClient 
{
public:
    CRabbitmqClient();
    ~CRabbitmqClient();

    int InitMQ(
        const string &strHostname, 
        int iPort, 
        const string &strUser, 
        const string &strPasswd,
        const std::string &strExchange,
        const std::string &strExchangeType,
        const std::string &strRoutekey,
        const std::string &strQueuename
        );

    int ReleaseMQ();
    /**
    * @brief    发布消息
    * @param    消息实体
    * @return   等于0值代表成功发送消息实体，小于0代表发送错误
    */
    int Publish(const string &strMessage);
    /**
    * @brief    消费消息
    * @param    队列名称
    * @param    获取的消息实体
    * @param    需要取得的消息个数
    * @param    取得的消息是延迟，若为NULL，表示持续取，无延迟，阻塞状态
    * @return   等于0值代表成功，小于0代表错误，错误信息从ErrorReturn返回
    */
    int Consumer(const string &strQueueName, vector<string> &message_array, int GetNum = 1, struct timeval *timeout = NULL);
protected:
    int OpenSock();
    int Connect();
    int Disconnect();

    int CheckOnline();

    /**
    * @brief    打开通道
    * @param    
    * @param    
    * @return   
    */
    int inline OpenChannel();
    /**
    * @brief    关闭通道
    * @param    
    * @param    
    * @return   
    */
    int inline CloseChannel();
    /**
    * @brief    声明exchange
    * @param    strExchange
    * @param    strType
    * @return   等于0值代表成功创建exchange，小于0代表错误
    */
    int ExchangeDeclare(const string &strExchange, const string &strType);

    /**
    * @brief    声明消息队列
    * @param    消息队列实例
    * @return   等于0值代表成功创建queue，小于0代表错误
    */
    int QueueDeclare(const string &strQueueName);

    /**
    * @brief    将队列，交换机和绑定规则绑定起来形成一个路由表
    * @param    消息队列
    * @param    交换机名称
    * @param    路由名称  “msg.#” “msg.weather.**”
    * @return   等于0值代表成功绑定，小于0代表错误
    */
    int QueueBind(const string &strQueueName, const string &strExchange, const string &strBindKey);

    /**
    * @brief    将队列，交换机和绑定规则绑定解除
    * @param    消息队列
    * @param    交换机名称
    * @param    路由名称  “msg.#” “msg.weather.**”
    * @return   等于0值代表成功绑定，小于0代表错误
    */
    int QueueUnbind(const string &strQueueName, const string &strExchange, const string &strBindKey);

    /**
    * @brief    删除消息队列。
    * @param    消息队列名称
    * @param    消息队列是否在用，1 则论是否在用都删除
    * @return   等于0值代表成功删除queue，小于0代表错误
    */
    int QueueDelete(const string &strQueueName, int iIfUnused);
private:
    CRabbitmqClient(const CRabbitmqClient & rh);
    void operator=(const CRabbitmqClient & rh);

    int ErrorMsg(amqp_rpc_reply_t x, char const *context);
private:
    /*< 是否退出 >*/
    bool                        m_bExit;
    /*< 是否在线 >*/
    bool                        m_bConnected;
    /*< 通道号 >*/
    int                         m_iChannel;
    /*< 端口号 >*/
    int                         m_iPort;
    
    amqp_socket_t*              m_pSock;
    amqp_connection_state_t     m_pConn;

    /*< 主机ip >*/
    string                      m_strHostname;
    /*< 用户名 >*/
    string					    m_strUser;
    /*< 密码 >*/
    string					    m_strPasswd;
    /*< 交换机名称 >*/
    string                      m_strExchange;
    /*< 交换机类型 >*/
    string                      m_strExchangeType;
    /*< RoutKey >*/
    string                      m_strRoutekey;
    /*< 队列名称 >*/
    string                      m_strQueuename;
};

#endif
