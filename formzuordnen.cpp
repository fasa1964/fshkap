#include "formzuordnen.h"
#include "ui_formzuordnen.h"

FormZuordnen::FormZuordnen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormZuordnen)
{
    ui->setupUi(this);
}

FormZuordnen::~FormZuordnen()
{
    delete ui;
}
