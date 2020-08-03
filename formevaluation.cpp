#include "formevaluation.h"
#include "ui_formevaluation.h"

#include <QDate>
#include <QDateTime>
#include <QPalette>
#include <QMessageBox>
#include <QtMath>

#include <QDebug>

FormEvaluation::FormEvaluation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEvaluation)
{
    ui->setupUi(this);

    dirty = false;
    selectedLehrling = ClassLehrling();
    selectedSkill = ClassSkills();

    connect(ui->closeButton, &QPushButton::clicked, this, &FormEvaluation::close);
//    connect(ui->azubiSortBox, &QComboBox::currentTextChanged, this, &FormEvaluation::sortBoxTextChanged);
    connect(ui->azubiListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::azubiBoxTextChanged);
    connect(ui->skillListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::skillBoxTextChanged);
    connect(ui->projektListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::projectBoxTextChanged);

}

FormEvaluation::~FormEvaluation()
{
    delete ui;
}

/// !brief Sort all apprentices in years
void FormEvaluation::updateSortBox()
{
    if(m_azubiMap.isEmpty())
        return;

    disconnect(ui->azubiSortBox, &QComboBox::currentTextChanged, this, &FormEvaluation::sortBoxTextChanged);
    QStringList sortLabels;
    ui->azubiSortBox->clear();

    for(int i = 1; i < 6; i++){

        QMap<QString, ClassLehrling> aMap;
        aMap = apprenticeship(i);
        if(!aMap.isEmpty()){
            QString label = QString::number(i,10)+".Lehrjahr";
            if(i > 4)
                label = "Nachholer";
            sortLabels << label;
        }
    }

    ui->azubiSortBox->addItems(sortLabels);
    connect(ui->azubiSortBox, &QComboBox::currentTextChanged, this, &FormEvaluation::sortBoxTextChanged);
    ui->azubiSortBox->setCurrentText(sortLabels.first());
    sortBoxTextChanged(sortLabels.first());

}

/// !brief When the apprentices ship changed
void FormEvaluation::sortBoxTextChanged(const QString &text)
{
    ui->azubiListBox->clear();
    int year = QString("%1").arg(text[0],1).toInt();
    QMap<QString, ClassLehrling> sortedMap;
    sortedMap = apprenticeship(year);

    ui->azubiListBox->addItems(sortedMap.keys());
    ui->countAzubiBox->setValue(sortedMap.values().size());
    //connect(ui->azubiListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::azubiBoxTextChanged);
}

/// !brief When the name of apprentices changed
void FormEvaluation::azubiBoxTextChanged(const QString &text)
{
    selectedLehrling = m_azubiMap.value(text);
    updateSkillBox(selectedLehrling);
}

void FormEvaluation::skillBoxTextChanged(const QString &text)
{
    selectedSkill = selectedLehrling.getSkillMap().value(text);
    ui->projektListBox->clear();
    ui->projektListBox->addItems(selectedSkill.getProjektMap().keys());
}

void FormEvaluation::projectBoxTextChanged(const QString &text)
{
    QMap<QString, ClassProjekt> pMap;
    pMap = selectedSkill.getProjektMap();
    selectedProjekt = pMap.value(text);
    ui->projektNameEdit->setText(selectedProjekt.name());
    setupQuestionTable(selectedProjekt);
}

/// !brief Calculate the points of curret project
void FormEvaluation::questionTableCellChanged(int, int)
{
    int points = 0;
    QMap<int, ClassFrage> fMap;
    fMap = selectedProjekt.questionMap();
    for(int i = 0; i < ui->fragenTableWidget->rowCount(); i++){
        int p = ui->fragenTableWidget->item(i, 2)->text().toInt();
        ClassFrage frg = fMap.value(i);
        frg.setPoints(p);
        fMap.insert(i, frg);
        points = points + p;
    }

    int maxPoints = selectedProjekt.maxPoints();
    double percent = points * 100.0 / maxPoints;
    if(percent < 50.0)
        setTextColor(ui->percentBox, Qt::red);
    else
        setTextColor(ui->percentBox, Qt::darkGreen);

    ui->percentBox->setValue(percent);
    selectedProjekt.setQuestionMap(fMap);
    selectedSkill.insertProjekt(selectedProjekt);
    selectedLehrling.insertSkill(selectedSkill);
    dirty = true;
    ui->saveButton->setEnabled(true);

}

