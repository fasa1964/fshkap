#include "fshkwindow.h"
#include "ui_fshkwindow.h"


#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QFileDevice>
#include <QDataStream>

#include <formupdateinformation.h>

#include <QDebug>

FSHKWindow::FSHKWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FSHKWindow)
{
    ui->setupUi(this);

    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    ui->menuBar->addAction((date))->setDisabled(true);

    readSettings();

    readDataBetriebe();
    readDataLehrlinge();
    readDataProjekte();
    readDataSkills();

    if(projektMap.isEmpty())
        ui->actionPruefungen->setEnabled(false);

    if(skillsMap.isEmpty()){
        ui->actionZuordnen->setEnabled(false);
        ui->actionAuswerten->setEnabled(false);
    }



    formbetrieb = new FormBetrieb(this);
    formbetrieb->hide();
    connect(formbetrieb, &FormBetrieb::saveBetriebMap, this, &FSHKWindow::saveBetriebMap);
    connect(formbetrieb, &FormBetrieb::betriebRemoved, this, &FSHKWindow::betriebRemoved );
    connect(formbetrieb, &FormBetrieb::azubiRemoved, this, &FSHKWindow::azubiRemovedFromBetrieb );

    formlehrling = new FormLehrling(this);
    formlehrling->hide();
    connect(formlehrling, &FormLehrling::saveLehrlingMap , this, &FSHKWindow::saveLehrlingMap);
    connect(formlehrling, &FormLehrling::updateBetriebe , this, &FSHKWindow::updateBetriebe);


    formprojekt = new FormProjekt(this);
    formprojekt->hide();
    connect(formprojekt, &FormProjekt::saveProjekte, this, &FSHKWindow::saveProjektMap);
    connect(formprojekt, &FormProjekt::projektChanged, this, &FSHKWindow::projektChanged);
    connect(formprojekt, &FormProjekt::projektAdded , this, &FSHKWindow::projektAdded);

    formskills = new FormSkills(this);
    formskills->hide();
    connect(formskills, &FormSkills::saveSkillsMap, this, &FSHKWindow::saveSkillMap);


    formzuordnen = new FormZuordnen(this);
    formzuordnen->hide();
    connect(formzuordnen, &FormZuordnen::saveAzubiMap, this, &FSHKWindow::saveLehrlingMap);

    connect(ui->actionBeenden, &QAction::triggered , this, &FSHKWindow::actionBeendenClicked);
    connect(ui->actionInfo, &QAction::triggered , this, &FSHKWindow::actionInfoClicked);
    connect(ui->actionBetriebe, &QAction::triggered , this, &FSHKWindow::actionBetriebeClicked);
    connect(ui->actionAuszubildende, &QAction::triggered , this, &FSHKWindow::actionAuszubildendeClicked);
    connect(ui->actionProjekte, &QAction::triggered , this, &FSHKWindow::actionProjekteClicked);
    connect(ui->actionPruefungen, &QAction::triggered , this, &FSHKWindow::actionSkillsClicked);
    connect(ui->actionZuordnen, &QAction::triggered , this, &FSHKWindow::actionZuordnenClicked);
}

FSHKWindow::~FSHKWindow()
{
    delete ui;
}

bool FSHKWindow::saveDataBetriebe(const QMap<int, ClassBetrieb> &bMap)
{
    QFile file("Betriebe.dat");

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Speichern"),
                     tr("Die Datei -Betriebe.dat- lässt sich nicht speichern.\n")+file.errorString());
        return false;
    }

    QDataStream out(&file);
    quint32 magic = 13101964;
    quint32 version = QDataStream::Qt_5_9;
    out << magic;
    out << version;
    QMapIterator<int, ClassBetrieb> it(bMap);
    while (it.hasNext()) {
        it.next();
        out << it.value();
    }

    file.close();
    return true;
}

bool FSHKWindow::saveDataLehrlinge(const QMap<QString, ClassLehrling> &azuMap)
{
    QFile file("Lehrlinge.dat");

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Speichern"),
                     tr("Die Datei -Lehrlinge.dat- lässt sich nicht speichern.\n")+file.errorString());
        return false;
    }

    QDataStream out(&file);
    quint32 magic = 13101964;
    quint32 version = QDataStream::Qt_5_9;
    out << magic;
    out << version;
    QMapIterator<QString, ClassLehrling> it(azuMap);
    while (it.hasNext()) {
        it.next();
        out << it.value();
    }

    file.close();
    return true;
}

