#include "formskills.h"
#include "ui_formskills.h"

#include <QMessageBox>
#include <QDebug>

FormSkills::FormSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSkills)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    changeSkill = false;
    createSkill = false;
    projectFactorChanged = false;

    selectedSkill = ClassSkills();
//    selectedProjekt = ClassProjekt();
//    selectedSkillProjekt = ClassProjekt();

    ui->criteriaBox->addItems(ClassSkills::supportedCriteria());
    setFormReadOnly(true);

    connect(ui->closeButton, &QPushButton::clicked, this, &FormSkills::close);
    connect(ui->deleteSkillProjektButton, &QPushButton::clicked, this, &FormSkills::deleteSkillProjektButtonClicked);
    connect(ui->createButton, &QPushButton::clicked, this, &FormSkills::createButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &FormSkills::deleteButtonClicked);
    connect(ui->changeButton, &QPushButton::clicked, this, &FormSkills::changeButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &FormSkills::saveButtonClicked);
    connect(ui->importProjekteButton, &QPushButton::clicked, this, &FormSkills::importProjekteButtonClicked);

    connect(ui->sortKennunBox, &QComboBox::currentTextChanged, this, &FormSkills::sortKennungBoxTextChanged);
    connect(ui->kennungBox, &QComboBox::currentTextChanged, this, &FormSkills::kennungBoxTextChanged);

    connect(ui->projektTable, &QTableWidget::itemClicked, this, &FormSkills::projektTableItemClicked);

    connect(ui->skillTable, &QTableWidget::itemClicked, this, &FormSkills::skillTableItemClicked);

    connect(ui->skillProjektTable, &QTableWidget::itemClicked, this, &FormSkills::skillProjektTableItemClicked);
    connect(ui->skillProjektTable, &QTableWidget::cellChanged, this, &FormSkills::skillProjektTableCellClicked);


}

FormSkills::~FormSkills()
{
    delete ui;
}

/// !brief Delete a projekt form skill
void FormSkills::deleteSkillProjektButtonClicked()
{
    int size = ui->skillProjektTable->rowCount();
    int column = ui->skillProjektTable->columnCount()-1;
    QStringList keyList;
    for(int i = 0; i < size; i++)
    {
        if(ui->skillProjektTable->item(i, column)->checkState() == Qt::Checked)
        {
            QString key = ui->skillProjektTable->item(i, 1)->text() + "." + ui->skillProjektTable->item(i, 2)->text();
            keyList << key;
        }
    }


    QMap<QString, ClassProjekt> pMap;
    pMap = selectedSkill.getProjektMap();

    foreach(QString k, keyList)
        pMap.remove(k);

    selectedSkill.setProjektMap(pMap);
    setSkillProjektToForm(selectedSkill.getProjektMap());

}

void FormSkills::createButtonClicked()
{
    clearForm();
    setFormReadOnly(false);
    setFormTextColor(QColor(0,87,127));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->nrBox->setFocus();
    ui->countProjekteBox->setValue(0);
    ui->tabWidget->setCurrentIndex(1);

    createSkill = true;
    changeSkill = false;

    ui->changeButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->createButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
}

void FormSkills::deleteButtonClicked()
{
    int result = QMessageBox::information(this, tr("Prüfung löschen"), tr("Die Prüfung:\n")+selectedSkill.name()+"-"+selectedSkill.identifier()+
                                       tr("\nwird unwiderruflich gelöscht!"), QMessageBox::Ok | QMessageBox::Cancel);
    if(result == QMessageBox::Cancel)
        return;


    if(m_skillMap.remove(selectedSkill.getKey()) == 1)
    {
        emit saveSkillsMap(m_skillMap);
        setupSkillTable(m_skillMap);
        selectedSkill = ClassSkills();
        clearForm();
        setFormReadOnly(true);

        ui->changeButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
        ui->createButton->setEnabled(true);
        ui->saveButton->setEnabled(false);
    }
    else
    {
        QMessageBox::information(this, tr("Prüfung löschen"), tr("Die Prüfung:\n")+selectedSkill.name()+"-"+selectedSkill.identifier()+
                                              tr("\nkonnte nicht gelöscht werden!"), QMessageBox::Ok | QMessageBox::Cancel);

    }
}