void FormEvaluation::setupQuestionTable(ClassProjekt pro)
{

    disconnect(ui->fragenTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::questionTableCellChanged);

    ui->fragenTableWidget->clear();
    ui->fragenTableWidget->setColumnCount(5);
    ui->fragenTableWidget->setRowCount(pro.questionMap().size());

    QStringList labels;
    labels << "Nr." << "Frage" << "Punkte" << "Max. Punkte" << "Kennung" ;
    ui->fragenTableWidget->setHorizontalHeaderLabels(labels);

    QMap<int, ClassFrage> fMap = pro.questionMap();

    int row = 0;
    QMapIterator<int, ClassFrage> it(fMap);
    while (it.hasNext()) {
        it.next();

        ClassFrage question = it.value();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(question.questionNr(),10) );
        QTableWidgetItem *itemFrage = new QTableWidgetItem( question.question() );
        QTableWidgetItem *itemPunkte = new QTableWidgetItem( QString::number(question.points(),10) );
        QTableWidgetItem *itemMaxPunkte = new QTableWidgetItem( QString::number(question.maxPoints(),10) );
        QTableWidgetItem *itemKennung = new QTableWidgetItem( question.identifier() );
        ui->fragenTableWidget->setItem(row,0, itemNr);
        ui->fragenTableWidget->setItem(row,1, itemFrage);
        ui->fragenTableWidget->setItem(row,2, itemPunkte);
        ui->fragenTableWidget->setItem(row,3, itemMaxPunkte);
        ui->fragenTableWidget->setItem(row,4, itemKennung);

        itemNr->setFlags(Qt::ItemIsEnabled);
        itemFrage->setFlags(Qt::ItemIsEnabled);
        itemMaxPunkte->setFlags(Qt::ItemIsEnabled);
        itemKennung->setFlags(Qt::ItemIsEnabled);
        itemPunkte->setTextColor(Qt::blue);

        row++;
    }

    ui->fragenTableWidget->resizeColumnToContents(0);
    ui->fragenTableWidget->resizeColumnToContents(1);
    ui->fragenTableWidget->resizeColumnToContents(2);
    ui->fragenTableWidget->resizeColumnToContents(3);
    ui->fragenTableWidget->resizeColumnToContents(4);

    connect(ui->fragenTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::questionTableCellChanged);

}

void FormEvaluation::updateSkillBox(const ClassLehrling &azu)
{
    ui->skillListBox->clear();
    ui->projektListBox->clear();
    ui->fragenTableWidget->clear();
    ui->projektNameEdit->clear();
    ui->percentBox->setValue(0);


    if(azu.getSkillMap().isEmpty()){
        setTextColor(ui->azubiListBox, Qt::red);
        selectedSkill = ClassSkills();
        ui->countSkillBox->setValue(0);
    }else{
        setTextColor(ui->azubiListBox, Qt::blue);
        ui->skillListBox->addItems(selectedLehrling.getSkillMap().keys());
        ui->countSkillBox->setValue(selectedLehrling.getSkillMap().values().size());
    }

    ui->nrBox->setValue(azu.nr());
    ui->azuNameEdit->setText(azu.firstname()+"."+azu.surname());
}

QMap<QString, ClassLehrling> FormEvaluation::azubiMap() const
{
    return m_azubiMap;
}

void FormEvaluation::setAzubiMap(const QMap<QString, ClassLehrling> &azubiMap)
{
    m_azubiMap = azubiMap;
}

/// !brief Returns a map of all apprentices that belong to year
QMap<QString, ClassLehrling> FormEvaluation::apprenticeship(int year)
{
    QMap<QString, ClassLehrling> sortMap;

    if(m_azubiMap.isEmpty())
        return sortMap;

    QDate today = QDate::currentDate();
    QMapIterator<QString, ClassLehrling> it(m_azubiMap);
    while (it.hasNext()) {
        it.next();
        ClassLehrling azu = it.value();
        int nyear = today.year() - azu.apprenticeshipDate().year();
        if( nyear <= 0)
            nyear = 1;
        if(nyear > 5)
            nyear = 5;

        if(nyear == year)
            sortMap.insert(azu.getKey(), azu);
    }

    return sortMap;
}

