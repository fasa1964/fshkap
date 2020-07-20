#ifndef FSHKWINDOW_H
#define FSHKWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <classbetrieb.h>
#include <classlehrling.h>
#include <classskills.h>

#include <formbetrieb.h>
#include <formlehrling.h>
#include <formprojekt.h>
#include <formskills.h>
#include <formzuordnen.h>

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
    bool saveDataProjekte(const QMap<QString, ClassProjekt> &pMap);
    bool saveDataSkills(const QMap<QString, ClassSkills> &pMap);

private slots:
    void actionBeendenClicked();
    void actionInfoClicked();
    void actionBetriebeClicked();
    void actionAuszubildendeClicked();
    void actionProjekteClicked();
    void actionSkillsClicked();

    void actionZuordnenClicked();
    void actionAktualisierenClicked();


    // Signal from FormBetrieb
    void saveBetriebMap(const QMap<int, ClassBetrieb> &bMap);
    void betriebRemoved(const ClassBetrieb &betrieb);
    void azubiRemovedFromBetrieb(const ClassLehrling azu, const ClassBetrieb &betrieb);

    // Signal from FormLehrling
    void saveLehrlingMap(const QMap<QString, ClassLehrling> &azuMap);
    void updateBetriebe(const QString &betriebName, const ClassLehrling &azu);

    // Signal from FormProjekt
    void saveProjektMap(const QMap<QString, ClassProjekt> &pMap);
    void projektChanged(const ClassProjekt &pro);
    void projektAdded(const ClassProjekt &pro);

    // Signal from FormSkills
    void saveSkillMap(const QMap<QString, ClassSkills> &sMap);

private:
    Ui::FSHKWindow *ui;

    FormBetrieb *formbetrieb;
    QMap<int, ClassBetrieb> betriebMap;

    FormLehrling *formlehrling;
    QMap<QString, ClassLehrling> azubiMap;

    FormProjekt *formprojekt;
    QMap<QString, ClassProjekt> projektMap;

    FormSkills *formskills;
    QMap<QString, ClassSkills> skillsMap;

    FormZuordnen *formzuordnen;

    void readDataBetriebe();
    void readDataLehrlinge();
    void readDataProjekte();
    void readDataSkills();


    QDateTime lastUpdate(const QString &filename);

    void readSettings();
    void writeSettings();
};

#endif // FSHKWINDOW_H
