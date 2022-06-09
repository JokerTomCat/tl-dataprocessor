#ifndef CCONNECTION_H
#define CCONNECTION_H
#include<QTcpSocket>
#include"cserverinfo.h"

class CConnection:public QObject
{
    Q_OBJECT
public:
    CConnection(QObject * parent = nullptr);
    ~CConnection();
    void connect2server(CServerInfo*);
    //void disconnect2server();

    QTcpSocket *pSocket() const;

private slots:
    void on_connected();
    //void on_disconnected();
    //void on_readyRead();
private:
    QTcpSocket *m_pSocket;

    //终止函数标志
    //bool m_bFlag;

signals:
    void connectSuccess(QTcpSocket*);

};

#endif // CCONNECTION_H