void FormEvaluation::setTextColor(QWidget *widget, QColor color)
{
    QPalette pal;
    pal = widget->palette();
    pal.setColor(QPalette::WindowText, color);
    pal.setColor(QPalette::Text, color);
    widget->setPalette(pal);
}

//void FormEvaluation::update()
//{
//    if(azubiMap.isEmpty()){
//        QMessageBox::information(this, tr("Update"), tr("Keine Daten vorhanden!"));
//        return;
//    }

//    setupSortBox();

//}

//void FormEvaluation::azubiSortBoxChanged(const QString &text)
//{
//    if(text.isEmpty())
//        return;

//    int jahrgang;
//    QList<ClassLehrling> jahrgangList;
//    QStringList azubiLabels;
//    QList<int>redList;

//    if(text != "Nachholer"){
//        jahrgang = QString("%1").arg(text[0],1).toInt();
//        jahrgangList = getAzubiList(jahrgang);
//    }
//    if(text == "Nachholer"){
//        jahrgang = 5;
//        jahrgangList = getAzubiList(jahrgang);
//    }

//    ui->azubiListBox->clear();
//    foreach(ClassLehrling azu, jahrgangList){
//        azubiLabels << azu.getKey();
//        if(azu.getSkillMap().size() <= 0)
//            redList << azubiLabels.size()-1;
//    }

//    model->setLabels(azubiLabels);
//    model->setRedList(redList);

//    ui->azubiListBox->setModel(testmodel);
//    ui->countAzubiBox->setValue(azubiLabels.size());
//}

//void FormEvaluation::azubiListBoxChanged(const QString &text)
//{
//    if(text.isEmpty())
//        return;

//    int index = ui->azubiListBox->currentIndex();
//    qDebug() << index;
//    qDebug() << text;

//    if(azubiMap.value(text).getSkillMap().size() <= 0)
//        setTextColor(ui->azubiListBox, Qt::red);
//    else
//        setTextColor(ui->azubiListBox, Qt::blue);
//    if(dirty){
//        int result = QMessageBox::question(this, tr("Daten speichern"), tr("Daten haben sich geändert!\n"
//                      "Wenn sie nicht speichern, gehen die Daten verloren!"), QMessageBox::Save | QMessageBox::Cancel);
//        if(result == QMessageBox::Cancel)
//            return;


//    }

//    selectedAzubi = azubiMap().value(text);
//    currentSkillMap = selectedAzubi.getSkillMap();

//    ui->nrBox->setValue(selectedAzubi.nr());
//    ui->azuNameEdit->setText(selectedAzubi.firstname()+"."+selectedAzubi.surname());


//    QList<ClassSkills> skillList;
//    QStringList skillLabels;
//    skillList = getSkillList(selectedAzubi);

//    foreach(ClassSkills skill, skillList){
//        skillLabels << skill.getKey();
//    }

//    ui->skillListBox->clear();
//    ui->skillListBox->addItems(skillLabels);
//    ui->countSkillBox->setValue(skillLabels.size());

//}

///// !brief Returns true if ClassLehrling in this
///// year exist
//bool FormEvaluation::yearExist(int year)
//{
//    QDate today = QDate::currentDate();
//    QMapIterator<QString, ClassLehrling> it(azubiMap);
//    while (it.hasNext()) {
//        it.next();
//        int jahrgang = today.year() - it.value().apprenticeshipDate().year();
//        if( jahrgang <= 0)
//            jahrgang = 1;
//        if(jahrgang > 5)
//            jahrgang = 5;

//        if(year == jahrgang)
//            return true;
//    }

//    return false;
//}

///// !brief Sort apprentices in grades (years)
//void FormEvaluation::setupSortBox()
//{
//    QStringList labels;
//    for(int i = 1; i < 6; i++)
//    {
//        if(yearExist(i))
//        {
//            QString label = QString::number(i,10)+".Lehrjahr";
//            if(i > 4)
//                label = "Nachholer";
//            labels << label;
//        }
//    }
//    ui->azubiSortBox->clear();
//    ui->azubiSortBox->addItems(labels);
//    azubiSortBoxChanged(labels.first());
//    connect(ui->azubiSortBox, &QComboBox::currentTextChanged, this, &FormEvaluation::azubiSortBoxChanged);
//}


