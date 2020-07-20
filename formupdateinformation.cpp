#include "formupdateinformation.h"
#include "ui_formupdateinformation.h"

FormUpdateInformation::FormUpdateInformation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUpdateInformation)
{
    ui->setupUi(this);
}

FormUpdateInformation::~FormUpdateInformation()
{
    delete ui;
}
