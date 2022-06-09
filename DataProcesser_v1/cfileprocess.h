#ifndef CFILEPROCESS_H
#define CFILEPROCESS_H
#include<QObject>
#include<QString>
#include<QDate>
#include<QFile>
#include<QFileSystemWatcher>

#include"ToolsClass.h"
class CFileProcess : public QObject
{
    Q_OBJECT
public:
    explicit CFileProcess(QObject *parent = nullptr);
    void setFilePath(const QString &newFilePath,const QString &newFilePath_event,const QString &newFilePath_status);
    void initFileObject();

protected:
    void processFileOfEvent();
    void processFileOfStatus();
signals:
    void xmlEventReady(QStringList);
    void xmlStatusReady(QStringList);
    void fileOpenFailed(QString);
    void closedFileProcessThread();

private:
    QString m_filePath; //文件夹
    QString m_file_status; // 绝对路径
    QString m_file_event;
    QString m_recordFile; //记录文件
    //文件监视器
    QFileSystemWatcher *m_pFileWatcher;
    //bool m_bRun;
    int m_lastReadRowsNum;

//    QFile m_pStatusFile;
//    QFile m_pEventFile;

private slots:
    void start();
    void on_fileChanged(QString);
    void on_closeFileProcessThread(int,int);


};



#endif // CFILEPROCESS_H