///// !brief Returns a list of ClassLehrling (apprentices)
///// sorted by grades (year)
//QList<ClassLehrling> FormEvaluation::getAzubiList(int year)
//{
//    QList<ClassLehrling> list;
//    QDate today = QDate::currentDate();
//    QMapIterator<QString, ClassLehrling> it(azubiMap);
//    while (it.hasNext()) {
//        it.next();
//        ClassLehrling azu = it.value();
//        int jahrgang = today.year() - azu.apprenticeshipDate().year();
//        if( jahrgang <= 0)
//            jahrgang = 1;
//        if(jahrgang > 5)
//            jahrgang = 5;

//        if(jahrgang == year)
//            list << it.value();
//    }

//    return list;
//}


























//void FormEvaluation::skillListBoxChanged(const QString &text)
//{
//    selectedSkill = currentSkillMap.value(text);
//    currentProjektMap = selectedSkill.getProjektMap();

//    QList<ClassProjekt> projektList;
//    projektList = getProjektList(selectedSkill);

//    QStringList labels;
//    foreach(ClassProjekt pro, projektList)
//        labels << pro.getKey();

//    ui->projektListBox->clear();
//    ui->projektListBox->addItems(labels);
//    ui->countProjektBox->setValue(labels.size());
//}

//void FormEvaluation::projektListBoxChanged(const QString &text)
//{
//    selectedProjekt = currentProjektMap.value(text);
//    ui->projektNameEdit->setText(selectedProjekt.name());
//    setupQuestionTable(selectedProjekt);

//    double prozent = selectedProjekt.percent();


//    if(prozent < 50)
//        setTextColor(ui->projektProzentBox, Qt::darkRed);
//    else
//        setTextColor(ui->projektProzentBox, Qt::darkGreen);

//    ui->projektProzentBox->setValue(prozent);

//}

//void FormEvaluation::fragenTableCellChanged(int , int )
//{
//    int maxPoints = selectedProjekt.maxPoints();
//    double punkte = 0.0;
//    for(int i = 0; i < ui->fragenTableWidget->rowCount(); i++){
//        punkte = punkte + ui->fragenTableWidget->item(i,2)->text().toDouble();
//        selectedProjekt.setQuestionPoints(i, ui->fragenTableWidget->item(i,2)->text().toInt());
//    }

//    double prozent = punkte * 100 / maxPoints;

//    if(prozent >= 50.0)
//        setTextColor(ui->projektProzentBox, Qt::darkGreen);
//    else
//        setTextColor(ui->projektProzentBox, Qt::darkRed);

//    ui->projektProzentBox->setValue(prozent);


//    updateAzubi( qFloor( punkte ), prozent);

//    selectedProjekt.setPoints( qFloor( punkte) );
//    selectedProjekt.setPercent(prozent);

//    currentProjektMap.insert(selectedProjekt.getKey(), selectedProjekt);
//    currentSkillMap.insert(selectedSkill.getKey(), selectedSkill);
//    selectedAzubi.setSkillMap(currentSkillMap);
//    m_azubiMap.insert(selectedAzubi.getKey(), selectedAzubi);

//    dirty = true;
//    ui->saveButton->setEnabled(true);

//    qDebug() << m_azubiMap.value("Farschad.Saberi.1").getSkillMap().values().first().getProjektMap().values().first().points();
//    qDebug() << m_azubiMap.value("Farschad.Saberi.1").getSkillMap().values().first().getProjektMap().values().first().name();

//}


//QList<ClassSkills> FormEvaluation::getSkillList(const ClassLehrling &azu)
//{
//    QList<ClassSkills> list;
//    QMap<QString, ClassSkills> sMap;
//    sMap = azu.getSkillMap();
//    list = sMap.values();
//    return list;
//}

//QList<ClassProjekt> FormEvaluation::getProjektList(const ClassSkills &skill)
//{
//    QList<ClassProjekt> list;
//    QMap<QString, ClassProjekt> pMap;
//    pMap = skill.getProjektMap();
//    list = pMap.values();
//    return list;
//}

//void FormEvaluation::setupQuestionTable(ClassProjekt pro)
//{

//    disconnect(ui->fragenTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::fragenTableCellChanged);

//    ui->fragenTableWidget->clear();
//    ui->fragenTableWidget->setColumnCount(5);
//    ui->fragenTableWidget->setRowCount(pro.questionMap().size());

