#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QApplication>
#include<qDebug>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btn_stop->setEnabled(false);

    //
    m_bIsRuning = false;
    //添加状态栏
    lb_state = new QLabel(this);
    lb_state->setText("当前客户端状态: 待机中...");
    lb_state->setStyleSheet("background-color: rgb(255, 255, 127)");
    ui->statusbar->addWidget(lb_state);

    //-------------------------初始化子窗口----------------------
    Formlog = new FormLog;  //set log
    Formserver = new FormServer;   // set ServerInfo
    Formset = new FormSet;      //set

    //-----------------------初始化类对象----------------------------
    //server的info--ip and port...
    m_pServerInfo = new CServerInfo ;
    //m_bContinue = false;

    //---------------------------------绑定信号-------------------------
    //【signals & slots 】 log文件路径设置
    connect(Formlog,SIGNAL(logPathUpdate(QString,QString,QString)),this,SLOT(on_logpath_update(QString,QString,QString)));
    //【signals & slots 】 serverInfo更新
    connect(Formserver,SIGNAL(serverInfoUpdate(IpAddress,int,QString)),this,SLOT(on_serverInfo_Update(IpAddress,int,QString)));


    //---------------------------socket连接通信线程---------------------

    //维持socket的线程（是个普通的类，使用方法--将使用moveToThread将该类作为线程类）
    m_pMyThreadSocket = new CThreadSocket();
    m_pThreadSocket = new QThread();

    //传入server基本数据
    m_pMyThreadSocket->moveToThread(m_pThreadSocket);
    //当线程结束时，释放子线程资源
    connect(m_pThreadSocket, &QThread::finished, m_pMyThreadSocket, &QObject::deleteLater);


    //点击按钮开始连接线程--运行子线程中的线程函数on_doSomething
    connect(this,SIGNAL(startSocketThread()),m_pMyThreadSocket,SLOT(on_connect2Server()));

    //连接成功时，主线程去做on_connectedSuccess
    connect(m_pMyThreadSocket,SIGNAL(connectSuccess()),this,SLOT(on_connectedSuccess()));
    //连接失败
    connect(m_pMyThreadSocket,SIGNAL(connectFailed()),this,SLOT(on_connectFailed()));

    //点击按钮暂停线程 --  运行子线程中的线程函数
    //connect(this,SIGNAL(stopThread()),m_pThreadSocket,SLOT(terminate()));
    connect(this,SIGNAL(stopSocketThread()),m_pMyThreadSocket,SLOT(on_stopThread()));
    //重启子线程
    connect(this,SIGNAL(continueSocketThread()),m_pMyThreadSocket,SLOT(on_continueThread()));
    //重启子线程反馈信号
    connect(m_pMyThreadSocket,SIGNAL(continued()),this,SLOT(on_threadContinued()));
    //子线程暂停后，主线程更新ui
    connect(m_pMyThreadSocket,SIGNAL(stopped()),this,SLOT(on_threadStopped()));
    //主线程关闭子线程
    connect(this,SIGNAL(closeSocketThread()),m_pMyThreadSocket,SLOT(on_closeSocketThread()));
    //子线程关闭socket连接成功
    connect(m_pMyThreadSocket,SIGNAL(disconnectSuccess(int,int)),this,SLOT(on_disconnectSuccess(int,int)));


    //-----------------------文件处理线程------------------------------------
    m_pMyFileProcessor = new CFileProcess();
    m_pThreadFileProcessor = new QThread(this);
    m_pMyFileProcessor->moveToThread(m_pThreadFileProcessor);
    //当文件处理线程结束时，释放子线程中的资源
    connect(m_pThreadFileProcessor, &QThread::finished, m_pMyFileProcessor, &QObject::deleteLater);
    //connect( m_pThreadSocket,SIGNAL(finished()),m_pThreadFileProcessor,SLOT(terminate()));
    //connect(m_pMyThreadSocket,SIGNAL(stopped()),m_pMyFileProcessor,SLOT(on_socketThreadStopped()));
    //
    //主线程发出开始文件处理线程信号--子线程响应
    connect(this,SIGNAL(startFileProcessThread()),m_pMyFileProcessor,SLOT(start()));

    //主线程 控制子线程fileThread停止运行
    connect(this,SIGNAL(closeFileProcessThread(int,int)),m_pMyFileProcessor,SLOT(on_closeFileProcessThread(int,int)));

    //子线程完成了xml数据的生成，就发送信号交给主线程来处理
    connect(m_pMyFileProcessor,SIGNAL(xmlEventReady(QStringList)),m_pMyThreadSocket,SLOT(sendEventXml(QStringList)));
    connect(m_pMyFileProcessor,SIGNAL(xmlStatusReady(QStringList)),m_pMyThreadSocket,SLOT(sendStatusXml(QStringList)));
    //文件操作的信号
    connect(m_pMyFileProcessor,SIGNAL(fileOpenFailed(QString)),this,SLOT(on_fileOpenFiled(QString)));
    //子线程fileProcess已关闭 -- 将两个线程都终止
    connect(m_pMyFileProcessor,SIGNAL(closedFileProcessThread()),this,SLOT(on_closedFileProcessThread()));
    //----------------------------窗口关闭-释放资源----------------------------------

}

