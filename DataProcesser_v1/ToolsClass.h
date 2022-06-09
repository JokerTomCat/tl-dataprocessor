#ifndef TOOLSCLASS_H
#define TOOLSCLASS_H
#include<QString>
//自定义ip地址类型
typedef struct IpAddress
{
    int ip_first;
    int ip_second;
    int ip_third;
    int ip_fourth;


    bool validation(){
        if(ip_first<0 || 255<ip_first)
            return false;
        if(ip_second<0 || 255<ip_second)
            return false;
        if(ip_third<0 || 255<ip_third)
            return false;
        if(ip_fourth<0 || 255<ip_fourth)
            return false;
        return true;
    };

}IpAddress;

//记录上次文件读取位置的结构
struct SfileRecord
{
    SfileRecord() {}
    QString line1_filename;
    int line1_rowsNo;
    QString line2_filename;
    int line2_rowsNo;

    QString getLine1(){
        return line1_filename+"|"+QString::number(line1_rowsNo, 10);
    }

    QString getLine2(){
        return line2_filename+"|"+QString::number(line2_rowsNo, 10);
    }


};










#endif // TOOLSCLASS_H
