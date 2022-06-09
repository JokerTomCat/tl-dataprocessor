#ifndef FORMSERVER_H
#define FORMSERVER_H

#include <QWidget>
#include"ToolsClass.h"


namespace Ui {
class FormServer;
}

class FormServer : public QWidget
{
    Q_OBJECT

public:
    explicit FormServer(QWidget *parent = nullptr);
    ~FormServer();

    bool initAddress(QString,IpAddress*);

private slots:
    void on_btn_internetselect_clicked();

    void on_btn_internetselect_2_clicked();

private:
    Ui::FormServer *ui;

signals:
    void serverInfoUpdate(IpAddress,int,QString);

};

#endif // FORMSERVER_H