MainWindow::~MainWindow()
{
    qDebug()<<"MainWindow::~mainwindow() start";

    delete ui;
    delete Formlog;
    delete Formserver;
    delete Formset;
    delete m_pServerInfo;
    //delete m_pMyThreadSocket;
    //delete m_thread;

    //m_pThreadSocket->terminate();
   // m_pThreadFileProcessor->terminate();

    m_pThreadSocket->quit();
    m_pThreadSocket->wait();


    m_pThreadFileProcessor->quit();
    m_pThreadFileProcessor->wait();


    qDebug()<<"MainWindow::~mainwindow() end";
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(!m_bIsRuning){
        //退出
        if (!(QMessageBox::information(this,tr("退出程序"),tr("是否要关闭程序?"),tr("是"),tr("否"))))
            e->accept();
        else
            e->ignore();
    }else
        //关闭线程  -- 强制关闭并释放资源！！！！
        emit closeSocketThread();

}



void MainWindow::on_btn_run_clicked()
{
    if(ui->btn_run->text() == "运行"){
        if(!m_pServerInfo->isValidation())
        {
            QMessageBox::warning(this,"缺失信息","缺少Server信息,请补全后重试!");
            return ;
        }else if(m_LogPath == NULL){
            QMessageBox::warning(this,"缺失信息","缺少Log路径信息,请补全后重试!");
            return ;
        }

        //创建socket线程
        m_pMyThreadSocket->setPServerInfo(m_pServerInfo);
        m_pMyThreadSocket->setClientID("DG-20220101");

        m_pThreadSocket->start();
        emit startSocketThread();

    }else if(ui->btn_run->text() == "继续"){


        emit continueSocketThread();

    }
}

//选择Log文件所在文件夹
void MainWindow::on_actionLog_triggered()
{
    Formlog->show();
}

//配置服务器端
void MainWindow::on_actionServer_triggered()
{
    Formserver->show();
}

//设置菜单
void MainWindow::on_actionSet_triggered()
{
    Formset->show();
}

//槽函数 处理路径更新
void MainWindow::on_logpath_update(QString dirPath,QString event_path,QString status_path)
{
    m_LogPath = dirPath;
    m_LogPath_event = event_path;
    m_LogPath_status= status_path;
}

//槽函数  处理serverinfo更新
void MainWindow::on_serverInfo_Update(IpAddress IpInfo,int port,QString idCode)
{
    m_pServerInfo->setServerIP(IpInfo);
    m_pServerInfo->setServerPort(port);
    m_pServerInfo->setServerSecretKey(idCode);

}

//socket连接成功  更新ui
void MainWindow::on_connectedSuccess()
{
    // update ui
    m_bIsRuning = true;
    ui->actionLog->setEnabled(false);
    ui->actionServer->setEnabled(false);
    ui->btn_run->setEnabled(false);
    ui->btn_stop->setEnabled(true);
    ui->btn_close->setEnabled(false);
    lb_state->setText("当前客户端状态: 运行中...");
    lb_state->setStyleSheet("background-color: rgb(0, 255, 0)");


    //open subthread of fileProcess
    m_pMyFileProcessor->setFilePath(m_LogPath,m_LogPath_event,m_LogPath_status);
    //开启文件处理线程
    m_pThreadFileProcessor->start();
    emit startFileProcessThread();


}

void MainWindow::on_connectFailed()
{
    QMessageBox::warning(this,tr("连接失败"),tr("连接Sever发生错误,连接失败!"));
}

//子线程已停止，UI更新
void MainWindow::on_threadStopped()
{
  ui->btn_run->setText("继续");
  ui->btn_stop->setEnabled(false);
  ui->btn_run->setEnabled(true);
  ui->btn_close->setEnabled(true);
  lb_state->setStyleSheet("background-color: rgb(255, 0, 0)");
  lb_state->setText("当前客户端状态: 已暂停...");


}

//子线程socketThread继续运行  接着fileThread也继续运行
void MainWindow::on_threadContinued()
{
    ui->btn_run->setEnabled(false);
    ui->btn_stop->setEnabled(true);
    ui->btn_close->setEnabled(false);
    lb_state->setText("当前客户端状态: 运行中...");
    lb_state->setStyleSheet("background-color: rgb(0, 255, 0)");

}


void MainWindow::on_disconnectSuccess(int line1,int line2)
{
    QMessageBox::information(this,"Socket断开连接","已与服务器断开连接！");
    emit closeFileProcessThread(line1,line2);
}

//关闭两个线程
void MainWindow::on_closedFileProcessThread()
{
    m_pThreadFileProcessor->quit();
    m_pThreadFileProcessor->wait();

    m_pThreadSocket->quit();
    m_pThreadSocket->wait();

    //退出
    if (!(QMessageBox::information(this,tr("退出程序"),tr("是否要关闭程序?"),tr("是"),tr("否"))))
    {
        qApp->exit(0);
    }


}

void MainWindow::on_fileOpenFiled(QString fileRecord)
{
    QMessageBox::warning(this,"文件打开失败",fileRecord+" 打开失败...请检查后重试");
}



//停止按钮
void MainWindow::on_btn_stop_clicked()
{
    emit stopSocketThread();
}


/** 关闭按钮点击事件，去关闭主程序
 * @brief MainWindow::on_btn_close_clicked
 */
void MainWindow::on_btn_close_clicked()
{

    //没创建过线程，则直接关闭
    if(!m_bIsRuning){
        //退出
        if (!(QMessageBox::information(this,tr("退出程序"),tr("是否要关闭程序?"),tr("是"),tr("否"))))
        {
            //主程序退出
            qApp->exit(0);
        }
    }else
        //关闭线程  -- 强制关闭并释放资源！！！！
        emit closeSocketThread();

}

