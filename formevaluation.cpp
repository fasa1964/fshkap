#include "formevaluation.h"
#include "ui_formevaluation.h"

#include <QDate>
#include <QDateTime>
#include <QPalette>
#include <QMessageBox>
#include <QtMath>
#include <QTreeWidgetItem>

#include <fshkwindow.h>
#include <QDebug>

FormEvaluation::FormEvaluation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEvaluation)
{
    ui->setupUi(this);

    dirty = false;
    selectedLehrling = ClassLehrling();
    selectedSkill = ClassSkills();
    selectedProjekt = ClassProjekt();

    connect(ui->closeButton, &QPushButton::clicked, this, &FormEvaluation::closeButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &FormEvaluation::saveButtonClicked);
    connect(ui->evaluatedCheckBox, &QCheckBox::stateChanged, this, &FormEvaluation::evaluatedCheckBoxChanged);
    connect(ui->azubiListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::azubiBoxTextChanged);
    connect(ui->skillListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::skillBoxTextChanged);
    connect(ui->projektListBox, &QComboBox::currentTextChanged, this, &FormEvaluation::projectBoxTextChanged);
    connect(ui->resultTreeWidget, &QTreeWidget::itemClicked, this, &FormEvaluation::resultTableItemClicked);

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
/// Get all apprentices from year
void FormEvaluation::sortBoxTextChanged(const QString &text)
{
    ui->azubiListBox->clear();
    int year = QString("%1").arg(text[0],1).toInt();
    QMap<QString, ClassLehrling> sortedMap;
    sortedMap = apprenticeship(year);

    ui->azubiListBox->addItems(sortedMap.keys());
    ui->countAzubiBox->setValue(sortedMap.values().size());
}

/// !brief When the name of apprentices changed
/// Update the skill and result table
void FormEvaluation::azubiBoxTextChanged(const QString &text)
{
    selectedLehrling = m_azubiMap.value(text);
    updateSkillBox(selectedLehrling);
    setupResultWidget(selectedLehrling);
}

void FormEvaluation::skillBoxTextChanged(const QString &text)
{
    selectedSkill = selectedLehrling.getSkillMap().value(text);
    ui->projektListBox->clear();
    ui->projektListBox->addItems(selectedSkill.getProjektMap().keys());
    ui->countProjektBox->setValue(selectedSkill.getProjektMap().values().size());
}

void FormEvaluation::projectBoxTextChanged(const QString &text)
{
    QMap<QString, ClassProjekt> pMap;
    pMap = selectedSkill.getProjektMap();
    selectedProjekt = pMap.value(text);

    ui->projektNameEdit->setText(selectedProjekt.name());
    ui->evaluatedCheckBox->setChecked(selectedProjekt.getEvaluated());

    double percent = selectedProjekt.percent();
    if(percent < 50)
        setTextColor(ui->percentBox, Qt::red);
    else
        setTextColor(ui->percentBox, Qt::darkGreen);
    ui->percentBox->setValue(percent);

    setupQuestionTable(selectedProjekt);

    qDebug() << selectedProjekt.createTime();
}

void FormEvaluation::evaluatedCheckBoxChanged(int status)
{

    if(status == Qt::Checked && selectedProjekt.getEvaluated() != true){
        selectedProjekt.setEvaluated(true);
        setupProjectValue();
    }

    if(status == Qt::Unchecked  && selectedProjekt.getEvaluated() == true){
        selectedProjekt.setEvaluated(false);
        setupProjectValue();
    }

}

/// !brief Calculate the points of current project
void FormEvaluation::questionTableCellChanged(int row, int column)
{
    // Check if already evaluated
    if(ui->evaluatedCheckBox->isChecked()){
        QMessageBox::information(this, tr("Eingabe"), tr("Das Projekt wurde bereits ausgewertet!\n"
                                           "Sollte dennoch Änderungen gemacht werden,\n"
                                           "deaktivieren sie bitte Ausgewertet!"));
        setupQuestionTable(selectedProjekt);

        return;
    }

    // Check entered value
    QString s = ui->fragenTableWidget->item(row, column)->text();
    foreach (QChar ch, s) {
        if(!ch.isDigit()){
            QMessageBox::information(this, tr("Eingabe"), tr("Bitte nur Zahlen eingeben!"));
            ui->fragenTableWidget->item(row, column)->setText("0");
        }
    }

    int value =  s.toInt();
    int mval = ui->fragenTableWidget->item(row, column+1)->text().toInt();
    if(value > mval){
        QMessageBox::information(this, tr("Eingabe"), tr("Der Zahlenwert darf das Maximum nicht überschreiten!"));
        ui->fragenTableWidget->item(row, column)->setText("0");
    }

    // Udate selected project
    QMap<int, ClassFrage> fMap;
    fMap = selectedProjekt.questionMap();
    for(int i = 0; i < ui->fragenTableWidget->rowCount(); i++){
        int p = ui->fragenTableWidget->item(i, 2)->text().toInt();
        ClassFrage frg = fMap.value(i);
        frg.setPoints(p);
        fMap.insert(i, frg);
    }
    selectedProjekt.setQuestionMap(fMap);

    double pp = getProjectPercent(selectedProjekt);
    selectedProjekt.setPoints(getProjectPoints(selectedProjekt));
    selectedProjekt.setPercent(pp);

    ui->percentBox->setValue(pp);
    if(pp < 50.0)
        setTextColor(ui->percentBox, Qt::red);
    else
        setTextColor(ui->percentBox, Qt::darkGreen);

    selectedProjekt.setPoints(getProjectPoints(selectedProjekt));
    selectedProjekt.setPercent(pp);
    setupProjectValue();

//    // Update selected skill and selected Lehrling
//    selectedSkill.insertProjekt(selectedProjekt);
//    selectedLehrling.insertSkill(selectedSkill);
//    m_azubiMap.insert(selectedLehrling.getKey(), selectedLehrling);
//    setupResultWidget(selectedLehrling);

//    dirty = true;
//    ui->saveButton->setEnabled(true);



//    qDebug() << "Max:" << getProjectMaxPoint(selectedProjekt);
//    qDebug() << "Points:" << getProjectPoints(selectedProjekt);
//    qDebug() << "Percent" << getProjectPercent(selectedProjekt);



//    // Calculate project
//    int points = 0;

//    QMap<int, ClassFrage> fMap;
//    fMap = selectedProjekt.questionMap();
//    for(int i = 0; i < ui->fragenTableWidget->rowCount(); i++){
//        int p = ui->fragenTableWidget->item(i, 2)->text().toInt();
//        ClassFrage frg = fMap.value(i);
//        frg.setPoints(p);
//        fMap.insert(i, frg);
//        points = points + p;
//    }

//    int maxPoints = selectedProjekt.maxPoints();
//    double percent = points * 100.0 / maxPoints;
//    if(percent < 50.0)
//        setTextColor(ui->percentBox, Qt::red);
//    else
//        setTextColor(ui->percentBox, Qt::darkGreen);

//    ui->percentBox->setValue(percent);


//    selectedProjekt.setPercent(percent);
//    selectedProjekt.setPoints(points);
//    selectedProjekt.setQuestionMap(fMap);
    //    setupProjectValue();
}

void FormEvaluation::resultTableItemClicked(QTreeWidgetItem *item, int column)
{
    QString key = item->text(column);
    ui->projektListBox->setCurrentText(key);
    //projectBoxTextChanged(key);
}

void FormEvaluation::setupProjectValue()
{
    // Update selected skill and selected Lehrling
    selectedSkill.insertProjekt(selectedProjekt);
    selectedLehrling.insertSkill(selectedSkill);
    m_azubiMap.insert(selectedLehrling.getKey(), selectedLehrling);

    setupResultWidget(selectedLehrling);

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

void FormEvaluation::closeButtonClicked()
{
    if(dirty){
        int result = QMessageBox::information(this,"Auswertung schließen", "Die Auswertungen haben sich geändert.\n"
                                 "Wenn sie jetzt schließen gehen die eingegebenen Daten verloren!",
                                  QMessageBox::Save | QMessageBox::Cancel);
         if(result == QMessageBox::Save)
            emit saveAzubiMap(m_azubiMap);
    }

    dirty = false;
    ui->saveButton->setEnabled(false);
    close();
}

void FormEvaluation::saveButtonClicked()
{
    emit saveAzubiMap(m_azubiMap);
    ui->saveButton->setEnabled(false);
    dirty = false;
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

void FormEvaluation::setupResultWidget(const ClassLehrling &azu)
{
    ui->resultTreeWidget->clear();

    QMap<QString, ClassSkills> sMap;
    sMap = azu.getSkillMap();

    ui->resultTreeWidget->setColumnCount(3);

    QStringList headers;
    headers << "Beschreibung" << "Ergebnis in %" << "Wert/Faktor";
    ui->resultTreeWidget->setHeaderLabels(headers);

    QMapIterator<QString, ClassSkills> it(sMap);
    while (it.hasNext()) {
        it.next();

        double skillPercent = 0.0;

        ClassSkills skill = it.value();
        bool projectNode = false;
        if( skill.index(skill.criteria()) == 0 )
            projectNode = true;

        QTreeWidgetItem *topItem = new QTreeWidgetItem(QStringList() << it.key());
        ui->resultTreeWidget->addTopLevelItem(topItem);
        ui->resultTreeWidget->expandItem(topItem);

        QFont font = topItem->font(0);
        font.setBold(true);
        topItem->setFont(0,font);
        topItem->setFont(1,font);

        // Set child item
        QMap<QString, ClassProjekt> pMap;
        pMap = skill.getProjektMap();

        QMapIterator<QString, ClassProjekt> ip(pMap);
        while (ip.hasNext()) {
             ip.next();
             ClassProjekt pro = ip.value();

             QTreeWidgetItem *childItem = new QTreeWidgetItem(QStringList() << ip.key());
             topItem->addChild(childItem);

             // if projekt has
             if(!pro.identifierList().isEmpty() && skill.criteria() == ClassSkills::identifierNode){
                QStringList list = pro.identifierList();
                foreach (QString ids, list) {
                    QTreeWidgetItem *cc = new QTreeWidgetItem(QStringList() << ids);
                    childItem->addChild(cc);

                    double identPercent =  getResultIdentifier(  getQuestions(pro, ids) );
                    cc->setText(1, QString::number(identPercent, 'g', 2));
                }

             }

             // Set percent each project
             if(pro.percent() < 50.0){
                 childItem->setTextColor(0, Qt::red);
                 childItem->setTextColor(1, Qt::red);
             }else{
                 childItem->setTextColor(0, Qt::darkGreen);
                 childItem->setTextColor(1, Qt::darkGreen);
             }

             childItem->setText(1, QString::number(pro.percent(), 'g', 2)+" %");
             childItem->setText(2, QString::number(pro.getFactor(), 'g', 2));

             // calculate the skill percent
             if(projectNode)
             {
                 double proPercent = pro.percent() * pro.getFactor();
                 skillPercent = skillPercent + proPercent;
             }

        }

        // Set skill percent text
        topItem->setText(1, QString::number( skillPercent,'g',2)+"%");

        if(sMap.size() == 1){
            QString text = "100%("+QString::number( skill.getWert(),10)+"%)";
            topItem->setText(2, text);
        }else
            topItem->setText(2, QString::number( skill.getWert(),10)+"%");

        if(skillPercent < 50.0)
            topItem->setTextColor(1,Qt::red);
        else
            topItem->setTextColor(1,Qt::darkGreen);


    }

    ui->resultTreeWidget->resizeColumnToContents(0);


//    double totalPercent = 0.0;
//    int index = 0;
//    QMapIterator<QString, ClassSkills> it(sMap);
//    while (it.hasNext()) {
//        it.next();

//        ClassSkills skill = it.value();

//        QTreeWidgetItem *topItem = new QTreeWidgetItem(QStringList() << it.key());

//        ui->resultTreeWidget->addTopLevelItem(topItem);
//        ui->resultTreeWidget->expandItem(topItem);

//        double percent = skill.points() * 100.0 / skill.maxPoints();
//        topItem->setText(1, QString::number(percent, 'g',3));

//        QString wert = QString::number( skill.getWert(), 10 )+"%";
//        if(sMap.size() == 1)
//            wert.append("(100%)");

//        topItem->setText(2, wert);


//        QFont font = topItem->font(0);
//        font.setBold(true);
//        topItem->setFont(0,font);
//        topItem->setFont(1,font);


//        double tp = 0.0;
//        if(sMap.size() == 1)
//            tp = percent * (100/100.0);
//        else
//            tp = percent * (skill.getWert()/100.0);


//        totalPercent = totalPercent + tp;

//        // Set all projects as child items
//        QMap<QString, ClassProjekt> pMap;
//        pMap = skill.getProjektMap();

//        QMapIterator<QString, ClassProjekt> ip(pMap);
//        while (ip.hasNext()) {
//            ip.next();
//            ClassProjekt pro = ip.value();

//            QTreeWidgetItem *childItem = new QTreeWidgetItem(QStringList() << ip.key());
//            topItem->addChild(childItem);

//            QString pers = QString::number(pro.percent(),'g' , 3);
//            QString fact = QString::number(pro.getFactor(),'g' , 3);
//            childItem->setText(1, pers);
//            childItem->setText(2, fact);

//            if(pro.percent() < 50.0){
//                childItem->setTextColor(0, Qt::red);
//                childItem->setTextColor(1, Qt::red);
//            }else{

//                childItem->setTextColor(0, Qt::black);
//                childItem->setTextColor(1, Qt::darkGreen);
//            }
//        }

//        index++;

//    }

//    // Total result from exame
//    QTreeWidgetItem *topItem = new QTreeWidgetItem(QStringList() << "Prüfungsergebnis:");
//    ui->resultTreeWidget->addTopLevelItem(topItem);
//    ui->resultTreeWidget->expandItem(topItem);
//    QFont font = topItem->font(0);
//    font.setBold(true);
//    topItem->setFont(0,font);
//    topItem->setFont(1,font);

//    if(totalPercent < 50.0)
//        topItem->setTextColor(1, Qt::red);
//    else
//        topItem->setTextColor(1, Qt::darkGreen);



//    topItem->setText(1, QString::number(totalPercent, 'g',3));


//    ui->resultTreeWidget->resizeColumnToContents(0);
//    ui->resultTreeWidget->resizeColumnToContents(1);

//    setupIdentifier(azu);
}

void FormEvaluation::setupIdentifier(const ClassLehrling &azu)
{

    QMultiMap<QString, ClassFrage> identifierMap;

    QMap<QString, ClassSkills> sMap;
    sMap = azu.getSkillMap();

    QMapIterator<QString, ClassSkills> it(sMap);
    while (it.hasNext()) {
        it.next();

        ClassSkills skill = it.value();

        // Set all projects as child items
        QMap<QString, ClassProjekt> pMap;
        pMap = skill.getProjektMap();

        QMapIterator<QString, ClassProjekt> ip(pMap);
        while (ip.hasNext()) {
            ip.next();
            ClassProjekt pro = ip.value();

            QMap<int, ClassFrage> iMap;
            iMap = pro.questionMap();
            QMapIterator<int, ClassFrage> iq(iMap);
            while (iq.hasNext()) {
                iq.next();
                ClassFrage frg = iq.value();
                if(!frg.identifier().isEmpty())
                    identifierMap.insert(frg.identifier(),frg);

            }

        }
    }

    if(identifierMap.isEmpty())
        return;

    QStringList keys = identifierMap.keys();
    QStringList headers;   

    foreach (QString k , keys) {

        // Create top item
        if(!headers.contains(k)){
            headers << k;
            QTreeWidgetItem *topItem = new QTreeWidgetItem(QStringList() << k);
            ui->resultTreeWidget->addTopLevelItem(topItem);

            double percent = getResultIdentifier(identifierMap.values(k));
            topItem->setText(1, QString::number(percent, 'g', 3));
        }
    }
}

double FormEvaluation::getResultIdentifier(const QList<ClassFrage> questList)
{
    double percent = 0.0;
    int points = 0;
    int maxpoints = 0;
    foreach (ClassFrage frg, questList) {
        maxpoints = maxpoints + frg.maxPoints();
        points = points + frg.points();
    }

    percent = points * 100.0 / maxpoints;

    return percent;
}

QStringList FormEvaluation::questionsIdentifierList(const ClassLehrling &azu)
{
    QStringList identList;
    QMap<QString, ClassSkills> sMap;
    sMap = azu.getSkillMap();
    QMapIterator<QString, ClassSkills> it(sMap);
    while (it.hasNext()) {
        it.next();
        ClassSkills skill = it.value();
        QMap<QString, ClassProjekt> pMap;
        pMap = skill.getProjektMap();
        QMapIterator<QString, ClassProjekt> ip(pMap);
        while (ip.hasNext()) {
            ip.next();
            ClassProjekt pro = ip.value();
            foreach (ClassFrage frg, pro.questionMap().values()) {
                if(!identList.contains(frg.identifier()))
                    identList << frg.identifier();
            }
        }
    }
    return identList;
}

QList<ClassFrage> FormEvaluation::getQuestions(const ClassProjekt &pro, const QString &key)
{
    QList<ClassFrage> qList;
    QMapIterator<int, ClassFrage> it(pro.questionMap());
    while (it.hasNext()) {
        it.next();
        if(it.value().identifier() == key)
            qList << it.value();

    }
    return qList;
}

double FormEvaluation::getProjectPercent(const ClassProjekt &pro)
{
    double percent = 0.0;
    int maxPoint = getProjectMaxPoint(pro);
    int points = getProjectPoints(pro);

    percent = points * 100.0 / maxPoint;

    return percent;
}

int FormEvaluation::getProjectMaxPoint(const ClassProjekt &pro)
{
    int mp = 0;
    foreach (ClassFrage frg, pro.questionMap().values()) {
        mp = mp + frg.maxPoints();
    }
    return mp;
}

int FormEvaluation::getProjectPoints(const ClassProjekt &pro)
{
    int p = 0;
    foreach (ClassFrage frg, pro.questionMap().values()) {
        p = p + frg.points();
    }
    return p;
}


























