void FormSkills::changeButtonClicked()
{
    setFormReadOnly(false);
    setFormTextColor(QColor(0,87,127));
    ui->nrBox->setFocus();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    changeSkill = true;
    createSkill = false;

    ui->changeButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->createButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
    ui->deleteSkillProjektButton->setEnabled(false);

    ui->kennungBox->setCurrentText( ui->kennungEdit->text() );
}

void FormSkills::saveButtonClicked()
{
    if(ui->nrBox->value() <= 0 || ui->nameEdit->text().isEmpty() || ui->kennungEdit->text().isEmpty() ||
            ui->dateEdit->date() == QDate() || ui->wertBox->value() <= 0)
    {
        QMessageBox::information(this, tr("Prüfung speichern"), tr("Prüfungs-Nr., Name, Kennung und Datum sowie Wert "
                                           "sind erforderlich zum Speichern!"));
        return;
    }

    if(projectFactorChanged){
        double factor = getSkillProjectFactor();
        if(factor > 1.0 || factor < 1.0){
            QMessageBox::information(this, tr("Eingabe"), tr("Die Faktoren der Projeke müssen insgesamt gleich 1.0 betragen!"));
            return;
        }
    }

    ClassSkills skill = readFromForm();
    QString key = skill.getKey();

    if(projektMap().keys().contains(key))
    {
        QMessageBox::information(this, tr("Prüfung speichern"), tr("Die Prüfung existiert bereits!\n"
                     "Das überschreiben von Prüfungen ist nicht zulässig!\n"));
        return;
    }

    if(skill.getProjektMap().size() <= 0)
    {
        QMessageBox::information(this, tr("Prüfung speichern"), tr("Die Prüfung hat keine Projekte!\n"
                                                                   "Projekte müssen zugeordnet werden!\n"));
        return;
    }



    m_skillMap.insert(key, skill);
    emit saveSkillsMap( skillMap() );

    setSkillMap(skillMap());

    if(changeSkill)
        emit skillChanged(skill);

    if(projectFactorChanged){
        QMapIterator<QString, ClassProjekt> ip(selectedSkill.getProjektMap());
        while(ip.hasNext()){
            ip.next();
            emit skillProjektChanged(ip.value());
        }
    }

    ui->deleteSkillProjektButton->setEnabled(false);
    ui->importProjekteButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->changeButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->createButton->setEnabled(true);

    selectedSkill = skill;
    setSkillToForm(selectedSkill);

    changeSkill = false;
    createSkill = false;
    projectFactorChanged = false;
}

void FormSkills::importProjekteButtonClicked()
{

    // Get market projects
    QMap<QString, ClassProjekt> proMap;

    if(selectedSkill.getProjektMap().size() > 0)
        proMap = selectedSkill.getProjektMap();


    int size = ui->projektTable->rowCount();
    int column = ui->projektTable->columnCount()-1;
    for(int i = 0; i < size; i++)
    {
        if(ui->projektTable->item(i,column)->checkState() == Qt::Checked){
            QString key = ui->projektTable->item(i,1)->text()+"."+ui->projektTable->item(i,2)->text();
            ClassProjekt pro = projektMap().value(key);
            proMap.insert(key, pro);
        }
    }

    setSkillProjektToForm(proMap);

}

void FormSkills::sortKennungBoxTextChanged(const QString &text)
{

    if(text == "Alle"){
        ui->importProjekteButton->setEnabled(false);
        setupProjektTable(projektMap(), Qt::Unchecked);
        return;
    }

    QMap<QString, ClassProjekt> sortMap;
    QMapIterator<QString, ClassProjekt> it(projektMap());
    while (it.hasNext()) {
        it.next();
        if(it.value().identifier() == text)
            sortMap.insert(it.key(), it.value());
    }

    if(!sortMap.isEmpty())
        setupProjektTable(sortMap, Qt::Checked);

    if(changeSkill || createSkill)
        ui->importProjekteButton->setEnabled(true);
}

/// !brief Item has selected
void FormSkills::projektTableItemClicked(QTableWidgetItem *)
{
    if(ui->sortKennunBox->currentText() != "Alle")
        return;

    if(isItemChecked(ui->projektTable))
        ui->importProjekteButton->setEnabled(true);
    else
        ui->importProjekteButton->setEnabled(false);
}


