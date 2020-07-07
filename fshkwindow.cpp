#include "fshkwindow.h"
#include "ui_fshkwindow.h"

FSHKWindow::FSHKWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FSHKWindow)
{
    ui->setupUi(this);
}

FSHKWindow::~FSHKWindow()
{
    delete ui;
}
