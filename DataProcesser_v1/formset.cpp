#include "formset.h"
#include "ui_formset.h"

FormSet::FormSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSet)
{
    ui->setupUi(this);
}

FormSet::~FormSet()
{
    delete ui;
}
