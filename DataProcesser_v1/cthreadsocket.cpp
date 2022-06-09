#include "cthreadsocket.h"
#include"cutils.h"
#include<QdateTime>

#include<QThread>
#include<qDebug>

//------------------------------------------------------------------
CThreadSocket::CThreadSocket(QObject *parent) : QObject(parent)
{
    //是否可以运行(暂停状态)
    m_bRun = true;
    //是否有缓存数据
    m_bCacheEvent = false;
    m_bCacheStatus = false;
    //初始化socket
    m_pSocekt = new QTcpSocket(this);

}// CThreadSocket(QObject)


CThreadSocket::~CThreadSocket()
{
    delete m_pServerInfo;
    //m_pSocekt->close();
    delete m_pSocekt;

}
//------------------------------------------------------------------
CServerInfo *CThreadSocket::pServerInfo() const
{
    return m_pServerInfo;
} //pServerInfo() const
//------------------------------------------------------------------
void CThreadSocket::setPServerInfo(CServerInfo *newPServerInfo)
{
    m_pServerInfo = newPServerInfo;
} //setPServerInfo(CServerInfo *newPServerInfo)
//------------------------------------------------------------------


//线程的运行函数 -- 连接server
void CThreadSocket::on_connect2Server()
{

        qDebug()<<"---线程CThreadSocket::on_connect2Server()... start ";



        connect(m_pSocekt,SIGNAL(connected()),this,SLOT(on_connectSuccess()));
        connect(m_pSocekt,SIGNAL(disconnected()),this,SLOT(on_disconnected()));
        connect(m_pSocekt,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
        m_pSocekt->connectToHost(m_pServerInfo->getIpAddress(),m_pServerInfo->serverPort());
        //尝试连接3s后如果未连接成功，则返回连接失败
        bool connectResult = m_pSocekt->waitForConnected(3000);

        if(!connectResult){
            emit connectFailed();
            m_pSocekt->disconnectFromHost();
            m_pSocekt->waitForDisconnected();
        }


       qDebug()<<"---线程CThreadSocket::on_connect2Server()... end ";

}//on_connect2Server()
//------------------------------------------------------------------


//暂停线程的执行...
void CThreadSocket::on_stopThread()
{
    qDebug()<<"---线程CThreadSocket::on_stopThread()... start ";


    m_bRun = false;
    emit stopped();

    qDebug()<<"---线程CThreadSocket::on_stopThread()... end ";
}

//继续线程执行
void CThreadSocket::on_continueThread()
{
    qDebug()<<"---线程CThreadSocket::on_continueThread()... start ";

    if(m_bCacheStatus){
        //satus数据缓存
        CXml xml;
        QString xmlInfo;
        for (int i = 0; i < m_cacheStatus.size(); ++i) {
            QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());
            QString str = datetime->toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
            xml.setTrxId(str);
            xml.setClientId(m_clientID);
            xml.setMessageType("upload");
            xml.setMsgName("DeviceStatus_Upload");
            xml.setBodyInfo(statusLine2Map(m_cacheStatus[i]));
            //获取xml文本
            //消息结束符 END
            xmlInfo = xml.getXML() + "END\n";

            //发送
            m_pSocekt->write(xmlInfo.toUtf8());
            m_pSocekt->flush();
            xmlInfo.clear();
        }
        m_cacheStatus.clear();
        m_bCacheStatus = false;
    }

    if(m_bCacheEvent){ //有缓存数据 -- 先处理缓存数据
        CXml xml;
        QString xmlInfo;
        for (int i = 0; i < m_cacheEvent.size(); ++i) {
            QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());
            QString str = datetime->toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
            xml.setTrxId(str);
            xml.setClientId(m_clientID);
            xml.setMessageType("upload");
            xml.setMsgName("DeviceEvent_Upload");
            xml.setBodyInfo(eventLine2Map(m_cacheEvent[i]));
            //获取xml文本
            //消息结束符 END
            xmlInfo = xml.getXML() + "END\n";

            //发送
            m_pSocekt->write(xmlInfo.toUtf8());
            m_pSocekt->flush();
            xmlInfo.clear();
        }
        m_cacheEvent.clear();
        m_bCacheEvent = false;    
    }

    m_bRun = true;
    emit continued();


    qDebug()<<"---线程CThreadSocket::on_continueThread()... end ";
}


/**
 * 发送事件内容msg到server
 * @brief CThreadSocket::sendEventXml
 * @param events
 */