//    QStringList labels;
//    labels << "Nr." << "Frage" << "Punkte" << "Max. Punkte" << "Kennung" ;
//    ui->fragenTableWidget->setHorizontalHeaderLabels(labels);

//    QMap<int, ClassFrage> fMap = pro.questionMap();

//    int row = 0;
//    QMapIterator<int, ClassFrage> it(fMap);
//    while (it.hasNext()) {
//        it.next();

//        ClassFrage question = it.value();
//        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(question.questionNr(),10) );
//        QTableWidgetItem *itemFrage = new QTableWidgetItem( question.question() );
//        QTableWidgetItem *itemPunkte = new QTableWidgetItem( QString::number(question.points(),10) );
//        QTableWidgetItem *itemMaxPunkte = new QTableWidgetItem( QString::number(question.maxPoints(),10) );
//        QTableWidgetItem *itemKennung = new QTableWidgetItem( question.identifier() );
//        ui->fragenTableWidget->setItem(row,0, itemNr);
//        ui->fragenTableWidget->setItem(row,1, itemFrage);
//        ui->fragenTableWidget->setItem(row,2, itemPunkte);
//        ui->fragenTableWidget->setItem(row,3, itemMaxPunkte);
//        ui->fragenTableWidget->setItem(row,4, itemKennung);

//        itemNr->setFlags(Qt::ItemIsEnabled);
//        itemFrage->setFlags(Qt::ItemIsEnabled);
//        itemMaxPunkte->setFlags(Qt::ItemIsEnabled);
//        itemKennung->setFlags(Qt::ItemIsEnabled);
//        itemPunkte->setTextColor(Qt::blue);

//        row++;
//    }

//    ui->fragenTableWidget->resizeColumnToContents(0);
//    ui->fragenTableWidget->resizeColumnToContents(1);
//    ui->fragenTableWidget->resizeColumnToContents(2);
//    ui->fragenTableWidget->resizeColumnToContents(3);
//    ui->fragenTableWidget->resizeColumnToContents(4);

//    connect(ui->fragenTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::fragenTableCellChanged);

//}

//void FormEvaluation::updateAzubi(int points, double percent)
//{
//    selectedProjekt.setPoints(points);
//    selectedProjekt.setPercent(percent);

//    QString pkey = selectedProjekt.getKey();
//    currentProjektMap.insert(pkey, selectedProjekt);


//}

//void FormEvaluation::azubiSortBoxChanged(const QString &text)
//{
//    ui->azubiListBox->clear();
//    QStringList list;

//    if(text == "Nachfolger")
//        list = getAzubiKeys(5);

//    if(text != "Nachfolger")
//    {
//        if(text == "1.Lehrjahr")
//            list = getAzubiKeys(1);

//        if(text == "2.Lehrjahr")
//            list = getAzubiKeys(2);

//        if(text == "3.Lehrjahr")
//            list = getAzubiKeys(3);

//        if(text == "4.Lehrjahr")
//            list = getAzubiKeys(4);
//    }

//    ui->azubiListBox->addItems(list);

//}

//void FormEvaluation::azubiListBoxChanged(const QString &text)
//{
//    ui->skillListBox->clear();
//    ui->projektListBox->clear();
//    ui->resultTableWidget->clear();

//    selectedAzubi = m_azubiMap.value(text);

//    ui->azuNameEdit->setText(selectedAzubi.firstname()+"."+selectedAzubi.surname());
//    ui->nrBox->setValue(selectedAzubi.nr());

//    QMap<QString, ClassSkills> sMap;
//    sMap = selectedAzubi.getSkillMap();
//    ui->skillListBox->addItems(sMap.keys());

//    if(selectedAzubi.getSkillMap().isEmpty())
//        setTextColor(ui->azuNameEdit, Qt::darkRed);
//    else
//        setTextColor(ui->azuNameEdit, Qt::black);
//}

//void FormEvaluation::skillListBoxChanged(const QString &text)
//{
//    ui->projektListBox->clear();

//    QMap<QString, ClassSkills> sMap;
//    sMap = selectedAzubi.getSkillMap();
//    selectedSkill = sMap.value(text);

//    QStringList pList;
//    pList = selectedSkill.getProjektMap().keys();

//    ui->projektListBox->addItems(pList);
//    setupErgebnisTable(selectedAzubi);
//}