QMap<QString, ClassSkills> FormSkills::skillMap() const
{
    return m_skillMap;
}

void FormSkills::setSkillMap(const QMap<QString, ClassSkills> &skillMap)
{
    m_skillMap = skillMap;
    setupSkillTable(skillMap);
}

void FormSkills::skillTableItemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString key = ui->skillTable->item(row,1)->text()+"."+ui->skillTable->item(row,2)->text();
    selectedSkill = skillMap().value(key);
    setSkillToForm(selectedSkill);
    setFormReadOnly(true);
    setFormTextColor(Qt::black);

    changeSkill = false;
    createSkill = false;

    ui->changeButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->createButton->setEnabled(true);
    ui->saveButton->setEnabled(false);

    ui->deleteSkillProjektButton->setEnabled(false);
    ui->importProjekteButton->setEnabled(false);
}

void FormSkills::skillProjektTableItemClicked(QTableWidgetItem *)
{
    if(!changeSkill)
        return;

    if(isItemChecked(ui->skillProjektTable))
        ui->deleteSkillProjektButton->setEnabled(true);
    else
        ui->deleteSkillProjektButton->setEnabled(false);
}

void FormSkills::skillProjektTableCellClicked(int row, int column)
{
    if(!changeSkill)
        return;

    // Factor has clicked
    if(column == 3){
        double fac = ui->skillProjektTable->item(row,3)->text().toDouble();
        if(fac > 1.0){
            QMessageBox::information(this, tr("Eingabe"), tr("Der Faktor eines Projektes darf nichT höher sein wie 1.0"));
            ui->skillProjektTable->item(row,3)->setText("1");
            return;
        }

        projectFactorChanged = true;
    }
}

bool FormSkills::isItemChecked(QTableWidget *widget)
{
    bool status = false;
    int size = widget->rowCount();
    int column = widget->columnCount()-1;
    for(int i = 0; i < size; i++){
        if(widget->item(i,column)->checkState() == Qt::Checked){
            status = true;
            break;
        }
    }

    return status;
}

void FormSkills::kennungBoxTextChanged(const QString &text)
{
    if(createSkill || changeSkill)
        ui->kennungEdit->setText(text);
}

void FormSkills::setupSkillTable(const QMap<QString, ClassSkills> &sMap)
{
    ui->skillTable->clear();
    ui->skillTable->setColumnCount(3);
    ui->skillTable->setRowCount(sMap.size());

    QStringList labels;
    labels << "Nr." << "Name" << "Kennung";
    ui->skillTable->setHorizontalHeaderLabels(labels);

    int row = 0;
    QMapIterator<QString, ClassSkills> it(sMap);
    while (it.hasNext()) {
        it.next();
        ClassSkills skill = it.value();

        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(skill.getNr(),10));
        QTableWidgetItem *itemName = new QTableWidgetItem( skill.name() );
        QTableWidgetItem *itemKennung = new QTableWidgetItem( skill.identifier() );

        ui->skillTable->setItem(row,0, itemNr);
        ui->skillTable->setItem(row,1, itemName);
        ui->skillTable->setItem(row,2, itemKennung);

//        itemNr->setFlags(Qt::ItemIsEnabled);
//        itemName->setFlags(Qt::ItemIsEnabled);
//        itemKennung->setFlags(Qt::ItemIsEnabled);

        row++;
    }

    ui->skillTable->resizeColumnToContents(0);
    ui->skillTable->resizeColumnToContents(1);
    ui->skillTable->resizeColumnToContents(2);
}

void FormSkills::setSkillToForm(const ClassSkills &skill)
{
    ui->nrBox->setValue( skill.getNr());
    ui->nameEdit->setText(skill.name());
    ui->dateEdit->setDate(skill.date());
    ui->kennungEdit->setText(skill.identifier());
    ui->wertBox->setValue(skill.getWert() );
    ui->dateTimeEdit->setDateTime(skill.getCreatedDate());
    setSkillProjektToForm(skill.getProjektMap());

    int min = 0;
    foreach (ClassProjekt p, skill.getProjektMap().values()) {
        min = min + p.getDuration();
    }

    ui->durationBox->setValue(min);

    ui->criteriaBox->setCurrentIndex(skill.criteria());
}

