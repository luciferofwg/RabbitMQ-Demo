#ifndef RABBITMQ_CLIENT_H_
#define RABBITMQ_CLIENT_H_


#include <string>
#include <vector>
#include "amqp_tcp_socket.h"

using std::string;
using std::vector;


#define CHECK_ONLINE_TIME   5 //��
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
    * @brief    ������Ϣ
    * @param    ��Ϣʵ��
    * @return   ����0ֵ����ɹ�������Ϣʵ�壬С��0�����ʹ���
    */
    int Publish(const string &strMessage);
    /**
    * @brief    ������Ϣ
    * @param    ��������
    * @param    ��ȡ����Ϣʵ��
    * @param    ��Ҫȡ�õ���Ϣ����
    * @param    ȡ�õ���Ϣ���ӳ٣���ΪNULL����ʾ����ȡ�����ӳ٣�����״̬
    * @return   ����0ֵ����ɹ���С��0������󣬴�����Ϣ��ErrorReturn����
    */
    int Consumer(const string &strQueueName, vector<string> &message_array, int GetNum = 1, struct timeval *timeout = NULL);
protected:
    int OpenSock();
    int Connect();
    int Disconnect();

    int CheckOnline();

    /**
    * @brief    ��ͨ��
    * @param    
    * @param    
    * @return   
    */
    int inline OpenChannel();
    /**
    * @brief    �ر�ͨ��
    * @param    
    * @param    
    * @return   
    */
    int inline CloseChannel();
    /**
    * @brief    ����exchange
    * @param    strExchange
    * @param    strType
    * @return   ����0ֵ����ɹ�����exchange��С��0�������
    */
    int ExchangeDeclare(const string &strExchange, const string &strType);

    /**
    * @brief    ������Ϣ����
    * @param    ��Ϣ����ʵ��
    * @return   ����0ֵ����ɹ�����queue��С��0�������
    */
    int QueueDeclare(const string &strQueueName);

    /**
    * @brief    �����У��������Ͱ󶨹���������γ�һ��·�ɱ�
    * @param    ��Ϣ����
    * @param    ����������
    * @param    ·������  ��msg.#�� ��msg.weather.**��
    * @return   ����0ֵ����ɹ��󶨣�С��0�������
    */
    int QueueBind(const string &strQueueName, const string &strExchange, const string &strBindKey);

    /**
    * @brief    �����У��������Ͱ󶨹���󶨽��
    * @param    ��Ϣ����
    * @param    ����������
    * @param    ·������  ��msg.#�� ��msg.weather.**��
    * @return   ����0ֵ����ɹ��󶨣�С��0�������
    */
    int QueueUnbind(const string &strQueueName, const string &strExchange, const string &strBindKey);

    /**
    * @brief    ɾ����Ϣ���С�
    * @param    ��Ϣ��������
    * @param    ��Ϣ�����Ƿ����ã�1 �����Ƿ����ö�ɾ��
    * @return   ����0ֵ����ɹ�ɾ��queue��С��0�������
    */
    int QueueDelete(const string &strQueueName, int iIfUnused);
private:
    CRabbitmqClient(const CRabbitmqClient & rh);
    void operator=(const CRabbitmqClient & rh);

    int ErrorMsg(amqp_rpc_reply_t x, char const *context);
private:
    /*< �Ƿ��˳� >*/
    bool                        m_bExit;
    /*< �Ƿ����� >*/
    bool                        m_bConnected;
    /*< ͨ���� >*/
    int                         m_iChannel;
    /*< �˿ں� >*/
    int                         m_iPort;
    
    amqp_socket_t*              m_pSock;
    amqp_connection_state_t     m_pConn;

    /*< ����ip >*/
    string                      m_strHostname;
    /*< �û��� >*/
    string					    m_strUser;
    /*< ���� >*/
    string					    m_strPasswd;
    /*< ���������� >*/
    string                      m_strExchange;
    /*< ���������� >*/
    string                      m_strExchangeType;
    /*< RoutKey >*/
    string                      m_strRoutekey;
    /*< �������� >*/
    string                      m_strQueuename;
};

#endif
