#ifndef FSHKWINDOW_H
#define FSHKWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QVariant>

#include <classbetrieb.h>
#include <classlehrling.h>
#include <classskills.h>

#include <formbetrieb.h>
#include <formlehrling.h>
#include <formprojekt.h>
#include <formskills.h>
#include <formzuordnen.h>
#include <formevaluation.h>

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

    static QVariant getValue(const QString &key){
        QMap<QString, QString> map;
        map.insert("test", "From static function");

        return map.value(key);
    }

private slots:
    void actionBeendenClicked();
    void actionInfoClicked();
    void actionBetriebeClicked();
    void actionAuszubildendeClicked();
    void actionProjekteClicked();
    void actionSkillsClicked();

    void actionZuordnenClicked();
    void actionAuswertungClicked();
    void actionAktualisierenClicked();
    void actionAddUserClicked();


    // Signal from FormBetrieb
    void saveBetriebMap(const QMap<int, ClassBetrieb> &bMap);
    void betriebRemoved(const ClassBetrieb &company);
    void azubiRemovedFromBetrieb(const ClassLehrling azu, const ClassBetrieb &company);

    // Signal from FormLehrling
    void saveLehrlingMap(const QMap<QString, ClassLehrling> &azuMap);
    void updateBetriebe(const QString &betriebName, const ClassLehrling &azu);

    // Signal from FormProjekt
    void saveProjektMap(const QMap<QString, ClassProjekt> &pMap);
    void projektChanged(const ClassProjekt &pro);
    void projektAdded(const ClassProjekt &pro);

    // Signal from FormSkills
    void saveSkillMap(const QMap<QString, ClassSkills> &sMap);
    void skillChanged(ClassSkills skill);
    void skillProjectChanged(ClassProjekt pro);

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

    FormEvaluation *formevaluation;

    QMap<QString, QVariant> lehrlingColorMap;
    void setupColorMap();

    void readDataBetriebe();
    void readDataLehrlinge();
    void readDataProjekte();
    void readDataSkills(); 

    // Test
    bool isDifferent(ClassFrage oldQuestion, ClassFrage newQuestion);
    QList<ClassLehrling> containsSkill(ClassSkills skill);


    QDateTime lastUpdate(const QString &filename);

    void readSettings();
    void writeSettings();
};

#endif // FSHKWINDOW_H
