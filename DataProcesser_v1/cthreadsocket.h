#ifndef CTHREADSOCKET_H
#define CTHREADSOCKET_H

#include <QObject>
#include<cserverinfo.h>
#include<QTcpSocket>
//#include<QMutex>
#include"cxml.h"

class CThreadSocket:public QObject
{
    Q_OBJECT
public:
    CThreadSocket(QObject *parent = 0);
    ~CThreadSocket();
    CServerInfo *pServerInfo() const;
    void setPServerInfo(CServerInfo *newPServerInfo);

    void setClientID(const QString &newClientID);

    QMap<QString, QString> eventLine2Map(QString);
    QMap<QString, QString> statusLine2Map(QString);
private slots:
    void on_connectSuccess();
    void on_connect2Server();
    void on_stopThread();
    void on_continueThread();
    void sendEventXml(QStringList);
    void sendStatusXml(QStringList);
    void on_disconnected();
    void on_readyRead();
    void on_closeSocketThread();
signals:
    void connectSuccess();
    void disconnectSuccess(int,int);
    void disconnectFailed();
    void stopped();
    void continued();
    void connectFailed();
   // void startThreadOfFileProcess();
private:
    CServerInfo *m_pServerInfo;
    QTcpSocket *m_pSocekt;
    //设备端的id
    QString m_clientID;
    //运行状态
    bool m_bRun;
    //Server认证后的通信码
    QString m_comCode;

    //缓存数据 -- 注意内容重置
    QStringList m_cacheEvent;
    QStringList m_cacheStatus;
    //是否有缓存
    bool m_bCacheEvent;
    bool m_bCacheStatus;


};

#endif // CTHREADSOCKET_H