bool FSHKWindow::saveDataProjekte(const QMap<QString, ClassProjekt> &pMap)
{
    QFile file("Projekte.dat");

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Speichern"),
                     tr("Die Datei -Projekte.dat- lässt sich nicht speichern.\n")+file.errorString());
        return false;
    }

    QDataStream out(&file);
    quint32 magic = 13101964;
    quint32 version = QDataStream::Qt_5_9;
    out << magic;
    out << version;
    QMapIterator<QString, ClassProjekt> it(pMap);
    while (it.hasNext()) {
        it.next();
        out << it.value();
    }

    file.close();
    return true;
}

bool FSHKWindow::saveDataSkills(const QMap<QString, ClassSkills> &sMap)
{
    QFile file("Pruefungen.dat");

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Speichern"),
                     tr("Die Datei -Projekte.dat- lässt sich nicht speichern.\n")+file.errorString());
        return false;
    }

    QDataStream out(&file);
    quint32 magic = 13101964;
    quint32 version = QDataStream::Qt_5_9;
    out << magic;
    out << version;
    QMapIterator<QString, ClassSkills> it(sMap);
    while (it.hasNext()) {
        it.next();
        out << it.value();
    }

    file.close();
    return true;
}

void FSHKWindow::actionBeendenClicked()
{
    writeSettings();
    close();
}

void FSHKWindow::actionInfoClicked()
{
    QMessageBox::information(this, tr("Info"), tr("Noch nicht implementiert"));
}

void FSHKWindow::actionBetriebeClicked()
{
    this->takeCentralWidget();
    formbetrieb->show();
    formbetrieb->setBetriebMap(betriebMap);
    formbetrieb->updateBetriebTable(betriebMap);
    formbetrieb->setLastModified( lastUpdate("Betriebe.dat"));
    setCentralWidget(formbetrieb);
}

void FSHKWindow::actionAuszubildendeClicked()
{
    this->takeCentralWidget();
    formlehrling->show();
    formlehrling->setLehrlingMap(azubiMap);
    formlehrling->updateLehrlingTable(azubiMap);
    formlehrling->setBetriebMap(betriebMap);
    formlehrling->setLastModified(lastUpdate("Lehrlinge.dat"));
    setCentralWidget(formlehrling);
}

void FSHKWindow::actionProjekteClicked()
{
    this->takeCentralWidget();
    formprojekt->show();
    formprojekt->setProjektMap(projektMap);
    formprojekt->updateProjektTable(projektMap);
    setCentralWidget(formprojekt);
}

void FSHKWindow::actionSkillsClicked()
{
    this->takeCentralWidget();
    formskills->show();
    formskills->setSkillMap(skillsMap);
    formskills->setProjektMap(projektMap);
    setCentralWidget(formskills);
}

void FSHKWindow::actionZuordnenClicked()
{
    this->takeCentralWidget();
    formzuordnen->show();
    formzuordnen->setAzubiMap(azubiMap);
    formzuordnen->setupAzuSortBox();
    formzuordnen->setSkillMap(skillsMap);
    formzuordnen->setupSkillSortBox();
    setCentralWidget(formzuordnen);
}

void FSHKWindow::actionAktualisierenClicked()
{

}

// Signal from FormBetrieb
void FSHKWindow::saveBetriebMap(const QMap<int, ClassBetrieb> &bMap)
{
    bool ok = saveDataBetriebe(bMap);
    betriebMap = bMap;

    if(ok)
        ui->statusBar->showMessage(tr("Die Datei Betrieb.dat wurde erfolgreich aktualisiert!"), 5000);
    else
        ui->statusBar->showMessage(tr("Fehler beim Speichern der Datei Betrieb.dat!"), 5000);
}

void FSHKWindow::betriebRemoved(const ClassBetrieb &betrieb)
{
    bool dirty = false;
    QMapIterator<QString, ClassLehrling> it(azubiMap);
    while (it.hasNext()) {
        it.next();
        ClassLehrling azubi = it.value();
        if(azubi.betrieb() == betrieb.name())
        {
            azubi.setBetrieb("");
            azubiMap.insert(azubi.getKey(), azubi);
            dirty = true;
        }
    }

    if(dirty)
        saveLehrlingMap(azubiMap);
}

void FSHKWindow::azubiRemovedFromBetrieb(const ClassLehrling azu, const ClassBetrieb &betrieb)
{
    if(azu.betrieb() == betrieb.name())
    {
        ClassLehrling lehrling = azubiMap.value(azu.getKey());
        lehrling.setBetrieb("");
        azubiMap.insert(lehrling.getKey(), lehrling);
        saveLehrlingMap(azubiMap);
    }
}

// Signal from FormLehrling
void FSHKWindow::saveLehrlingMap(const QMap<QString, ClassLehrling> &azuMap)
{
    bool ok = saveDataLehrlinge(azuMap);
    azubiMap = azuMap;
    formlehrling->setLastModified( lastUpdate("Lehrlinge.dat") );

    if(ok)
        ui->statusBar->showMessage(tr("Die Datei Lehrlinge.dat wurde erfolgreich aktualisiert!"), 5000);
    else
        ui->statusBar->showMessage(tr("Fehler beim Speichern der Datei Lehrlinge.dat!"), 5000);

}

