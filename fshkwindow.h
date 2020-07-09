#ifndef FSHKWINDOW_H
#define FSHKWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <classbetrieb.h>
#include <classlehrling.h>

#include <formbetrieb.h>
#include <formlehrling.h>


namespace Ui {
class FSHKWindow;
}

class FSHKWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FSHKWindow(QWidget *parent = nullptr);
    ~FSHKWindow();

    bool saveDataBetriebe(const QMap<int, ClassBetrieb> &bMap);
    bool saveDataLehrlinge(const QMap<QString, ClassLehrling> &azuMap);

private slots:
    void actionBeendenClicked();
    void actionInfoClicked();
    void actionBetriebeClicked();
    void actionAuszubildendeClicked();
    void actionProjekteClicked();


    // Signal from FormBetrieb
    void saveBetriebMap(const QMap<int, ClassBetrieb> &bMap);
    void betriebRemoved(const ClassBetrieb &betrieb);
    void azubiRemovedFromBetrieb(const ClassLehrling azu, const ClassBetrieb &betrieb);

    // Signal from FormLehrling
    void saveLehrlingMap(const QMap<QString, ClassLehrling> &azuMap);
    void updateBetriebe(const QString &betriebName, const ClassLehrling &azu);

private:
    Ui::FSHKWindow *ui;

    FormBetrieb *formbetrieb;
    QMap<int, ClassBetrieb> betriebMap;

    FormLehrling *formlehrling;
    QMap<QString, ClassLehrling> azubiMap;

    void readDataBetriebe();
    void readDataLehrlinge();


    QDateTime lastUpdate(const QString &filename);

    void readSettings();
    void writeSettings();
};

#endif // FSHKWINDOW_H
