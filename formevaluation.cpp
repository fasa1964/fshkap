#include "formevaluation.h"
#include "ui_formevaluation.h"

FormEvaluation::FormEvaluation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEvaluation)
{
    ui->setupUi(this);
}

FormEvaluation::~FormEvaluation()
{
    delete ui;
}