/// !brief Set the skill project table
void FormSkills::setSkillProjektToForm(const QMap<QString, ClassProjekt> &proMap)
{
    ui->skillProjektTable->clear();
    ui->skillProjektTable->setColumnCount(6);
    ui->skillProjektTable->setRowCount(proMap.size());

    QStringList labels;
    labels << "Nr." << "Name" << "Kennung" << "Faktor" << "Marker" << "Datum";
    ui->skillProjektTable->setHorizontalHeaderLabels(labels);

    int row = 0;
    QMapIterator<QString, ClassProjekt> it(proMap);
    while (it.hasNext()) {
        it.next();
        ClassProjekt pro = it.value();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(pro.nr(),10));
        QTableWidgetItem *itemName = new QTableWidgetItem( pro.name() );
        QTableWidgetItem *itemKennung = new QTableWidgetItem( pro.identifier() );
        QTableWidgetItem *itemFactor = new QTableWidgetItem( QString::number(pro.getFactor(),'g',2) );
        QTableWidgetItem *itemCheck = new QTableWidgetItem();
        QTableWidgetItem *itemDate = new QTableWidgetItem( pro.createTime() );

        ui->skillProjektTable->setItem(row, 0, itemNr);
        ui->skillProjektTable->setItem(row, 1, itemName);
        ui->skillProjektTable->setItem(row, 2, itemKennung);
        ui->skillProjektTable->setItem(row, 3, itemFactor);
        ui->skillProjektTable->setItem(row, 4, itemCheck);
        ui->skillProjektTable->setItem(row, 5, itemDate);

        itemFactor->setToolTip(tr("Hier kann der Faktor des Projektes geändert werden.\n"
                                  "Die Änderung wird auch in den Stammdaten des Projektes übertragen.\n"
                                  "Die Faktoren aller Projekte in einer Prüfung muss insgesamt 1.0 betragen!"));

        itemNr->setFlags(Qt::ItemIsEnabled);
        itemName->setFlags(Qt::ItemIsEnabled);
        itemKennung->setFlags(Qt::ItemIsEnabled);
        itemDate->setFlags(Qt::ItemIsEnabled);
        itemCheck->setCheckState(Qt::Unchecked);

        row++;
    }

    ui->skillProjektTable->resizeColumnToContents(0);
    ui->skillProjektTable->resizeColumnToContents(1);
    ui->skillProjektTable->resizeColumnToContents(2);
    ui->skillProjektTable->resizeColumnToContents(3);
    ui->skillProjektTable->resizeColumnToContents(5);

    ui->countProjekteBox->setValue(proMap.size());
}

ClassSkills FormSkills::readFromForm()
{
    ClassSkills skill;

    skill.setNr(ui->nrBox->value());
    skill.setName( ui->nameEdit->text());
    skill.setIdentifier(ui->kennungEdit->text());
    skill.setDate( ui->dateEdit->date() );
    skill.setWert( ui->wertBox->value() );
    skill.setCreatedDate( ui->dateTimeEdit->dateTime() );

    skill.setProjektMap( getSkillProjektMap() );

    int ci = ClassSkills::supportedCriteria().indexOf(ui->criteriaBox->currentText());
    skill.setCriteria( skill.convert(ci) );

    return skill;
}

QMap<QString, ClassProjekt> FormSkills::getSkillProjektMap()
{
    QMap<QString, ClassProjekt> map;
    int size = ui->skillProjektTable->rowCount();
    for(int i = 0; i < size; i++)
    {
        QString key = ui->skillProjektTable->item(i,1)->text()+"."+ui->skillProjektTable->item(i,2)->text();
        ClassProjekt pro = projektMap().value(key);

        if(projectFactorChanged){
            double fac =  ui->skillProjektTable->item(i,3)->text().toDouble();
            pro.setFactor(fac);
        }


        map.insert(key, pro);
        //map.insert(key, projektMap().value(key));
    }

    return map;
}

QMap<QString, ClassProjekt> FormSkills::projektMap() const
{
    return m_projektMap;
}

