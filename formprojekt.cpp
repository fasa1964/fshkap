#include "formprojekt.h"
#include "ui_formprojekt.h"

#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QMapIterator>
#include <QFileDialog>

#include <QDebug>

FormProjekt::FormProjekt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormProjekt)
{
    ui->setupUi(this);
    setFormReadOnly(true);
    changeProjekt = false;
    selectedProjekt = ClassProjekt();


    connect(ui->closeButton, &QPushButton::clicked, this, &FormProjekt::closeForm);
    connect(ui->createButton, &QPushButton::clicked, this, &FormProjekt::createButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &FormProjekt::saveButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &FormProjekt::deleteButtonClicked);
    connect(ui->changeButton, &QPushButton::clicked, this, &FormProjekt::changeButtonClicked);
    connect(ui->addFrageButton, &QToolButton::clicked, this, &FormProjekt::addFrageButtonClicked);
    connect(ui->removeFrageButton, &QToolButton::clicked, this, &FormProjekt::removeFrageButtonClicked);
    connect(ui->openFilelButton , &QToolButton::clicked, this, &FormProjekt::openFileButtonClicked );

    connect(ui->anzahlFragenBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
         [=](int i){ anzahlFragenChanged(i); });

    connect(ui->projekteTableWidget, &QTableWidget::itemClicked, this, &FormProjekt::projekteTableClicked);
    connect(ui->sortBox, &QComboBox::currentTextChanged, this, &FormProjekt::sortBoxTextChanged);

}

FormProjekt::~FormProjekt()
{
    delete ui;
}

void FormProjekt::updateProjektTable(const QMap<QString, ClassProjekt> &proMap)
{
    ui->projekteTableWidget->clear();
    ui->projekteTableWidget->setColumnCount(3);
    ui->projekteTableWidget->setRowCount(proMap.size());

    QStringList labels;
    labels << "Nr." << "Name" << "Kennung" ;
    ui->projekteTableWidget->setHorizontalHeaderLabels(labels);
    int row = 0;
    QMapIterator<QString, ClassProjekt> it(proMap);
    while (it.hasNext()) {
        it.next();
        ClassProjekt pro = it.value();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(pro.nr(),10));
        QTableWidgetItem *itemName = new QTableWidgetItem( pro.name() );
        QTableWidgetItem *itemKennung = new QTableWidgetItem( pro.identifier() );

        ui->projekteTableWidget->setItem(row, 0, itemNr);
        ui->projekteTableWidget->setItem(row, 1, itemName);
        ui->projekteTableWidget->setItem(row, 2, itemKennung);

        row++;
    }

    ui->projekteTableWidget->resizeColumnToContents(0);
    ui->projekteTableWidget->resizeColumnToContents(1);
    ui->projekteTableWidget->resizeColumnToContents(2);
}

void FormProjekt::closeForm()
{
    close();
}

void FormProjekt::createButtonClicked()
{
    QDateTime dt = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(dt);


    setFormReadOnly(false);
    ui->nrBox->setFocus();

    clearForm();
    selectedProjekt = ClassProjekt();
    changeProjekt = false;
    ui->saveButton->setEnabled(true);
    ui->createButton->setEnabled(false);
}

void FormProjekt::saveButtonClicked()
{
   // Check some values
    if(ui->nrBox->value() <= 0 || ui->nameEdit->text().isEmpty() || ui->kennungEdit->text().isEmpty()
            || ui->anzahlFragenBox->value() <= 0)
    {
        QMessageBox::information(this, tr("Projekt speichern"), tr("Folgende Angaben sind zum Speicher erforderlich!")+"\n"
                                 +tr("Projekt: Nr, Name, Kennung, Anzahl der Fragen und die max. Punkte bei den Fragen!"));
        return;
    }

    if(changeProjekt){
        QString key = selectedProjekt.getKey();
        m_projektMap.remove(key);
    }

    ClassProjekt projekt = readFromForm();
    QString key = projekt.getKey();

    if(!changeProjekt)
    {
        if(projektMap().keys().contains(key))
        {
            QMessageBox::information(this, tr("Projekt speichern"), tr("Das Projekt existiert bereits!\n"
                         "Das überschreiben von Projekten ist nicht zulässig!\n"));
            return;
        }
    }

    m_projektMap.insert(key, projekt);
    emit saveProjekte(m_projektMap);

    if(!changeProjekt)
        emit projektAdded(projekt);

    if(changeProjekt)
        emit projektChanged(projekt);

    ui->saveButton->setEnabled(false);
    ui->createButton->setEnabled(true);
    ui->changeButton->setEnabled(true);

    setProjektToForm(projekt);
    setFormReadOnly(true);
    setColorTableFragen(Qt::black);

    updateSortBox();
    updateProjektTable(projektMap());

    changeProjekt = false;
}