void FSHKWindow::updateBetriebe(const QString &betriebName, const ClassLehrling &azu)
{
    QMapIterator<int, ClassBetrieb> it(betriebMap);
    while (it.hasNext()) {
        it.next();
        ClassBetrieb betrieb = it.value();
        if(betriebName.isEmpty() && betrieb.azubiMap().keys().contains(azu.getKey()))
        {
            QMap<QString, ClassLehrling> aMap = betrieb.azubiMap();
            aMap.remove(azu.getKey());
            betrieb.setAzubiMap(aMap);
            betriebMap.insert(betrieb.nr(), betrieb);
        }

        if(betriebName == betrieb.name())
        {
            QMap<QString, ClassLehrling> aMap = betrieb.azubiMap();
            aMap.insert(azu.getKey(), azu);
            betrieb.setAzubiMap(aMap);
            betriebMap.insert(betrieb.nr(), betrieb);
        }

        if(betriebName != betrieb.name() && betrieb.azubiMap().keys().contains(azu.getKey()))
        {
            QMap<QString, ClassLehrling> aMap = betrieb.azubiMap();
            aMap.remove(azu.getKey());
            betrieb.setAzubiMap(aMap);
            betriebMap.insert(betrieb.nr(), betrieb);
        }
    }

    saveBetriebMap(betriebMap);

}

// Signal from FormProjekt
void FSHKWindow::saveProjektMap(const QMap<QString, ClassProjekt> &pMap)
{

    bool ok = saveDataProjekte(pMap);
    projektMap = pMap;
    if(ok)
        ui->statusBar->showMessage(tr("Die Datei Projekte.dat wurde erfolgreich aktualisiert!"), 5000);
    else
        ui->statusBar->showMessage(tr("Fehler beim Speichern der Datei Projekte.dat!"), 5000);

}

/// Update skill and azubi
void FSHKWindow::projektChanged(const ClassProjekt &pro)
{

    ClassProjekt projekt = pro;
    QStringList skillKeyList;
    QStringList azubiKeyList;

    // Update the skill when project exist
    QMapIterator<QString, ClassSkills> it(skillsMap);
    while (it.hasNext())
    {
        it.next();
        foreach(ClassProjekt p, it.value().getProjektMap().values())
        {
            if(p.getKey() == projekt.getKey())
            {
                    ClassSkills skill = it.value();
                    QMap<QString, ClassProjekt> spmap;
                    spmap = skill.getProjektMap();
                    spmap.insert(projekt.getKey(), projekt);
                    skill.setProjektMap(spmap);
                    skill.setCreatedDate(QDateTime::currentDateTime());
                    skillsMap.insert(skill.getKey(), skill);
                    saveSkillMap(skillsMap);
                    ui->statusBar->showMessage("Die Prüfung: "+skill.name()+"."+skill.kennung()+" wurde aktualisiert!",8000);
                    skillKeyList << skill.getKey();
            }
        }
    }

    // Update azubi when skill assigned
    bool saveAzubi = false;
    QMapIterator<QString, ClassLehrling> ia(azubiMap);
    while (ia.hasNext())
    {
        ia.next();
        foreach(ClassSkills skill, ia.value().getSkillMap().values())
        {
            if(skillKeyList.contains(skill.getKey()))
            {
                ClassLehrling azubi = ia.value();
                QMap<QString, ClassSkills> aMap;
                aMap = azubi.getSkillMap();
                int index = skillKeyList.indexOf(skill.getKey());
                ClassSkills ns = skillsMap.value( skillKeyList.at(index) );
                aMap.insert(ns.getKey(), ns);
                azubi.setSkillMap(aMap);
                azubiMap.insert(azubi.getKey(), azubi);
                azubiKeyList << azubi.getKey();
                saveAzubi = true;
            }
        }
    }

    if(saveAzubi)
    {
        saveLehrlingMap(azubiMap);
        QMap<QString, ClassSkills> sMap;
        QMap<QString, ClassLehrling> lMap;
        foreach(QString s, skillKeyList){
            ClassSkills skill = skillsMap.value(s);
            sMap.insert(skill.getKey(), skill);
        }

        foreach(QString a, azubiKeyList){
            ClassLehrling azu = azubiMap.value(a);
            lMap.insert(azu.getKey(), azu);
        }

        FormUpdateInformation *info = new FormUpdateInformation(sMap, lMap);
        info->show();
    }
}


