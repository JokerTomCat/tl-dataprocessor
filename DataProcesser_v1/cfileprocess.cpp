#include "cfileprocess.h"

#include<QThread>
#include<QDebug>
#include<QTextStream>

CFileProcess::CFileProcess(QObject *parent) : QObject(parent)
{
    m_pFileWatcher = new QFileSystemWatcher;

}

void CFileProcess::setFilePath(const QString &newFilePath,const QString &newFilePath_event,const QString &newFilePath_status)
{
    m_filePath = newFilePath;
    m_file_event = newFilePath_event;
    m_file_status = newFilePath_status;
}


void CFileProcess::initFileObject()
{

    //文件内容监视器
    m_pFileWatcher->addPath(m_file_event);
    m_pFileWatcher->addPath(m_file_status);

    //初始化文件读取记录....后期采用数据库形式储存，现在用文件
    QString filename= "lastReadRecord.txt";
    m_recordFile = m_filePath +"/"+filename;
    m_recordFile.replace("/","\\");
    QFile lastRecordFile(m_recordFile);
    if(!lastRecordFile.exists()){

        lastRecordFile.open(QIODevice::ReadWrite|QIODevice::Text);
        QTextStream streamFile(&lastRecordFile);
        streamFile<<m_file_event+"|"+"0\n";
        streamFile<<m_file_status+"|"+"0\n";
        lastRecordFile.close();
    }

}

void CFileProcess::processFileOfEvent()
{
    qDebug()<<"CFileProcess::processFileOfEvent()    start;"<<QThread::currentThreadId();

    //--------------------------获取上次读取到的行数----------------------------
    QFile lastRecordFile(m_recordFile);
    //m_lastReadRowsNum
    if(!lastRecordFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        emit fileOpenFailed(m_recordFile);
        return;

    }
    //文件内容记录对象
    SfileRecord recordContent;
    int lineNo = 0;
    while (!lastRecordFile.atEnd())
    {
        QByteArray line = lastRecordFile.readLine();
        QString str(line);
        QStringList s = str.split("|");

        if(lineNo == 0){
            recordContent.line1_filename = s[0];
            recordContent.line1_rowsNo = s[1].toInt();
            lineNo++;
        }else {
            recordContent.line2_filename = s[0];
            recordContent.line2_rowsNo = s[1].toInt();
            lineNo++;
        }
    }
    lastRecordFile.close();



    //--------------------------读取log文件的更新内容--------------------------------
    QFile eventFile(m_file_event);
    //打开文件
    if(!eventFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit   fileOpenFailed(m_file_event);
        return;
    }
    QStringList newEventContent;
    int readlineNo = 0;

    while(!eventFile.atEnd()){
        QByteArray line = eventFile.readLine();
        //读到上次读到的结尾处
        //qDebug()<<QString("当前行:%1 --  上次行:%2").arg(readlineNo).arg(recordContent.line1_rowsNo);
        if(readlineNo >= recordContent.line1_rowsNo){
            newEventContent.append(QString(line));
        }
        readlineNo++;
    }
    eventFile.close();


    //------------------------更新上次读取的行数----------------------------
    recordContent.line1_rowsNo = readlineNo;
    //更新记录文件内容
    if(!lastRecordFile.open(QIODevice::WriteOnly|QIODevice::Text)){
        emit fileOpenFailed(m_recordFile);
        return;
    }
    //写入记录
    QTextStream in(&lastRecordFile);
    in<<recordContent.getLine1()+"\n";
    in<<recordContent.getLine2()+"\n";
    lastRecordFile.close();


    //将更新的内容发送给socket线程去传输
    emit xmlEventReady(newEventContent);


    qDebug()<<"CFileProcess::processFileOfEvent()    end;"<<QThread::currentThreadId();

}



