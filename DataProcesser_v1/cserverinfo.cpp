#include "cserverinfo.h"

CServerInfo::CServerInfo()
{

}

const IpAddress &CServerInfo::serverIP() const
{
    return m_serverIP;
}

void CServerInfo::setServerIP(const IpAddress &newServerIP)
{
    m_serverIP = newServerIP;
}

int CServerInfo::serverPort() const
{
    return m_serverPort;
}

void CServerInfo::setServerPort(int newServerPort)
{
    m_serverPort = newServerPort;
}

//bool CServerInfo::serverState() const
//{
//    return m_serverState;
//}

//void CServerInfo::setServerState(bool newServerState)
//{
//    m_serverState = newServerState;
//}

const QString &CServerInfo::serverSecretKey() const
{
    return m_serverSecretKey;
}

void CServerInfo::setServerSecretKey(const QString &newServerSecretKey)
{
    m_serverSecretKey = newServerSecretKey;
}

QString CServerInfo::getIpAddress()
{
    QString ip = QString("%1.%2.%3.%4")
            .arg(m_serverIP.ip_first)
            .arg(m_serverIP.ip_second)
            .arg(m_serverIP.ip_third)
            .arg(m_serverIP.ip_fourth);

    return ip;
}

bool CServerInfo::isValidation()
{
    if(m_serverIP.ip_first==0 && m_serverIP.ip_second == 0 && m_serverIP.ip_third == 0&& m_serverIP.ip_fourth== 0)
        return false;
    return true;

}
