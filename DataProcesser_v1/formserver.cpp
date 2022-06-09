#include "formserver.h"
#include "ui_formserver.h"
#include<QMessageBox>
#include<QRegExpValidator>
FormServer::FormServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormServer)
{
    ui->setupUi(this);

    //设置port只能输入数字
    QRegExp regx("[0-9]+$");
    QValidator* validator = new QRegExpValidator(regx, ui->le_internetport);
    ui->le_internetport->setValidator(validator);


}

FormServer::~FormServer()
{
    delete ui;
}

//从ui中获取数据初始化ip对象
bool FormServer::initAddress(QString ip_ui, IpAddress *ip)
{
    QStringList ipAddr = ip_ui.split(".");
    //长度限制
    if(ipAddr.size()!=4)
        return false;

    //数值限制
    for (int i = 0; i < ipAddr.size(); ++i) {
        int n ;
        n = ipAddr[i].toInt();
        if(n<0||255<n)
            return false;

        switch (i) {
        case 0:
            ip->ip_first = n;
            break;
        case 1:
            ip->ip_second = n;
            break;
        case 2:
            ip->ip_third = n;
            break;
        case 3:
            ip->ip_fourth = n;
            break;
        default:
            break;
        }
    }

    return ip->validation();

}


//选定服务器通信
void FormServer::on_btn_internetselect_clicked()
{
    if(ui->btn_internetselect->text() == "确定"){

        //验证格式
        QString ip = ui->le_internetip->text();
        int port = ui->le_internetport->text().toInt();
        QString idCode = ui->le_internetcode->text();
        IpAddress address;
        if(initAddress(ip,&address) && port>0&&port<65535)
            //传送信号
            emit serverInfoUpdate(address,port,idCode);
        else{

            //提示信息
            QMessageBox::critical(this,"错误信息","输入IP地址不正确");
            return;
        }


        //update ui
        ui->le_internetcode->setEnabled(false);
        ui->le_internetip->setEnabled(false);
        ui->le_internetport->setEnabled(false);
        ui->btn_internetselect->setText("更改");







    }else if(ui->btn_internetselect->text() == "更改"){
        ui->le_internetcode->setEnabled(true);
        ui->le_internetip->setEnabled(true);
        ui->le_internetport->setEnabled(true);
        ui->btn_internetselect->setText("确定");
    }


}



void FormServer::on_btn_internetselect_2_clicked()
{
    this->hide();
}

