#include "fshkwindow.h"
#include "ui_fshkwindow.h"

FSHKWindow::FSHKWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FSHKWindow)
{
    ui->setupUi(this);
    readSettings();

    connect(ui->actionBeenden, &QAction::triggered , this, &FSHKWindow::actionBeendenClicked);
}

FSHKWindow::~FSHKWindow()
{
    delete ui;
}

void FSHKWindow::actionBeendenClicked()
{
    writeSettings();
    close();
}

void FSHKWindow::actionInfoClicked()
{

}


void FSHKWindow::readSettings()
{
    QSettings settings("fasa@shkap", "FSHK-AP");
    QRect rect = settings.value("geometrie", QRect(0,0,800,550)).toRect();
    setGeometry(rect);
}

void FSHKWindow::writeSettings()
{
    QSettings settings("fasa@shkap", "FSHK-AP");
    settings.setValue("geometrie", this->geometry());
}
