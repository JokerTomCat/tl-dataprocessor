#ifndef FORMSET_H
#define FORMSET_H

#include <QWidget>

namespace Ui {
class FormSet;
}

class FormSet : public QWidget
{
    Q_OBJECT

public:
    explicit FormSet(QWidget *parent = nullptr);
    ~FormSet();

private:
    Ui::FormSet *ui;
};

#endif // FORMSET_H
