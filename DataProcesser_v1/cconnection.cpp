#include "cconnection.h"

CConnection::CConnection(QObject * parent):QObject(parent)
{
    m_pSocket = new QTcpSocket(this);

}

CConnection::~CConnection()
{
    delete m_pSocket;
}

//连接到 服务器端
void CConnection::connect2server(CServerInfo *serverinfo)
{
    //异步连接到server
    m_pSocket->connectToHost(serverinfo->getIpAddress(),serverinfo->serverPort());
    connect(m_pSocket,SIGNAL(connected()),this,SLOT(on_connected()));
}

//void CConnection::disconnect2server()
//{
//    m_pSocket->close();
//}

QTcpSocket *CConnection::pSocket() const
{
    return m_pSocket;
}

//socket连接成功后的事件处理函数
void CConnection::on_connected()
{
    //连接成功后--发送信号--更新ui
    emit connectSuccess(m_pSocket);

}