void FormProjekt::changeButtonClicked()
{
    QDateTime dt = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(dt);

    changeProjekt = true;
    ui->nrBox->setFocus();
    setFormReadOnly(false);
    setColorTableFragen(Qt::blue);

//    ui->nameEdit->setReadOnly(true);
//    ui->kennungEdit->setReadOnly(true);
    ui->anzahlFragenBox->setEnabled(false);

    ui->changeButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->createButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
}

void FormProjekt::deleteButtonClicked()
{

    m_projektMap.remove(selectedProjekt.getKey());
    emit saveProjekte(m_projektMap);

    updateProjektTable(projektMap());
    updateSortBox();

    if(projektMap().values().isEmpty()){
        clearForm();
        clearTableFragen();
        ui->deleteButton->setEnabled(false);
        ui->changeButton->setEnabled(false);
    }else
        setProjektToForm( projektMap().values().first());

}


void FormProjekt::openFileButtonClicked()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Dateipfad"), tr("Dateipfad zum hinterlegtem Dokument!"));
    ui->documentEdit->setText(s);
}

void FormProjekt::addFrageButtonClicked()
{
    int rowCount = ui->fragenTableWidget->rowCount();
    ui->anzahlFragenBox->setValue(rowCount+1);

    ui->fragenTableWidget->insertRow(rowCount);

    QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(rowCount+1,10) );
    QTableWidgetItem *itemFrage = new QTableWidgetItem("Frage");
    QTableWidgetItem *itemPunkte = new QTableWidgetItem("0");
    QTableWidgetItem *itemKennung = new QTableWidgetItem("");

    ui->fragenTableWidget->setItem(rowCount, 0, itemNr);
    ui->fragenTableWidget->setItem(rowCount, 1, itemFrage);
    ui->fragenTableWidget->setItem(rowCount, 2, itemPunkte);
    ui->fragenTableWidget->setItem(rowCount, 3, itemKennung);

    itemNr->setTextColor( ui->fragenTableWidget->item(0,1)->textColor() );
    itemFrage->setTextColor( ui->fragenTableWidget->item(0,1)->textColor() );

}

void FormProjekt::removeFrageButtonClicked()
{
    int rowCount = ui->fragenTableWidget->rowCount()-1;
    ui->fragenTableWidget->removeRow(rowCount);
    ui->anzahlFragenBox->setValue(rowCount);
}

void FormProjekt::anzahlFragenChanged(int value)
{

    if(changeProjekt)
        return;

    clearTableFragen();
    ui->fragenTableWidget->setRowCount(value);

    int nr = 1;
    for(int i = 0; i < value; i++)
    {
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(nr,10) );
        QTableWidgetItem *itemFrage = new QTableWidgetItem( QString("Frage"));
        QTableWidgetItem *itemPunkte = new QTableWidgetItem( QString("0"));
        QTableWidgetItem *itemKennung = new QTableWidgetItem( "" );
        ui->fragenTableWidget->setItem(i,0, itemNr);
        ui->fragenTableWidget->setItem(i,1, itemFrage);
        ui->fragenTableWidget->setItem(i,2, itemPunkte);
        ui->fragenTableWidget->setItem(i,3, itemKennung);
        nr++;
    }

     setColorTableFragen(Qt::blue);
}

void FormProjekt::projekteTableClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString key = ui->projekteTableWidget->item(row, 1)->text()+"."+ ui->projekteTableWidget->item(row, 2)->text();
    selectedProjekt = projektMap().value(key);
    setProjektToForm(selectedProjekt);
    setFormReadOnly(true);

    ui->changeButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->createButton->setEnabled(true);
    ui->saveButton->setEnabled(false);
    changeProjekt = false;
}

