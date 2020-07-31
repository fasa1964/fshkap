#include "formlehrling.h"
#include "ui_formlehrling.h"

#include <QMessageBox>
#include <QDebug>

FormLehrling::FormLehrling(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLehrling)
{
    ui->setupUi(this);

    model = new QStandardItemModel();


    setupKlasseMap();
    ui->klasseBox->addItems(klasseMap.keys());

    setFormReadOnly(true);
    changeLehrling = false;
    selectedLehrling = ClassLehrling();
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->closeButton, &QPushButton::clicked, this, &FormLehrling::close);
    connect(ui->createButton, &QPushButton::clicked, this, &FormLehrling::createButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &FormLehrling::deleteButtonClicked);
    connect(ui->deleteSkillButton, &QPushButton::clicked, this, &FormLehrling::deleteSkillButtonClicked);
    connect(ui->changeButton, &QPushButton::clicked, this, &FormLehrling::changeButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &FormLehrling::saveButtonClicked);
    connect(ui->klasseBox, &QComboBox::currentTextChanged , this, &FormLehrling::klasseBoxChanged);


    connect(ui->azubiTableWidget , &QTableWidget::itemClicked , this, &FormLehrling::azubiItemClicked);
    connect(ui->betriebButton, &QPushButton::clicked, this, &FormLehrling::openBetriebsListe);

    connect(model, SIGNAL(dataChanged ( const QModelIndex&, const QModelIndex&)), this, SLOT(slotchanged(const QModelIndex&, const QModelIndex&)));

}

FormLehrling::~FormLehrling()
{
    delete ui;
}

void FormLehrling::createButtonClicked()
{
    clearForm();
    setFormReadOnly(false);
    ui->nrBox->setFocus();

    ui->saveButton->setEnabled(true);
    ui->createButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->changeButton->setEnabled(false);

}

void FormLehrling::deleteButtonClicked()
{
    int result = QMessageBox::information(this, tr("Löschen Lehrling"), tr("Alle Daten vom Lehrling einschl. der Prüfungsergebnisse\n"
                 "werden unwiderruflich gelöscht!"), QMessageBox::Ok | QMessageBox::Abort);

    if(result == QMessageBox::Abort)
        return;

    QString key = selectedLehrling.getKey();
    if(m_lehrlingMap.remove(key) == 1)
    {

        emit saveLehrlingMap(m_lehrlingMap);
        emit updateBetriebe("", selectedLehrling);
        updateLehrlingTable(m_lehrlingMap);

        clearForm();
        if(!lehrlingMap().values().isEmpty()){
            selectedLehrling = lehrlingMap().values().first();
            setLehrlingToForm(selectedLehrling);
        }
        else {
            selectedLehrling = ClassLehrling();
            ui->deleteButton->setEnabled(false);
            ui->changeButton->setEnabled(false);
            ui->saveButton->setEnabled(false);
        }
    }

}

void FormLehrling::deleteSkillButtonClicked()
{
    QList<ClassSkills> skillList;

    int rows = ui->exameBox->count();
    for (int i = 0; i < rows; i++) {
        if(model->item(i,0)->checkState() == Qt::Checked)
           skillList << selectedLehrling.getSkillMap().value(model->item(i,0)->text());
    }

    QString message = "Vom Auszubildenden: " + selectedLehrling.vorname()+"."+selectedLehrling.nachname()+"\n"+
            "Werden die Prüfungen:\n";

    foreach(ClassSkills s, skillList){
        message.append(s.getKey());
        message.append("\n");
    }

    message.append("unwiderruflich gelöscht?");


    int result = QMessageBox::question(this, tr("Löschen der Prüfungen"), message, QMessageBox::Ok | QMessageBox::Cancel);
    if(result == QMessageBox::Cancel)
        return;

    QMap<QString, ClassSkills> smap;
    smap = selectedLehrling.getSkillMap();
    foreach(ClassSkills s, skillList){
        smap.remove(s.getKey());
    }

    selectedLehrling.setSkillMap(smap);
    m_lehrlingMap.insert(selectedLehrling.getKey(), selectedLehrling);
    updateLehrlingTable(m_lehrlingMap);
    setLehrlingToForm(selectedLehrling);
    ui->deleteSkillButton->setEnabled(false);

}

