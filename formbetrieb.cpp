#include "formbetrieb.h"
#include "ui_formbetrieb.h"

#include <QMessageBox>
#include <QDebug>

FormBetrieb::FormBetrieb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBetrieb)
{
    ui->setupUi(this);
    setFormReadOnly(true);

    changeBetrieb = false;
    selectedBetrieb = ClassBetrieb();
    selectedLehrling = ClassLehrling();

    QStringList labels;
    labels << "Alle" << "Betriebe mit Auszubildenden";
    ui->sortBox->addItems(labels);

    connect(ui->closeButton, &QPushButton::clicked, this, &FormBetrieb::close);
    connect(ui->deleteButton, &QPushButton::clicked, this, &FormBetrieb::deleteButtonClicked);
    connect(ui->createButton, &QPushButton::clicked, this, &FormBetrieb::createButtonClicked);
    connect(ui->changeButton, &QPushButton::clicked, this, &FormBetrieb::changeButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &FormBetrieb::saveButtonClicked);
    connect(ui->deleteLehrlingButton, &QPushButton::clicked, this, &FormBetrieb::deleteLehrlingButtonClicked);

    connect(ui->betriebeTableWidget, &QTableWidget::itemClicked , this, &FormBetrieb::betriebTableClicked);
    connect(ui->lehrlingTableWidget, &QTableWidget::itemClicked , this, &FormBetrieb::lehrlingTableClicked);

    connect(ui->sortBox, &QComboBox::currentTextChanged , this, &FormBetrieb::sortBoxTextChanged);
}

FormBetrieb::~FormBetrieb()
{
    delete ui;
}

void FormBetrieb::createButtonClicked()
{
    setFormReadOnly(false);
    clearForm();
    ui->nrBox->setValue( betriebMap().size()+1 );
    ui->nameEdit->setFocus();

    ui->saveButton->setEnabled(true);
    ui->createButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->changeButton->setEnabled(false);
    ui->deleteLehrlingButton->setEnabled(false);
}

void FormBetrieb::deleteButtonClicked()
{
    int result = QMessageBox::question(this, tr("Löschen Betrieb"), tr("Soll das Löschen unwiderruflich fortgesetz werden?"),
                                       QMessageBox::Yes | QMessageBox::Abort);
    if(result == QMessageBox::Abort)
        return;

    m_betriebMap.remove(selectedBetrieb.nr());
    emit betriebRemoved(selectedBetrieb);
    updateBetriebTable(m_betriebMap);

    saveBetriebMap(betriebMap());
    clearForm();
    selectedBetrieb = ClassBetrieb();

}

void FormBetrieb::changeButtonClicked()
{
    changeBetrieb = true;
    setFormReadOnly(false);
    ui->nameEdit->setFocus();

    ui->changeButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->createButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
}

void FormBetrieb::saveButtonClicked()
{
    ClassBetrieb company = readFromForm();

    if(company.name().isEmpty() || company.city().isEmpty() || company.street().isEmpty()){
        QMessageBox::information(this, tr("Betrieb speichern"), tr("Der Name, Straße und Ort vom "
                                         "Betrieb müssen angegeben werden!"));
        return;
    }

    if(changeBetrieb && selectedBetrieb.azubiMap().size() > 0)
        company.setAzubiMap( selectedBetrieb.azubiMap());


    m_betriebMap.insert(company.nr(), company);
    updateBetriebTable(m_betriebMap);
    emit saveBetriebMap(m_betriebMap);

    selectedBetrieb = company;
    changeBetrieb = false;
    setFormReadOnly(true);

    ui->saveButton->setEnabled(false);
    ui->createButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->changeButton->setEnabled(true);
    ui->deleteLehrlingButton->setEnabled(false);
}

void FormBetrieb::deleteLehrlingButtonClicked()
{

    QString vn = selectedLehrling.firstname()+ ","+ selectedLehrling.surname();

    int result = QMessageBox::information(this, tr("Löschen Lehrling"), tr("Der Auszubildende : ")+vn+"\n"+
                                          tr("wird nur vom Betrieb entfernt!"),
                                       QMessageBox::Yes | QMessageBox::Abort);
    if(result == QMessageBox::Abort)
        return;

    QMap<QString, ClassLehrling> aMap;
    aMap = selectedBetrieb.azubiMap();
    aMap.remove(selectedLehrling.getKey());
    selectedBetrieb.setAzubiMap(aMap);
    m_betriebMap.insert(selectedBetrieb.nr(), selectedBetrieb);

    emit azubiRemoved(selectedLehrling, selectedBetrieb);
    emit saveBetriebMap(betriebMap());
    updateLehrlingTable(selectedBetrieb.azubiMap());
}

void FormBetrieb::betriebTableClicked(QTableWidgetItem *item)
{
    int row = item->row();
    int nr = ui->betriebeTableWidget->item(row, 0)->text().toInt();
    selectedBetrieb = m_betriebMap.value(nr);
    changeBetrieb = false;

    setBetriebToForm(selectedBetrieb);
    setFormReadOnly(true);

    updateLehrlingTable(selectedBetrieb.azubiMap());
    selectedLehrling = ClassLehrling();

    ui->saveButton->setEnabled(false);
    ui->createButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->changeButton->setEnabled(true);
    ui->deleteLehrlingButton->setEnabled(false);

}