void FormProjekt::sortBoxTextChanged(const QString &text)
{
    if(text == "Alle"){
        updateProjektTable(projektMap());
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
        updateProjektTable(sortMap);
}

void FormProjekt::setFormReadOnly(bool status)
{
    ui->nrBox->setReadOnly(status);
    ui->nameEdit->setReadOnly(status);
    ui->kennungEdit->setReadOnly(status);
    ui->anzahlFragenBox->setReadOnly(status);
    ui->maxPunkteBox->setReadOnly(status);
    ui->documentEdit->setReadOnly(status);
    ui->addFrageButton->setEnabled(!status);
    ui->removeFrageButton->setEnabled(!status);
    ui->openFilelButton->setEnabled(!status);
    ui->sperrfachBox->setEnabled(!status);
    ui->factorBox->setReadOnly(status);
    ui->durationBox->setReadOnly(status);

    if(!changeProjekt){
        ui->anzahlFragenBox->setEnabled(true);
        ui->anzahlFragenBox->setReadOnly(status);
    }

    if(!status)
        setFormTextColor(QColor(0,85,127));
    else
        setFormTextColor(Qt::black);
}

ClassProjekt FormProjekt::readFromForm()
{
    ClassProjekt pro;
    pro.setNr( ui->nrBox->value() );
    pro.setName( ui->nameEdit->text() );
    pro.setIdentifier( ui->kennungEdit->text() );
    pro.setCountQuestion( ui->anzahlFragenBox->value() );
    pro.setMaxPoints(ui->maxPunkteBox->value());
    pro.setDocument(ui->documentEdit->text());
    pro.setDateTime( ui->dateTimeEdit->dateTime());
    pro.setFactor(ui->factorBox->value());
    pro.setDuration( ui->durationBox->value());

    if(ui->sperrfachBox->checkState() == Qt::Checked)
        pro.setLockSubject(true);
    else
        pro.setLockSubject(false);

    pro.setCreateTime( ui->dateTimeEdit->dateTime().toString("dd.MM.yy hh:mm"));

    // update ClassFragen Map
    int maxpunkte = 0;
    int rowCount = ui->anzahlFragenBox->value(); //ui->fragenTableWidget->rowCount();

    QMap<int, ClassFrage> frgMap;
    for(int i = 0; i < rowCount; i++)
    {
        ClassFrage question;
        int nr = ui->fragenTableWidget->item(i,0)->text().toInt();
        QString frg = ui->fragenTableWidget->item(i,1)->text();
        int punkte = ui->fragenTableWidget->item(i,2)->text().toInt();
        QString identifier = ui->fragenTableWidget->item(i,3)->text();

        question.setQuestionNr(nr);
        question.setQuestion(frg);
        question.setMaxPoints(punkte);
        question.setIdentifier(identifier);

        maxpunkte = maxpunkte + punkte;

        frgMap.insert(i, question);
    }

    if(maxpunkte != pro.maxPoints())
    {
        QMessageBox::information(this, tr("Projekt speichern"), tr("Die Punktzahl stimmt nicht mit den Punkten der Fragen überein!\n"
                     "Die maximale Punktzahl wird angeglichen!"));

        pro.setMaxPoints(maxpunkte);
        ui->maxPunkteBox->setValue(maxpunkte);
    }

    pro.setQuestionMap(frgMap);
    return pro;
}

void FormProjekt::clearTableFragen()
{
    ui->fragenTableWidget->clear();
    ui->fragenTableWidget->setColumnCount(4);

    QStringList labels;
    labels << "Nr." << "Frage" << "Max. Punkte" << "Kennung" ;
    ui->fragenTableWidget->setHorizontalHeaderLabels(labels);

}


void FormProjekt::setColorTableFragen(QColor color)
{
    int rowCount = ui->fragenTableWidget->rowCount();
    int columnCount = ui->fragenTableWidget->columnCount();

    for(int i = 0; i < rowCount; i++)
    {
        for(int c = 0; c < columnCount; c++)
        {
            QTableWidgetItem *item = ui->fragenTableWidget->item(i,c);
            item->setTextColor(color);
        }
    }
}

void FormProjekt::setFormTextColor(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    palette.setColor(QPalette::Text,color);
    palette.setColor(QPalette::ButtonText,color);

    ui->nrBox->setPalette(palette);
    ui->nameEdit->setPalette(palette);
    ui->kennungEdit->setPalette(palette);
    ui->anzahlFragenBox->setPalette(palette);
    ui->maxPunkteBox->setPalette(palette);
    ui->dateTimeEdit->setPalette(palette);
    ui->documentEdit->setPalette(palette);
    //ui->sperrfachBox->setPalette(palette);
    ui->durationBox->setPalette(palette);
    ui->factorBox->setPalette(palette);

}

void FormProjekt::setProjektToForm(const ClassProjekt &pro)
{
    ui->nrBox->setValue(pro.nr());
    ui->nameEdit->setText(pro.name());
    ui->kennungEdit->setText(pro.identifier());
    ui->anzahlFragenBox->setValue(pro.countQuestion());
    ui->maxPunkteBox->setValue(pro.maxPoints());
    ui->documentEdit->setText( pro.document() );
    ui->factorBox->setValue(pro.getFactor());
    ui->durationBox->setValue(pro.getDuration());

    if(pro.lockSubject())
        ui->sperrfachBox->setCheckState(Qt::Checked);
    else
        ui->sperrfachBox->setCheckState(Qt::Unchecked);

    if(pro.dateTime().isValid()){
        ui->dateTimeEdit->setDateTime(pro.dateTime());
    }else{

        emit invalidDateTime();
        QString dts = pro.createTime();
    }

    updateFragenTable(pro.questionMap());
}

void FormProjekt::clearForm()
{
    ui->nrBox->setValue(0);
    ui->nameEdit->clear();
    ui->kennungEdit->clear();
    ui->anzahlFragenBox->setValue(0);
    ui->maxPunkteBox->setValue(0);
    ui->dateTimeEdit->clear();
    ui->documentEdit->clear();
    ui->sperrfachBox->setCheckState(Qt::Unchecked);
    ui->durationBox->setValue(0);

    clearTableFragen();
}

void FormProjekt::updateFragenTable(const QMap<int, ClassFrage> &fMap)
{
    ui->fragenTableWidget->clear();
    ui->fragenTableWidget->setColumnCount(4);
    ui->fragenTableWidget->setRowCount(fMap.size());

    QStringList labels;
    labels << "Nr." << "Frage" << "Max. Punkte" << "Kennung" ;
    ui->fragenTableWidget->setHorizontalHeaderLabels(labels);

    int row = 0;
    QMapIterator<int, ClassFrage> it(fMap);
    while (it.hasNext()) {
        it.next();

        ClassFrage question = it.value();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(question.questionNr(),10) );
        QTableWidgetItem *itemFrage = new QTableWidgetItem( question.question() );
        QTableWidgetItem *itemPunkte = new QTableWidgetItem( QString::number(question.maxPoints(),10) );
        QTableWidgetItem *itemKennung = new QTableWidgetItem( question.identifier() );
        ui->fragenTableWidget->setItem(row,0, itemNr);
        ui->fragenTableWidget->setItem(row,1, itemFrage);
        ui->fragenTableWidget->setItem(row,2, itemPunkte);
        ui->fragenTableWidget->setItem(row,3, itemKennung);

//        itemNr->setFlags(Qt::ItemIsEnabled);
//        itemFrage->setFlags(Qt::ItemIsEnabled);
//        itemPunkte->setFlags(Qt::ItemIsEnabled);
//        itemKennung->setFlags(Qt::ItemIsEnabled);

        row++;
    }

    ui->fragenTableWidget->resizeColumnToContents(0);
    ui->fragenTableWidget->resizeColumnToContents(1);
    ui->fragenTableWidget->resizeColumnToContents(2);

}

void FormProjekt::updateSortBox()
{
    QStringList sortList;
    sortList << "Alle";

    QMapIterator<QString, ClassProjekt> it(projektMap());
    while (it.hasNext()) {
        it.next();
        if(!sortList.contains(it.value().identifier()))
            sortList << it.value().identifier();
    }

    sortList.sort();
    ui->sortBox->clear();
    ui->sortBox->addItems(sortList);
}

QMap<QString, ClassProjekt> FormProjekt::projektMap() const
{
    return m_projektMap;
}

void FormProjekt::setProjektMap(const QMap<QString, ClassProjekt> &projektMap)
{
    m_projektMap = projektMap;
    updateSortBox();
}