void FormLehrling::changeButtonClicked()
{
    changeLehrling = true;
    setFormReadOnly(false);
    setFormTextColor(QColor(0,87,127));
    ui->strasseEdit->setFocus();

    ui->changeButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->createButton->setEnabled(false);
    ui->saveButton->setEnabled(true);

}

void FormLehrling::saveButtonClicked()
{

    if(ui->nrBox->value() == 0 || ui->vornameEdit->text().isEmpty() || ui->nachnameEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Speichern Lehrling"), tr("Prüfungs-Nummer, Vorname, Nachname und Ausbildungsdatum "
                                        "müssen angegeben werden!"));
        return;
    }

    QDate today = QDate::currentDate();
    int jahrgang = today.year() - ui->ausbildungsDateEdit->date().year();

    if(jahrgang <= 0)
        jahrgang = 1;

    if(jahrgang > 5)
        jahrgang = 5;

    if(jahrgang >= 5)
    {
        int result = QMessageBox::information(this, tr("Speichern Lehrling"), tr("Das Ausbildungsdatum liegt länger als 4 Jahre zurück.\n"
                     "Wenn der Lehrling ein Nachholer ist, dann ist das ok. Ansonsten bitte das Ausbildungsdatum anpassen!"), QMessageBox::Yes | QMessageBox::Abort);

        if(result == QMessageBox::Abort)
            return;
    }


    ClassLehrling azubi = readFromForm();

    if(changeLehrling){
        azubi.setSkillMap( selectedLehrling.getSkillMap() );
        m_lehrlingMap.remove(selectedLehrling.getKey());
    }

    if(m_lehrlingMap.keys().contains(azubi.getKey()) && !changeLehrling)
    {
        int result = QMessageBox::question(this, tr("Speichern Lehrling"), tr("Den Auszubildenden gibt es bereits.\n"
                     "Sollen die Daten überschrieben werden?"), QMessageBox::Yes | QMessageBox::Abort);

        if(result == QMessageBox::Abort)
            return;
    }

   if(pruefnummerExist(jahrgang, ui->nrBox->value()))
   {
           QMessageBox::information(this, tr("Speichern Lehrling"), tr("Die Prüfungsnummer existiert bereits.\n"
                                            "Bitte ändere die Prüfungsnummer!"));

            return;
   }


    m_lehrlingMap.insert(azubi.getKey(), azubi);
    updateLehrlingTable(m_lehrlingMap);
    emit saveLehrlingMap(m_lehrlingMap);
    emit updateBetriebe(azubi.betrieb(), azubi);

    selectedLehrling = azubi;
    changeLehrling = false;

    ui->saveButton->setEnabled(false);
    ui->createButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->changeButton->setEnabled(true);

    setFormReadOnly(true);
    setFormTextColor(Qt::black);


}

void FormLehrling::klasseBoxChanged(const QString &text)
{
    ui->klasseEdit->setText(text);
    ui->ausbildungsDateEdit->setDate( klasseMap.value(text));

}


void FormLehrling::slotchanged(const QModelIndex &index, const QModelIndex &)
{
    if(!changeLehrling)
        return;

    bool select = false;
    int rows = ui->exameBox->count();
    for (int i = 0; i < rows; i++) {
        if(model->item(i,0)->checkState() == Qt::Checked)
            select = true;
    }

    if(select)
       ui->deleteSkillButton->setEnabled(true);
    else
       ui->deleteSkillButton->setEnabled(false);

}

