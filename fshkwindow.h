#ifndef FSHKWINDOW_H
#define FSHKWINDOW_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class FSHKWindow;
}

class FSHKWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FSHKWindow(QWidget *parent = nullptr);
    ~FSHKWindow();

private slots:
    void actionBeendenClicked();
    void actionInfoClicked();

private:
    Ui::FSHKWindow *ui;


    void readSettings();
    void writeSettings();
};

#endif // FSHKWINDOW_H
