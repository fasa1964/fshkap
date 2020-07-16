#include "formskills.h"
#include "ui_formskills.h"

FormSkills::FormSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSkills)
{
    ui->setupUi(this);
}

FormSkills::~FormSkills()
{
    delete ui;
}
