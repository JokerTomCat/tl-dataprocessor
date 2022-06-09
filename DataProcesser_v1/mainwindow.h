#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QThread>
#include<QLabel>
#include<QCloseEvent>
#include"formlog.h"
#include"formserver.h"
#include"formset.h"
#include"cserverinfo.h"
#include"cthreadsocket.h"
#include"cfileprocess.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent*);


private slots:


    void on_actionLog_triggered();

    void on_actionServer_triggered();

    void on_actionSet_triggered();

    void on_logpath_update(QString,QString,QString);

    void on_serverInfo_Update(IpAddress,int,QString);

    void on_connectedSuccess();
    void on_connectFailed();
    void on_threadStopped();
    void on_threadContinued();
    void on_disconnectSuccess(int,int);

    void on_closedFileProcessThread();


    void on_fileOpenFiled(QString);

    void on_btn_run_clicked();

    void on_btn_stop_clicked();

    void on_btn_close_clicked();

private:
    Ui::MainWindow *ui;



private:
    //控件
    QLabel *lb_state;

    //子窗体
    FormLog *Formlog;
    FormServer *Formserver;
    FormSet *Formset;

    //log文件路径
    QString m_LogPath;
    QString m_LogPath_event;
    QString m_LogPath_status;
    //Serverinfo
    CServerInfo *m_pServerInfo;
    //socket线程
    CThreadSocket *m_pMyThreadSocket;
    QThread *m_pThreadSocket;
    //file process
    CFileProcess *m_pMyFileProcessor;
    QThread *m_pThreadFileProcessor;

    //用于关闭界面时判断是否创建过线程
    bool m_bIsRuning;

  signals:
    void startSocketThread();
    void stopSocketThread();
    void closeSocketThread();
    void continueSocketThread();
    void startFileProcessThread();
    void closeFileProcessThread(int,int);

};
#endif // MAINWINDOW_H