void FSHKWindow::projektAdded(const ClassProjekt &pro)
{

    QMapIterator<QString, ClassSkills> it(skillsMap);
    while (it.hasNext()) {
        it.next();
        if(pro.kennung() == it.value().kennung())
        {
            QString s = it.value().name()+"."+it.value().kennung();
            int result = QMessageBox::question(this, tr("Projekt"),tr("Die Prüfung: ")+s+"\n"+
                         "mit der Kennung existiert bereits, soll das Projekt der Prüfung hinzugefügt werden?",
                         QMessageBox::Yes | QMessageBox::No);
            if(result == QMessageBox::No)
                return;

            ClassProjekt projekt = pro;
            ClassSkills skill = it.value();
            QMap<QString, ClassProjekt> pMap;
            pMap = skill.getProjektMap();
            pMap.insert(projekt.getKey(), projekt);
            skill.setProjektMap(pMap);
            skillsMap.insert(skill.getKey(), skill);
            saveSkillMap(skillsMap);
        }
    }
}

void FSHKWindow::saveSkillMap(const QMap<QString, ClassSkills> &sMap)
{
    bool ok = saveDataSkills(sMap);
    if(ok)
        ui->statusBar->showMessage(tr("Die Datei Pruefungen.dat wurde erfolgreich aktualisiert!"), 5000);
    else
        ui->statusBar->showMessage(tr("Fehler beim Speichern der Datei Pruefungen.dat!"), 5000);

}

void FSHKWindow::readDataBetriebe()
{
    QFile file("Betriebe.dat");
    if(!file.exists())
        return;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Laden"),
                     tr("Die Datei -Betriebe.dat- lässt sich nicht zum Lesen öffnen.\n")+file.errorString());
        return;
    }

    QDataStream in(&file);
    quint32 magic;
    quint32 version;
    in >> magic;
    if(magic != 13101964){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht das richtige Format!"));
        return;
    }

    in >> version;
    if(version != QDataStream::Qt_5_9){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht die richtige Version!"));
        return;
    }

    while (!in.atEnd())
    {
        ClassBetrieb betrieb;
        in >> betrieb;
        betriebMap.insert(betrieb.nr(), betrieb);
    }

    file.close();
}

void FSHKWindow::readDataLehrlinge()
{
    QFile file("Lehrlinge.dat");
    if(!file.exists())
        return;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Laden"),
                     tr("Die Datei -Lehrlinge.dat- lässt sich nicht zum Lesen öffnen.\n")+file.errorString());
        return;
    }

    QDataStream in(&file);
    quint32 magic;
    quint32 version;
    in >> magic;
    if(magic != 13101964){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht das richtige Format!"));
        return;
    }

    in >> version;
    if(version != QDataStream::Qt_5_9){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht die richtige Version!"));
        return;
    }

    while (!in.atEnd())
    {
        ClassLehrling azubi;
        in >> azubi;
        azubiMap.insert(azubi.getKey(), azubi);
    }

    file.close();
}

void FSHKWindow::readDataProjekte()
{
    QFile file("Projekte.dat");
    if(!file.exists())
        return;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Laden"),
                     tr("Die Datei -Projekte.dat- lässt sich nicht zum Lesen öffnen.\n")+file.errorString());
        return;
    }

    QDataStream in(&file);
    quint32 magic;
    quint32 version;
    in >> magic;
    if(magic != 13101964){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht das richtige Format!"));
        return;
    }

    in >> version;
    if(version != QDataStream::Qt_5_9){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht die richtige Version!"));
        return;
    }

    while (!in.atEnd())
    {
        ClassProjekt pro;
        in >> pro;
        projektMap.insert(pro.getKey(), pro);
    }

    file.close();
}

void FSHKWindow::readDataSkills()
{
    QFile file("Pruefungen.dat");
    if(!file.exists())
        return;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,  tr("Fehler Laden"),
                     tr("Die Datei -Pruefungen.dat- lässt sich nicht zum Lesen öffnen.\n")+file.errorString());
        return;
    }

    QDataStream in(&file);
    quint32 magic;
    quint32 version;
    in >> magic;
    if(magic != 13101964){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht das richtige Format!"));
        return;
    }

    in >> version;
    if(version != QDataStream::Qt_5_9){
        QMessageBox::warning(this, tr("Dateifehler"), tr("Die Datei hat nicht die richtige Version!"));
        return;
    }

    while (!in.atEnd())
    {
        ClassSkills skill;
        in >> skill;
        skillsMap.insert(skill.getKey(), skill);
    }

    file.close();
}

QDateTime FSHKWindow::lastUpdate(const QString &filename)
{
    QDateTime dt;
    QFileInfo file(filename);
    dt = file.lastModified();
    return dt;
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
