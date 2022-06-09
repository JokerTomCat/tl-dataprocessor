#include "cutils.h"

CUtils::CUtils()
{

}

QString CUtils::getAttributeByName(QString xml, QString attr)
{
    //处理XML格式内容
    int startIndex = xml.indexOf("<"+attr+">") + attr.length() + 2;
    if(startIndex == -1)
        return "";
    int strLen = xml.indexOf("</"+attr+">") - startIndex;

    QString res = xml.mid(startIndex, strLen);
    return res;
}

