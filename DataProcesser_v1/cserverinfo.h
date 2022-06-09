#ifndef CSERVERINFO_H
#define CSERVERINFO_H

#include<QString>
#include"ToolsClass.h"


class CServerInfo
{
public:
    CServerInfo();

private:
    //ip Address struct
    IpAddress m_serverIP;
    //ip Port
    int m_serverPort;
    //bool m_serverState;
    QString m_serverSecretKey;

public :

    const IpAddress &serverIP() const;
    void setServerIP(const IpAddress &newServerIP);
    int serverPort() const;
    void setServerPort(int newServerPort);
    //bool serverState() const;
    //void setServerState(bool newServerState);
    const QString &serverSecretKey() const;
    void setServerSecretKey(const QString &newServerSecretKey);
    //获取全路径
    QString getIpAddress();
    bool isValidation();
};







#endif // CSERVERINFO_H