void CThreadSocket::sendEventXml(QStringList events )
{
    if(m_bRun){ // 正常运行
        //有缓存内容先传缓存内容


        //再传收到的内容
        CXml xml;
        QString xmlInfo;

        for (int i = 0; i < events.size(); ++i) {
            if(events[i].split(",").size() != 5)
                continue;

            QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());
            QString str = datetime->toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
            xml.setTrxId(str);
            xml.setClientId(m_clientID);
            xml.setComCode(m_comCode);
            xml.setMessageType("upload");
            xml.setMsgName("DeviceEvents_Upload");
            xml.setBodyInfo(eventLine2Map(events[i]));

            xmlInfo = xml.getXML() + "END\n";
            //qDebug()<<xmlInfo;
            m_pSocekt->write(xmlInfo.toUtf8());
            m_pSocekt->flush();
            xmlInfo.clear();
        }



    }else{
        // m_bRun = false
        //使用缓存机制，将数据存储下来！！！很关键
        m_cacheEvent.append(events);
        m_bCacheEvent = true;
    }



}

void CThreadSocket::sendStatusXml(QStringList status)
{
    if(m_bRun){ // 正常运行
        CXml xml;
        QString xmlInfo;

        for (int i = 0; i < status.size(); ++i) {
            if(status[i].split(",").size() != 7)
                continue;

            QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());
            QString str = datetime->toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
            xml.setTrxId(str);
            xml.setClientId(m_clientID);
            xml.setMessageType("upload");
            xml.setMsgName("DeviceStatus_Upload");
            xml.setBodyInfo(statusLine2Map(status[i]));

            xmlInfo = xml.getXML() + "END\n";
            m_pSocekt->write(xmlInfo.toUtf8());
            m_pSocekt->flush();
            xmlInfo.clear();
        }



    }else{
        // m_bRun = false
        //使用缓存机制，将数据存储下来！！！很关键
        m_cacheStatus.append(status);
        m_bCacheStatus = true;
    }


}

void CThreadSocket::on_disconnected()
{
    m_pSocekt->close();

}

void CThreadSocket::on_readyRead()
{
    if(m_bRun){
        QString txt = m_pSocekt->readAll();


        qDebug()<<"[ CThreadSocket::on_readyRead() ]\n"+txt;


        QString msgName = CUtils::getAttributeByName(txt,"msg_name");
        if(msgName == "CONNECT_RESPONSE"){
            //是 连接回应报文
            if(CUtils::getAttributeByName(txt,"identity") == "0"){
                //认证成功
                m_comCode = CUtils::getAttributeByName(txt,"com_code");
                emit connectSuccess();

            }else{
                //认证失败  断开socket
                m_pSocekt->disconnectFromHost();
                m_pSocekt->waitForDisconnected();
                emit connectFailed();
            }
        }else{

            //其他类型的 回应报文
            qDebug()<<txt;
        }
    }

}

//响应主线程关闭该子线程的事件
void CThreadSocket::on_closeSocketThread()
{
    //断开socket连接
    m_pSocekt->disconnectFromHost();
    if(m_pSocekt->waitForDisconnected(3000))
        emit disconnectSuccess(m_cacheEvent.size(),m_cacheStatus.size());
    else
        emit disconnectFailed();
}

void CThreadSocket::setClientID(const QString &newClientID)
{
    m_clientID = newClientID;
}

//将event log逐行处理成xml的body部分
QMap<QString, QString> CThreadSocket::statusLine2Map(QString line)
{
    QMap<QString, QString> res;
    QStringList parts = line.split(",");
    res.insert("TimeDate",parts[0]);
    res.insert("MachineNo",parts[1]);
    res.insert("ProcessFile",parts[2]);
    res.insert("StateLight",parts[3]);
    res.insert("StateAxis",parts[4]);
    res.insert("TotalTime",parts[5]);
    res.insert("CurrentTime",parts[6]);
    return res;

}

QMap<QString, QString> CThreadSocket::eventLine2Map(QString line)
{
    QMap<QString, QString> res;
    QStringList parts = line.split(",");
    res.insert("TimeDate",parts[0]);
    res.insert("MachineNo",parts[1]);
    res.insert("ProcessFile",parts[2]);
    res.insert("EventCode",parts[3]);
    res.insert("EventInfo",parts[4]);
    return res;

}

//socket连接成功后的事件处理--给ui发信号，主线程创建文件处理线程
void CThreadSocket::on_connectSuccess()
{
    //发送身份验证msg
    CXml xml;
    QString xmlInfo;
    QDateTime *datetime=new QDateTime(QDateTime::currentDateTime());
    QString dateStr = datetime->toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    xml.setMessageType("request");
    xml.setMsgName("CONNECT_REQUEST");
    xml.setTrxId(dateStr);
    xml.setClientId(m_clientID);
    //要验证identity_code是否与服务器提供的一致
    xml.setIdCode(m_pServerInfo->serverSecretKey());


    xmlInfo = xml.getXML() + "END\n";

    m_pSocekt->write(xmlInfo.toUtf8());
    m_pSocekt->flush();

    //qDebug()<<"[ CThreadSocket::on_connectSuccess() ]\n"+xmlInfo;
    //emit connectSuccess();


} // on_connectSuccess()
//------------------------------------------------------------------
