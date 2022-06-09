#ifndef CXML_H
#define CXML_H
#include<Qstring>
#include<QMap>
class CXml
{
public:
    CXml();
    //map<string, string> *getBody_info();
    //map< string, map<string, string> > *getBody_info_arr();
    //void setFilePath(string filePath);




    QString getXML();

    void message2XML();

    void setMessageType(const QString &newMessageType);

    void setMsgName(const QString &newMsgName);

    void setClientId(const QString &newClientId);


    void setTrxId(const QString &newTrxId);

    void setResult(const QString &newResult);

    void setRtnCode(const QString &newRtnCode);

    void setRtnMsg(const QString &newRtnMsg);

    void setMsgLevel(const QString &newMsgLevel);

    void setBodyInfo(const QMap<QString, QString> &newBodyInfo);

    void setComCode(const QString &newIdCODE);

    void setIdCode(const QString &newIdCODE);

private :

    //<xml>
    QString m_fileHeader;

    //
    QString m_messageType;
    //Client端设置为request  Server端可设置为response,
    //后面可以优化它为 常量字符串 选择，不用用户去自定义
    // 下面的header与body会同步修改
    //<header>  标识请求/回复
    QString m_header;
    //<msg_name>  消息名
    QString m_msgName;
    //<client_id> 设备的唯一id
    QString m_clientId;
    //<trx_id> 消息的唯一id
    QString m_trxId;
    //<id_code> 设备的身份验证码
    QString m_idCode;
    //<com_code> 设备通信的通信码
    QString m_comCode;

    //<result> response消息携带用于标识结果状态 -1：请求失败  0：请求成功
    QString m_result;
    //<rtn_code> 返回消息标志码 --
    QString m_rtnCode;
    //<rtn_msg> 返回的消息的简要描述
    QString m_rtnMsg;
    //<msg_level> 消息的紧急等级 -- 0:普通状态消息  1：警告消息  2:紧急故障消息
    QString m_msgLevel;

    //形成的xml文档字符
    QString m_allInfo;
    //private byte[] all_info_byte;

    QString m_body;
    //body数据存储  目前只支持对String类型的数据进行存储
    QMap<QString, QString> m_bodyInfo;



};

#endif // CXML_H
