#include "cxml.h"

CXml::CXml()
{
    m_fileHeader = "<?xml version= \"1.0\" encoding=\"UTF-8\" ?>" ;
}

void CXml::setMessageType(const QString &newMessageType)
{
    m_messageType = newMessageType;
    m_header = m_messageType + "_header";
    m_body = m_messageType + "_body";
}

void CXml::setMsgName(const QString &newMsgName)
{
    m_msgName = newMsgName;
}

void CXml::setClientId(const QString &newClientId)
{
    m_clientId = newClientId;
}

void CXml::setTrxId(const QString &newTrxId)
{
    m_trxId = newTrxId;
}

void CXml::setResult(const QString &newResult)
{
    m_result = newResult;
}

void CXml::setRtnCode(const QString &newRtnCode)
{
    m_rtnCode = newRtnCode;
}

void CXml::setRtnMsg(const QString &newRtnMsg)
{
    m_rtnMsg = newRtnMsg;
}

void CXml::setMsgLevel(const QString &newMsgLevel)
{
    m_msgLevel = newMsgLevel;
}

void CXml::setBodyInfo(const QMap<QString, QString> &newBodyInfo)
{
    m_bodyInfo = newBodyInfo;
}

void CXml::setComCode(const QString &newIdCODE)
{
    m_comCode = newIdCODE;
}

void CXml::setIdCode(const QString &newIdCODE)
{
    m_idCode = newIdCODE;
}



QString CXml::getXML()
{
    message2XML();
    QString xml = m_allInfo.trimmed();
    return xml;
}

void CXml::message2XML()
{
    //将上述信息组成Xml文件 存储在filepath中
    m_allInfo = m_fileHeader + "\n";

    //<request>  / <response>
    m_allInfo += "<" + m_messageType + ">\n" + "\t" + "<" + m_header + ">\n";
    //<header>
    m_allInfo += "\t\t<msg_name>" + m_msgName + "</msg_name>\n";
    m_allInfo += "\t\t<client_id>" + m_clientId + "</client_id>\n";
    m_allInfo += "\t\t<trx_id>" + m_trxId + "</trx_id>\n";

    if(m_msgName == "CONNECT_REQUEST"){
        m_allInfo += "\t\t<id_code>" + m_idCode + "</id_code>\n";
    }else{
       m_allInfo += "\t\t<com_code>" + m_comCode + "</com_code>\n";
    }



    //回应报文 对应的xml标签
    if(m_messageType == "response"){
        m_allInfo += "\t\t<result>" + m_result + "</result>\n";
        m_allInfo += "\t\t<rtn_code>" + m_rtnCode + "</rtn_code>\n";
        m_allInfo += "\t\t<rtn_msg>" + m_rtnMsg + "</rtn_msg>\n";
    }


    //</header>
    m_allInfo += "\t</" + m_header + ">\n ";

    //<body>
    m_allInfo += "\t<" + m_body + ">\n";
    if (m_bodyInfo.size() > 0) {
        QMap<QString, QString>::const_iterator itr;
        for (itr = m_bodyInfo.constBegin(); itr != m_bodyInfo.constEnd(); ++itr) {
            QString s = itr.key();
            m_allInfo += "\t\t<" + s + ">" + itr.value() + "</" + s + ">\n";
        }
        //</body>
        m_allInfo += "\t</" + m_body + ">\n ";
    }
    //</request> </response>
    m_allInfo += "</" +m_messageType + ">";
}