void CFileProcess::processFileOfStatus()
{
    qDebug()<<"CFileProcess::processFileOfStatus()    start;"<<QThread::currentThreadId();

    //--------------------------获取上次读取到的行数----------------------------
    QFile lastRecordFile(m_recordFile);
    //m_lastReadRowsNum
    if(!lastRecordFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        emit fileOpenFailed(m_recordFile);
        return;

    }
    //文件内容记录对象
    SfileRecord recordContent;
    int lineNo = 0;
    while (!lastRecordFile.atEnd())
    {
        QByteArray line = lastRecordFile.readLine();
        QString str(line);
        QStringList s = str.split("|");

        if(lineNo == 0){
            recordContent.line1_filename = s[0];
            recordContent.line1_rowsNo = s[1].toInt();
            lineNo++;
        }else {
            recordContent.line2_filename = s[0];
            recordContent.line2_rowsNo = s[1].toInt();
            lineNo++;
        }
    }
    lastRecordFile.close();



    //--------------------------读取log文件的更新内容--------------------------------
    QFile statusFile(m_file_status);
    //打开文件
    if(!statusFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit   fileOpenFailed(m_file_status);
        return;
    }
    QStringList newStatusContent;
    int readlineNo = 0;

    while(!statusFile.atEnd()){
        QByteArray line = statusFile.readLine();
        //读到上次读到的结尾处
        if(readlineNo >= recordContent.line2_rowsNo){
            newStatusContent.append(QString(line));
        }
        readlineNo++;
    }
    statusFile.close();


    //------------------------更新上次读取的行数----------------------------
    recordContent.line2_rowsNo = readlineNo;
    //更新记录文件内容
    if(!lastRecordFile.open(QIODevice::WriteOnly|QIODevice::Text)){
        emit fileOpenFailed(m_recordFile);
        return;
    }
    //写入记录
    QTextStream in(&lastRecordFile);
    in<<recordContent.getLine1()+"\n";
    in<<recordContent.getLine2()+"\n";
    lastRecordFile.close();

    //将更新的内容发送给socket线程去传输
    emit xmlStatusReady(newStatusContent);


    qDebug()<<"CFileProcess::processFileOfStatus()    end;"<<QThread::currentThreadId();
}


//处理主线程信号 start()
void CFileProcess::start()
{
    qDebug()<<"CFileProcess::start()    start"<<QThread::currentThreadId();



    //initial
    initFileObject();
    connect(m_pFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(on_fileChanged(QString)));




    qDebug()<<"CFileProcess::start()    end"<<QThread::currentThreadId();
}



void CFileProcess::on_fileChanged(QString file)
{
    if(file == m_file_event)
        processFileOfEvent();
    else if(file == m_file_status)
        processFileOfStatus();

}

/**
 * 在socket线程关闭后 来关闭文件处理线程
 * @brief CFileProcess::on_closeFileProcessThread
 * @param event_cacheline  事件文件在socket线程中缓存的记录数
 * @param status_cacheline 状态文件在socket线程中缓存的记录数
 */
void CFileProcess::on_closeFileProcessThread(int event_cacheline,int status_cacheline)
{
    //关闭文件监听
    m_pFileWatcher->removePath(m_file_event);
    m_pFileWatcher->removePath(m_file_status);
    m_pFileWatcher->deleteLater();

    //更新lastRecord记录文件内容
    QFile lastRecordFile(m_recordFile);
    if(!lastRecordFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit fileOpenFailed(m_recordFile);
        return;
    }
    SfileRecord recordContent;
    int lineNo = 0;
    while (!lastRecordFile.atEnd())
    {
        QByteArray line = lastRecordFile.readLine();
        QString str(line);
        QStringList s = str.split("|");

        if(lineNo == 0){
            recordContent.line1_filename = s[0];
            recordContent.line1_rowsNo = s[1].toInt();
            lineNo++;
        }else {
            recordContent.line2_filename = s[0];
            recordContent.line2_rowsNo = s[1].toInt();
            lineNo++;
        }
    }
    lastRecordFile.close();

    recordContent.line1_rowsNo -= event_cacheline;
    recordContent.line2_rowsNo -= status_cacheline;

    //写入记录
    if(!lastRecordFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        emit fileOpenFailed(m_recordFile);
        return;
    }
    QTextStream in(&lastRecordFile);
    in<<recordContent.getLine1()+"\n";
    in<<recordContent.getLine2()+"\n";
    lastRecordFile.close();

    emit closedFileProcessThread();

}