//void FormEvaluation::projektListBoxChanged(const QString &text)
//{
//    QMap<QString, ClassProjekt> pMap;
//    pMap = selectedSkill.getProjektMap();

//    selectedProjekt = pMap.value(text);
//    ui->projektNameEdit->setText(selectedProjekt.name());
//    setupQuestionTable(selectedProjekt);
//}

//void FormEvaluation::cellItemClicked(int row, int column)
//{

//    int maxPoints = ui->fragenTableWidget->item(row,3)->text().toInt();
//    int punkte = ui->fragenTableWidget->item(row,2)->text().toInt();

//    if(punkte > maxPoints || punkte < 0){
//        QMessageBox::information(this, tr("Eingabe Punkte"), tr("Die Eingabe von Punkten kleiner als 0 oder größer"
//                                                                " als die max. Punkte ist nicht erlaubt!"));
//        return;
//    }

//    int proPunkte = 0;
//    for(int i = 0; i < ui->fragenTableWidget->rowCount(); i++)
//        proPunkte = proPunkte + ui->fragenTableWidget->item(i, 2)->text().toInt();

//    selectedProjekt.setPoints(proPunkte);
//    cellProMap.insert(selectedProjekt.nr(), selectedProjekt);

//    // update resultTable
//    if(!cellProMap.isEmpty()){

//        // Projekt
//        int projektrow = getProjektRow(selectedProjekt);
//        ui->resultTableWidget->item(projektrow , 3)->setText(QString::number(proPunkte,10));
//        double maxPoints = ui->resultTableWidget->item(projektrow ,2)->text().toDouble();
//        double prozent =  proPunkte * 100.0 / maxPoints ;
//        if(prozent < 50.0)
//            ui->resultTableWidget->item(projektrow ,4)->setTextColor(Qt::darkRed);
//        else
//            ui->resultTableWidget->item(projektrow ,4)->setTextColor(Qt::darkGreen);

//        QString result = QString::number(prozent, 'g', 3);
//        ui->resultTableWidget->item(projektrow ,4)->setText( result );

//        // Skill
//        int skillrow = getSkillRow(selectedSkill);
//        int maxSkillPunkte = ui->resultTableWidget->item(skillrow,2)->text().toInt();
//        int skillpoints = getCurrentPoints();
//        QString sp = QString::number(skillpoints,10);
//        ui->resultTableWidget->item(skillrow,3)->setText(sp);


//        qDebug() << skillrow;
//        qDebug() << maxSkillPunkte;
//        qDebug() << skillpoints;

//    }


//}

//void FormEvaluation::cellItemChanged(int row, int column)
//{
//    if(column != 3)
//        return;

//    // Calculate

//    qDebug() << "Update result table";
//}

//void FormEvaluation::sortAzubi(const QMap<QString, ClassLehrling> &aMap)
//{
//    ui->azubiSortBox->clear();
//    QDate today = QDate::currentDate();

//    QMapIterator<QString, ClassLehrling> it(aMap);
//    while (it.hasNext()) {
//        it.next();
//        ClassLehrling azu = it.value();
//        int year = today.year() - azu.apprenticeshipDate().year();
//        if( year <= 0)
//            year = 1;
//        if(year > 5)
//            year = 5;

//        jahrgangMap.insert(year, azu);
//    }

//    QStringList list;
//    for (int i = 1; i < jahrgangMap.values().size(); i++) {
//        if(jahrgangMap.values(i).size() > 0){
//            if(i < 5){
//                QString s = QString::number(i,10)+".Lehrjahr";
//                list << s;
//            }
//            if(i == 5){
//                list << "Nachfolger";
//            }
//        }
//    }

//    ui->azubiSortBox->addItems(list);
//    azubiSortBoxChanged(list.first());

//    connect(ui->azubiSortBox, &QComboBox::currentTextChanged, this, &FormEvaluation::azubiSortBoxChanged);
//}

//QStringList FormEvaluation::getAzubiKeys(int jahrgang)
//{
//    QStringList keyList;
//    QList<ClassLehrling> azuList;
//    azuList = jahrgangMap.values(jahrgang);
//    foreach(ClassLehrling azu, azuList)
//        keyList << azu.getKey();

//    return keyList;
//}

//void FormEvaluation::setupQuestionTable(const ClassProjekt &pro)
//{

//    disconnect(ui->fragenTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::cellItemClicked);
//    //disconnect(ui->resultTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::cellItemChanged);