void FormBetrieb::lehrlingTableClicked(QTableWidgetItem *item)
{
    if(selectedBetrieb.nr() == 0)
        return;

    if(!changeBetrieb)
        return;

    int row = item->row();
    QString key = ui->lehrlingTableWidget->item(row, 0)->text();
    selectedLehrling = selectedBetrieb.azubiMap().value(key);

    ui->deleteLehrlingButton->setEnabled(true);
}

void FormBetrieb::sortBoxTextChanged(const QString &text)
{
    if(text == "Alle"){
        updateBetriebTable(betriebMap());
        setBetriebToForm(betriebMap().values().first());
    }

    if(text == "Betriebe mit Auszubildenden"){
        QMap<int, ClassBetrieb> sortMap;
        QMapIterator<int, ClassBetrieb> it(betriebMap());
        while (it.hasNext()) {
            it.next();
            ClassBetrieb betrieb = it.value();
            if(!betrieb.azubiMap().isEmpty())
                sortMap.insert(betrieb.nr(), betrieb);

        }

        if(!sortMap.isEmpty()){
            updateBetriebTable(sortMap);
            setBetriebToForm(sortMap.values().first());
        }
    }
}

void FormBetrieb::updateLehrlingTable(const QMap<QString, ClassLehrling> &azubiMap)
{
    ui->lehrlingTableWidget->clear();
    ui->lehrlingTableWidget->setColumnCount(2);
    ui->lehrlingTableWidget->setRowCount(azubiMap.size());

    QStringList headers;
    headers << "Kennung" << "Klasse";
    ui->lehrlingTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    QMapIterator<QString, ClassLehrling> it(azubiMap);
    while (it.hasNext()) {
       it.next();

       QTableWidgetItem *itemKennung = new QTableWidgetItem(it.value().getKey());
       QTableWidgetItem *itemKlasse = new QTableWidgetItem(it.value().educationClass());

       ui->lehrlingTableWidget->setItem(row,0, itemKennung);
       ui->lehrlingTableWidget->setItem(row,1, itemKlasse);

       row++;
    }

    ui->lehrlingTableWidget->resizeColumnToContents(0);
    ui->lehrlingTableWidget->resizeColumnToContents(1);
}

ClassBetrieb FormBetrieb::readFromForm()
{
    ClassBetrieb company;
    company.setNr(ui->nrBox->value());
    company.setName(ui->nameEdit->text());
    company.setCity(ui->ortEdit->text());
    company.setStreet(ui->strasseEdit->text());
    company.setPhone(ui->phoneEdit->text());

    return company;
}

void FormBetrieb::setBetriebToForm(const ClassBetrieb company)
{
    ui->nrBox->setValue(company.nr());
    ui->nameEdit->setText(company.name());
    ui->strasseEdit->setText(company.street());
    ui->ortEdit->setText(company.city());
    ui->phoneEdit->setText(company.phone());

    if(!company.azubiMap().isEmpty())
        updateLehrlingTable(company.azubiMap());

}

void FormBetrieb::setFormTextColor(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    palette.setColor(QPalette::Text,color);

    ui->nrBox->setPalette(palette);
    ui->nameEdit->setPalette(palette);
    ui->strasseEdit->setPalette(palette);
    ui->ortEdit->setPalette(palette);
    ui->phoneEdit->setPalette(palette);

}

QMap<int, ClassBetrieb> FormBetrieb::betriebMap() const
{
    return m_betriebMap;
}

void FormBetrieb::setBetriebMap(const QMap<int, ClassBetrieb> &betriebMap)
{
    m_betriebMap = betriebMap;

    if(!betriebMap.isEmpty())
        setBetriebToForm(betriebMap.values().first());

}

void FormBetrieb::updateBetriebTable(const QMap<int, ClassBetrieb> &bMap)
{
    ui->betriebeTableWidget->clear();
    ui->betriebeTableWidget->setColumnCount(2);
    ui->betriebeTableWidget->setRowCount(bMap.size());

    QStringList headers;
    headers << "Nr" << "Name";
    ui->betriebeTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    QMapIterator<int, ClassBetrieb> it(bMap);
    while (it.hasNext()) {
       it.next();
       QString nrs = QString::number(it.key(),10);

       QTableWidgetItem *itemNr = new QTableWidgetItem(nrs);
       QTableWidgetItem *itemName = new QTableWidgetItem(it.value().name());

       ui->betriebeTableWidget->setItem(row,0, itemNr);
       ui->betriebeTableWidget->setItem(row,1, itemName);

       row++;
    }

    ui->betriebeTableWidget->resizeColumnToContents(0);
    ui->betriebeTableWidget->resizeColumnToContents(1);
}

void FormBetrieb::setLastModified(const QDateTime &date)
{
    ui->lastChangeEdit->setDateTime(date);
}

void FormBetrieb::setFormReadOnly(bool status)
{
    ui->nameEdit->setReadOnly(status);
    ui->strasseEdit->setReadOnly(status);
    ui->ortEdit->setReadOnly(status);
    ui->phoneEdit->setReadOnly(status);

    if(!status)
        setFormTextColor(QColor(0,85,127));
    else
        setFormTextColor(Qt::black);

    //ui->lehrlingTableWidget->setEnabled(!status);
}

void FormBetrieb::clearForm()
{
    ui->nrBox->setValue(0);
    ui->nameEdit->clear();
    ui->strasseEdit->clear();
    ui->ortEdit->clear();
    ui->phoneEdit->clear();
    ui->lehrlingTableWidget->clear();
}
