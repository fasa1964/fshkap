#include "formlehrling.h"
#include "ui_formlehrling.h"

FormLehrling::FormLehrling(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLehrling)
{
    ui->setupUi(this);
}

FormLehrling::~FormLehrling()
{
    delete ui;
}