//    ui->fragenTableWidget->clear();
//    ui->fragenTableWidget->setColumnCount(5);
//    ui->fragenTableWidget->setRowCount(pro.questionMap().size());

//    QStringList labels;
//    labels << "Nr." << "Frage" << "Punkte" << "Max. Punkte" << "Kennung" ;
//    ui->fragenTableWidget->setHorizontalHeaderLabels(labels);

//    QMap<int, ClassFrage> fMap = pro.questionMap();

//    int row = 0;
//    QMapIterator<int, ClassFrage> it(fMap);
//    while (it.hasNext()) {
//        it.next();

//        ClassFrage question = it.value();
//        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(question.questionNr(),10) );
//        QTableWidgetItem *itemFrage = new QTableWidgetItem( question.question() );
//        QTableWidgetItem *itemPunkte = new QTableWidgetItem( QString::number(question.points(),10) );
//        QTableWidgetItem *itemMaxPunkte = new QTableWidgetItem( QString::number(question.maxPoints(),10) );
//        QTableWidgetItem *itemKennung = new QTableWidgetItem( question.identifier() );
//        ui->fragenTableWidget->setItem(row,0, itemNr);
//        ui->fragenTableWidget->setItem(row,1, itemFrage);
//        ui->fragenTableWidget->setItem(row,2, itemPunkte);
//        ui->fragenTableWidget->setItem(row,3, itemMaxPunkte);
//        ui->fragenTableWidget->setItem(row,4, itemKennung);

//        itemNr->setFlags(Qt::ItemIsEnabled);
//        itemFrage->setFlags(Qt::ItemIsEnabled);
//        itemMaxPunkte->setFlags(Qt::ItemIsEnabled);
//        itemKennung->setFlags(Qt::ItemIsEnabled);
//        itemPunkte->setTextColor(Qt::blue);

//        row++;
//    }

//    ui->fragenTableWidget->resizeColumnToContents(0);
//    ui->fragenTableWidget->resizeColumnToContents(1);
//    ui->fragenTableWidget->resizeColumnToContents(2);
//    ui->fragenTableWidget->resizeColumnToContents(3);
//    ui->fragenTableWidget->resizeColumnToContents(4);

//    connect(ui->fragenTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::cellItemClicked);
//    //connect(ui->resultTableWidget, &QTableWidget::cellChanged, this, &FormEvaluation::cellItemChanged);
//}

//void FormEvaluation::setupErgebnisTable(const ClassLehrling &azu)
//{
//    cellProMap.clear();
//    cellSkillMap.clear();

//    ui->resultTableWidget->clear();
//    ui->resultTableWidget->setColumnCount(6);

//    QMap<int, ClassSkills> skillMap;

//    int rows = azu.getSkillMap().values().size();
//    foreach(ClassSkills skill, azu.getSkillMap().values()){
//        rows = rows + skill.getProjektMap().keys().size();
//        skillMap.insert(skill.getNr(), skill);
//    }

//    QStringList labels;
//    labels << "Nr." << "Beschreibung" << "Max.Punkte" << "Punkte" << "%" << "Note";


//    ui->resultTableWidget->setRowCount(rows);
//    ui->resultTableWidget->setColumnCount(5);
//    ui->resultTableWidget->setHorizontalHeaderLabels(labels);


//    int row = 0;
//    QMapIterator<int, ClassSkills> it(skillMap);
//    while (it.hasNext()) {
//        it.next();
//        ClassSkills skill = it.value();

//        QTableWidgetItem *itemSkillName = new QTableWidgetItem (skill.getKey());
//        QTableWidgetItem *itemSkillNr = new QTableWidgetItem ( QString::number( it.key(),10));
//        QTableWidgetItem *itemSkillMaxPunkte = new QTableWidgetItem ( QString::number( skill.maxPoints(),10) );
//        QTableWidgetItem *itemSkillPunkte = new QTableWidgetItem ( QString::number( skill.points(),10) );
//        QTableWidgetItem *itemSkillProzent = new QTableWidgetItem ( "0");

//        ui->resultTableWidget->setItem(row,0, itemSkillNr);
//        ui->resultTableWidget->setItem(row,1, itemSkillName);
//        ui->resultTableWidget->setItem(row,2, itemSkillMaxPunkte);
//        ui->resultTableWidget->setItem(row,3, itemSkillPunkte);
//        ui->resultTableWidget->setItem(row,4, itemSkillProzent);