void FormLehrling::azubiItemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString key;

    if(ui->tabWidget->currentIndex() == 0)
        key = ui->azubiTableWidget->item(row,1)->text()+"."+ui->azubiTableWidget->item(row,0)->text();

    if(ui->tabWidget->currentIndex() == 1)
        key = ui->lehrling1Table->item(row,1)->text()+"."+ui->lehrling1Table->item(row,0)->text();

    if(ui->tabWidget->currentIndex() == 2)
        key = ui->lehrling2Table->item(row,1)->text()+"."+ui->lehrling2Table->item(row,0)->text();;

    if(ui->tabWidget->currentIndex() == 3)
        key = ui->lehrling3Table->item(row,1)->text()+"."+ui->lehrling3Table->item(row,0)->text();;

    if(ui->tabWidget->currentIndex() == 4)
        key = ui->lehrling4Table->item(row,1)->text()+"."+ui->lehrling4Table->item(row,0)->text();;

    selectedLehrling = lehrlingMap().value(key);
    setLehrlingToForm(selectedLehrling);
    setFormReadOnly(true);

    ui->saveButton->setEnabled(false);
    ui->changeButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->createButton->setEnabled(true);
    ui->deleteSkillButton->setEnabled(false);

}

void FormLehrling::openBetriebsListe()
{
    FormBetriebListe *dlg = new FormBetriebListe();
    dlg->updateTable(betriebMap());
    dlg->exec();
    QString betriebname = dlg->verify();
    if(!betriebname.isEmpty())
        ui->betriebEdit->setText( betriebname );
}

void FormLehrling::updateLehrlingTable(const QMap<QString, ClassLehrling> &aMap)
{
    ui->azubiTableWidget->clear();
    ui->azubiTableWidget->setColumnCount(4);
    ui->azubiTableWidget->setRowCount(aMap.size());

    QStringList headers;
    headers << "Nr." << "Name" << "Klasse" << "Betrieb";
    ui->azubiTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    QMapIterator<QString, ClassLehrling> it(aMap);
    while (it.hasNext()) {
       it.next();

       QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(it.value().nr(),10) );
       QTableWidgetItem *itemName = new QTableWidgetItem(it.value().vorname()+"."+it.value().nachname());
       QTableWidgetItem *itemKlasse = new QTableWidgetItem(it.value().klasse());
       QTableWidgetItem *itemBetrieb = new QTableWidgetItem(it.value().betrieb());

       ui->azubiTableWidget->setItem(row,0, itemNr);
       ui->azubiTableWidget->setItem(row,1, itemName);
       ui->azubiTableWidget->setItem(row,2, itemKlasse);
       ui->azubiTableWidget->setItem(row,3, itemBetrieb);

       itemNr->setTextColor(QColor(0,85,127,255));
       itemName->setTextColor(QColor(0,85,127,255));
//       itemNr->setFlags(Qt::ItemIsEnabled);
//       itemName->setFlags(Qt::ItemIsEnabled);
//       itemName->setFlags(Qt::ItemIsSelectable);
//       itemKlasse->setFlags(Qt::ItemIsEnabled);
//       itemBetrieb->setFlags(Qt::ItemIsEnabled);

       row++;
    }

    ui->azubiTableWidget->resizeColumnToContents(0);
    ui->azubiTableWidget->resizeColumnToContents(1);
    ui->azubiTableWidget->resizeColumnToContents(2);

    createSortMap(aMap);
}

void FormLehrling::createSortMap(const QMap<QString, ClassLehrling> &aMap)
{
    QDate cDate = QDate::currentDate();
    QMultiMap<int, ClassLehrling> map;

    QMapIterator<QString, ClassLehrling> it(aMap);
    while (it.hasNext()) {
        it.next();
        int year = cDate.year() - it.value().ausbildungsDatum().year();
        if(year <= 0)
            year = 1;
        if(year > 4)    // For repeater
            year = 5;
        map.insert(year, it.value());
    }

    for (int i = 1; i < 6; i++)
    {

        if(map.values(i).size() == 0)
            ui->tabWidget->setTabEnabled(i, false);


        if(map.values(i).size() > 0)
        {
            ui->tabWidget->setTabEnabled(i, true);
            if(i == 1)
                updateTable(ui->lehrling1Table, map.values(i) );
            if(i == 2)
                updateTable(ui->lehrling2Table, map.values(i) );
            if(i == 3)
                updateTable(ui->lehrling3Table, map.values(i) );
            if(i == 4)
                updateTable(ui->lehrling4Table, map.values(i) );
            if(i == 5)
                updateTable(ui->nachholerTable, map.values(i) );
        }
    }
}

