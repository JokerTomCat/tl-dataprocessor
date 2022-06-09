#include "formlog.h"
#include "ui_formlog.h"
#include<QFileDialog>
#include<QDate>
#include<QMessageBox>

FormLog::FormLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLog)
{
    ui->setupUi(this);
}

FormLog::~FormLog()
{
    delete ui;
}


//选择Log文件夹
void FormLog::on_pushButton_clicked()
{
    //文件夹路径
    QString srcDirPath = QFileDialog::getExistingDirectory(
                this, "选择Log文件夹",
                "/");
    if (srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        QDate d = QDate::currentDate();
        QString part1 = QString("%1.%2.%3").arg(d.year()).arg(d.month(),2,10,QChar('0')).arg(d.day(),2,10,QChar('0'));
        QString fileName_event = "Event_"+ part1;
        QString fileName_status = "Status_"+part1;
        //定位目标文件夹下的文件
        QString file_status  = srcDirPath+"/"+fileName_status+".txt";
        QString file_event  = srcDirPath+"/"+fileName_event+".txt";
        file_status.replace("/","\\");
        file_event.replace("/","\\");

        QFile eventFile(file_event);
        QFile statusFile(file_status);
        //判断log文件是否存在...
        if(!eventFile.exists()){

            QMessageBox::warning(this,"文件未找到","[ "+file_event+" ]不存在，已自动创建空文件");
            eventFile.open( QIODevice::ReadWrite | QIODevice::Text );
            //注意关闭文件
            eventFile.close();

        }else if ( !statusFile.exists() ){

            QMessageBox::warning(this,"文件未找到","[ "+file_status+" ]不存在，已自动创建空文件");
            statusFile.open( QIODevice::ReadWrite | QIODevice::Text );
            //注意关闭文件
            statusFile.close();
        }

        //更新ui
        ui->lb_logpath ->setText("The directory of log file :\n "+srcDirPath);
        emit logPathUpdate(srcDirPath,file_event,file_status);
    }
}

//关闭窗口
void FormLog::on_pushButton_2_clicked()
{
    this->hide();
}