//        itemSkillNr->setTextColor(QColor(170,25,255));
//        itemSkillName->setTextColor(QColor(170,25,255));
//        itemSkillMaxPunkte->setTextColor(QColor(170,25,255));
//        itemSkillPunkte->setTextColor(QColor(170,25,255));

//        itemSkillNr->setFlags(Qt::ItemIsEnabled);
//        itemSkillName->setFlags(Qt::ItemIsEnabled);
//        itemSkillMaxPunkte->setFlags(Qt::ItemIsEnabled);
//        itemSkillPunkte->setFlags(Qt::ItemIsEnabled);

//        cellSkillMap.insert(row, skill);

//        row++;

//        QMap<int, ClassProjekt> proMap;
//        QList<ClassProjekt> projektList;
//        projektList = skill.getProjektMap().values();
//        foreach(ClassProjekt pro, projektList)
//            proMap.insert(pro.nr(), pro);

//        QMapIterator<int, ClassProjekt> ip(proMap);
//        while (ip.hasNext()) {
//            ip.next();
//            ClassProjekt pro = ip.value();

//            QTableWidgetItem *itemProNr = new QTableWidgetItem ( QString::number(pro.nr(),10));
//            QTableWidgetItem *itemProName = new QTableWidgetItem ( pro.name() );
//            QTableWidgetItem *itemProMaxPunkte = new QTableWidgetItem ( QString::number( pro.maxPoints(),10) );
//            QTableWidgetItem *itemProPunkte = new QTableWidgetItem ( QString::number( pro.points(),10) );
//            QTableWidgetItem *itemProzent = new QTableWidgetItem ( "0" );

//            ui->resultTableWidget->setItem(row,0, itemProNr);
//            ui->resultTableWidget->setItem(row,1, itemProName);
//            ui->resultTableWidget->setItem(row,2, itemProMaxPunkte);
//            ui->resultTableWidget->setItem(row,3, itemProPunkte);
//            ui->resultTableWidget->setItem(row,4, itemProzent);

//            itemProNr->setTextColor(QColor(0,85,127));
//            itemProName->setTextColor(QColor(0,85,127));
//            itemProMaxPunkte->setTextColor(QColor(0,85,127));
//            itemProPunkte->setTextColor(QColor(0,85,127));
//            itemProzent->setTextColor(QColor(0,85,127));


//            cellProMap.insert(row, pro);

//            row++;
//        }

//    }

//    ui->resultTableWidget->resizeColumnToContents(0);
//    ui->resultTableWidget->resizeColumnToContents(1);
//    ui->resultTableWidget->resizeColumnToContents(2);
//    ui->resultTableWidget->resizeColumnToContents(3);
//    ui->resultTableWidget->resizeColumnToContents(4);

//}


//int FormEvaluation::getSkillRow(ClassSkills skill)
//{
//    int row = -1;
//    QMapIterator<int, ClassSkills> it(cellSkillMap);
//    while (it.hasNext()) {
//        it.next();
//        ClassSkills s = it.value();
//        if(s.getKey() == skill.getKey()){
//            row = it.key();
//            break;
//        }
//    }

//    return row;
//}

//int FormEvaluation::getProjektRow( ClassProjekt pro )
//{
//    int row = -1;
//    QMapIterator<int, ClassProjekt> it(cellProMap);
//    while (it.hasNext()) {
//        it.next();
//        ClassProjekt p = it.value();
//        if(p.getKey() == pro.getKey()){
//            row = it.key();
//            break;
//        }
//    }

//    return row;
//}

//int FormEvaluation::getCurrentPoints()
//{
//    int points = 0;

//    if(cellProMap.isEmpty())
//        return points;

//    QMapIterator<int, ClassProjekt> it(cellProMap);
//    while (it.hasNext()) {
//        it.next();
//        points = points + it.value().points();
//    }

//    return points;
//}

//QStringList ItemList::labels() const
//{
//    return m_labels;
//}

//void ItemList::setLabels(const QStringList &labels)
//{
//    m_labels = labels;
//}

//QList<int> ItemList::redList() const
//{
//    return m_redList;
//}

//void ItemList::setRedList(const QList<int> &redList)
//{
//    m_redList = redList;
//}
