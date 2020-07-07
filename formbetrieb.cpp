#include "formbetrieb.h"
#include "ui_formbetrieb.h"

FormBetrieb::FormBetrieb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBetrieb)
{
    ui->setupUi(this);
}

FormBetrieb::~FormBetrieb()
{
    delete ui;
}