void FormSkills::setProjektMap(const QMap<QString, ClassProjekt> &projektMap)
{
    m_projektMap = projektMap;
    setupKennungBox(projektMap);
    setupProjektTable(projektMap, Qt::Unchecked);
}

void FormSkills::setupKennungBox(const QMap<QString, ClassProjekt> &proMap)
{
    QStringList kennungList;
    ui->kennungBox->clear();
    ui->sortKennunBox->clear();

    QMapIterator<QString, ClassProjekt> it(proMap);
    while (it.hasNext()) {
        it.next();
        if(!kennungList.contains(it.value().identifier()))
            kennungList << it.value().identifier();
    }

    ui->kennungBox->addItems(kennungList);
    kennungList << "Alle";
    kennungList.sort();
    ui->sortKennunBox->addItems(kennungList);
}

void FormSkills::setupProjektTable(const QMap<QString, ClassProjekt> &proMap, Qt::CheckState state)
{
    ui->projektTable->clear();
    ui->projektTable->setColumnCount(5);
    ui->projektTable->setRowCount(proMap.size());

    QStringList labels;
    labels << "Nr." << "Name" << "Kennung" << "Marker" << "Datum";
    ui->projektTable->setHorizontalHeaderLabels(labels);

    int row = 0;
    QMapIterator<QString, ClassProjekt> it(proMap);
    while (it.hasNext()) {
        it.next();
        ClassProjekt pro = it.value();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(pro.nr(),10));
        QTableWidgetItem *itemName = new QTableWidgetItem( pro.name() );
        QTableWidgetItem *itemKennung = new QTableWidgetItem( pro.identifier() );
        QTableWidgetItem *itemCheck = new QTableWidgetItem();
        QTableWidgetItem *itemDate = new QTableWidgetItem( pro.createTime());

        ui->projektTable->setItem(row, 0, itemNr);
        ui->projektTable->setItem(row, 1, itemName);
        ui->projektTable->setItem(row, 2, itemKennung);
        ui->projektTable->setItem(row, 3, itemCheck);
        ui->projektTable->setItem(row, 4, itemDate);

        itemCheck->setCheckState(state);



        row++;
    }

    ui->projektTable->resizeColumnToContents(0);
    ui->projektTable->resizeColumnToContents(1);
    ui->projektTable->resizeColumnToContents(2);
    ui->projektTable->resizeColumnToContents(3);
    ui->projektTable->resizeColumnToContents(4);
}

double FormSkills::getSkillProjectFactor()
{
    double factor = 0.0;
    int size = ui->skillProjektTable->rowCount();
    for(int i = 0; i < size; i++)
    {
        factor = factor + ui->skillProjektTable->item(i,3)->text().toDouble();

    }

    return factor;
}

void FormSkills::setFormTextColor(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    palette.setColor(QPalette::Text,color);

    ui->nrBox->setPalette(palette);
    ui->nameEdit->setPalette(palette);
    ui->kennungEdit->setPalette(palette);
    ui->dateEdit->setPalette(palette);
    ui->dateTimeEdit->setPalette(palette);
    ui->wertBox->setPalette(palette);

    int size = ui->skillProjektTable->rowCount();
    for(int i = 0; i < size; i++)
    {
        ui->skillProjektTable->item(i,0)->setTextColor(color);
        ui->skillProjektTable->item(i,1)->setTextColor(color);
        ui->skillProjektTable->item(i,2)->setTextColor(color);
        ui->skillProjektTable->item(i,3)->setTextColor(color);
    }
}

void FormSkills::setFormReadOnly(bool status)
{
    ui->nrBox->setReadOnly(status);
    ui->nameEdit->setReadOnly(status);
    ui->dateEdit->setReadOnly(status);
    ui->kennungEdit->setReadOnly(status);
    ui->kennungBox->setEnabled(!status);
    ui->criteriaBox->setEnabled(!status);
}

void FormSkills::clearForm()
{
    ui->nrBox->setValue(0);
    ui->nameEdit->clear();
    ui->dateEdit->setDate(QDate());
    ui->kennungEdit->clear();
    ui->wertBox->setValue(0);

    ui->skillProjektTable->setRowCount(0);
    ui->skillProjektTable->clear();
}