bool FormLehrling::pruefnummerExist(int year, int nr)
{
    bool status = false;
    QDate today = QDate::currentDate();
    QMapIterator<QString, ClassLehrling> it(lehrlingMap());
    while (it.hasNext()) {
        it.next();
        int ayear = today.year() - it.value().ausbildungsDatum().year();
        if(ayear <= 0)
            ayear = 1;

        if( ayear == year){
            if(it.value().nr() == nr){
                status = true;
                break;
            }
        }
    }

    return status;
}



void FormLehrling::updateTable(QTableWidget *widget, const QList<ClassLehrling> &azuList)
{
    widget->clear();

    widget->setRowCount(azuList.size());
    widget->setColumnCount(4);

    QStringList labels;
    labels << "Nr." << "Name" << "Klasse" << "Betrieb";
    widget->setHorizontalHeaderLabels(labels);

    int row = 0;
    foreach(ClassLehrling azu, azuList){

        QString key = azu.vorname()+"."+azu.nachname();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(azu.nr(),10));
        QTableWidgetItem *itemName = new QTableWidgetItem( key );
        QTableWidgetItem *itemKlasse = new QTableWidgetItem( azu.klasse() );
        QTableWidgetItem *itemBetrieb = new QTableWidgetItem( azu.betrieb() );

        widget->setItem(row,0,itemNr);
        widget->setItem(row,1,itemName);
        widget->setItem(row,2,itemKlasse);
        widget->setItem(row,3,itemBetrieb);

        itemNr->setTextColor(QColor(0,85,127,255));
        itemNr->setToolTip(tr("Prüfungsnummer"));
        itemName->setTextColor(QColor(0,85,127,255));
//        itemNr->setFlags(Qt::ItemIsEnabled);
//        itemName->setFlags(Qt::ItemIsEnabled);
//        itemBetrieb->setFlags(Qt::ItemIsEnabled);

        row++;
    }

    widget->resizeColumnToContents(0);
    widget->resizeColumnToContents(1);
    widget->resizeColumnToContents(2);
    widget->resizeColumnToContents(4);

    connect(widget, &QTableWidget::itemClicked, this, &FormLehrling::azubiItemClicked);
}

ClassLehrling FormLehrling::readFromForm()
{
    ClassLehrling azubi;

    azubi.setNr( ui->nrBox->value() );
    azubi.setAusbildungsDatum( ui->ausbildungsDateEdit->date() );
    azubi.setVorname( ui->vornameEdit->text() );
    azubi.setNachname( ui->nachnameEdit->text() );
    azubi.setStrasse( ui->strasseEdit->text() );
    azubi.setOrt( ui->ortEdit->text() );
    azubi.setTelefon( ui->telefonEdit->text() );
    azubi.setKlasse( ui->klasseEdit->text());
    azubi.setGebDatum( ui->gebDateEdit->date() );
    azubi.setBetrieb( ui->betriebEdit->text() );
    azubi.setNotiz( ui->notizEdit->toPlainText());


    return azubi;
}

void FormLehrling::setLehrlingToForm(const ClassLehrling &azubi)
{
    ui->nrBox->setValue(azubi.nr());
    ui->vornameEdit->setText(azubi.vorname());
    ui->nachnameEdit->setText(azubi.nachname());
    ui->ausbildungsDateEdit->setDate(azubi.ausbildungsDatum());
    ui->strasseEdit->setText(azubi.strasse());
    ui->ortEdit->setText(azubi.ort());
    ui->klasseEdit->setText( azubi.klasse());
    ui->telefonEdit->setText(azubi.telefon());
    ui->betriebEdit->setText( azubi.betrieb());
    ui->gebDateEdit->setDate(azubi.gebDatum());
    ui->notizEdit->setPlainText( azubi.notiz());

    ui->exameBox->clear();
    model->clear();

    if(azubi.getSkillMap().size() > 0){
        QStringList apList;
        int index = 0;
        foreach(ClassSkills skill, azubi.getSkillMap().values()){
            apList << skill.getKey();
            QStandardItem *item = new QStandardItem();
            item->setText(skill.getKey());
            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            item->setData(Qt::Unchecked, Qt::CheckStateRole);
            model->insertRow(index, item);
            index++;
        }



        ui->exameBox->setModel(model);

    }

    ui->klasseBox->setCurrentText(azubi.klasse());

}

