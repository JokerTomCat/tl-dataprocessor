#ifndef FORMLOG_H
#define FORMLOG_H

#include <QWidget>
namespace Ui {
class FormLog;
}

class FormLog : public QWidget
{
    Q_OBJECT

public:
    explicit FormLog(QWidget *parent = nullptr);
    ~FormLog();

signals:
    void logPathUpdate(QString,QString,QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FormLog *ui;
};

#endif // FORMLOG_H