void FormLehrling::setFormTextColor(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    palette.setColor(QPalette::Text,color);
    palette.setColor(QPalette::ButtonText,color);

    ui->nrBox->setPalette(palette);
    ui->vornameEdit->setPalette(palette);
    ui->nachnameEdit->setPalette(palette);
    ui->ausbildungsDateEdit->setPalette(palette);
    ui->strasseEdit->setPalette(palette);
    ui->ortEdit->setPalette(palette);
    ui->telefonEdit->setPalette(palette);
    ui->gebDateEdit->setPalette(palette);
    ui->notizEdit->setPalette(palette);
    ui->klasseEdit->setPalette(palette);
    ui->betriebEdit->setPalette(palette);
    ui->exameBox->setPalette(palette);

}

void FormLehrling::setFormReadOnly(bool status)
{
    ui->nrBox->setReadOnly(status);
    ui->vornameEdit->setReadOnly(status);
    ui->nachnameEdit->setReadOnly(status);
    ui->ausbildungsDateEdit->setReadOnly(status);
    ui->strasseEdit->setReadOnly(status);
    ui->ortEdit->setReadOnly(status);
    ui->telefonEdit->setReadOnly(status);
    ui->gebDateEdit->setReadOnly(status);
    ui->notizEdit->setReadOnly(status);
    ui->klasseEdit->setReadOnly(status);
    ui->klasseBox->setEnabled(!status);

    ui->betriebEdit->setReadOnly(true);
    ui->betriebButton->setEnabled(!status);

    if(changeLehrling && !status)
        ui->betriebEdit->setReadOnly(false);

    if(!status)
        setFormTextColor(QColor(0,85,127));
    else
        setFormTextColor(Qt::black);

}

void FormLehrling::clearForm()
{
    ui->nrBox->setValue(0);
    ui->vornameEdit->clear();
    ui->nachnameEdit->clear();
    ui->ausbildungsDateEdit->setDate( QDate() );
    ui->strasseEdit->clear();
    ui->ortEdit->clear();
    ui->telefonEdit->clear();
    ui->betriebEdit->clear();
    ui->gebDateEdit->setDate( QDate());
    ui->notizEdit->clear();
    ui->exameBox->clear();
}

QMap<int, ClassBetrieb> FormLehrling::betriebMap() const
{
    return m_betriebMap;
}

void FormLehrling::setBetriebMap(const QMap<int, ClassBetrieb> &betriebMap)
{
    m_betriebMap = betriebMap;
}

void FormLehrling::setupKlasseMap()
{
    klasseMap.insert("MAS 16/1", QDate(2016,8,1));
    klasseMap.insert("MAS 17/1", QDate(2017,8,1));
    klasseMap.insert("MAS 18/1", QDate(2018,8,1));
    klasseMap.insert("MAS 19/1", QDate(2019,8,1));
    klasseMap.insert("MAS 20/1", QDate(2020,8,1));
    klasseMap.insert("MAS 21/1", QDate(2021,8,1));
    klasseMap.insert("MAS 22/1", QDate(2022,8,1));
    klasseMap.insert("MAS 23/1", QDate(2023,8,1));
    klasseMap.insert("MAS 24/1", QDate(2024,8,1));
    klasseMap.insert("MAS 25/1", QDate(2025,8,1));
    klasseMap.insert("MAS 26/1", QDate(2026,8,1));
    klasseMap.insert("MAS 27/1", QDate(2027,8,1));
    klasseMap.insert("MAS 28/1", QDate(2028,8,1));
    klasseMap.insert("MAS 29/1", QDate(2029,8,1));
    klasseMap.insert("MAS 30/1", QDate(2030,8,1));
}

QMap<QString, ClassLehrling> FormLehrling::lehrlingMap() const
{
    return m_lehrlingMap;
}

void FormLehrling::setLehrlingMap(const QMap<QString, ClassLehrling> &lehrlingMap)
{
    m_lehrlingMap = lehrlingMap;
}

void FormLehrling::setLastModified(const QDateTime &date)
{
    ui->lastChangeEdit->setDateTime(date);
}
