#include "formbetriebliste.h"
#include "ui_formbetriebliste.h"

#include <QMapIterator>
#include <QTableWidgetItem>
#include <QSettings>

FormBetriebListe::FormBetriebListe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormBetriebListe)
{
    ui->setupUi(this);
    setWindowTitle(tr("Liste Betriebe"));
    readSettings();

    connect(ui->assumeButton, &QPushButton::clicked, this, &FormBetriebListe::verify);
    connect(ui->cancelButton, &QPushButton::clicked, this, &FormBetriebListe::rejectForm);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &FormBetriebListe::tableClicked);
}

FormBetriebListe::~FormBetriebListe()
{
    delete ui;
}

void FormBetriebListe::updateTable(const QMap<int, ClassBetrieb> &map)
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(map.size());

    QStringList labels;
    labels << "Nr." << "Name";
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    int row = 0;
    QMapIterator<int, ClassBetrieb> it( map );
    while (it.hasNext())
    {
        it.next();
        QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number( it.value().nr(),10));
        QTableWidgetItem *itemName = new QTableWidgetItem( it.value().name() );

        ui->tableWidget->setItem(row, 0, itemNr);
        ui->tableWidget->setItem(row, 1, itemName);

        row++;
    }

    ui->tableWidget->resizeColumnsToContents();
}

QString FormBetriebListe::verify()
{
    writeSettings();
    accept();
    return ui->nameEdit->text();
}

void FormBetriebListe::tableClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString name = ui->tableWidget->item(row,1)->text();
    ui->nameEdit->setText( name );

    if(!name.isEmpty())
        ui->assumeButton->setEnabled(true);
    else
        ui->assumeButton->setEnabled(false);

}

void FormBetriebListe::rejectForm()
{
    writeSettings();
    reject();
}

void FormBetriebListe::writeSettings()
{
    QSettings settings("FaSa-Soft", "FormBetriebListe");
    settings.setValue("formgeometrie", this->geometry());
    settings.setValue("colw0", ui->tableWidget->columnWidth(0) );
    settings.setValue("colw1", ui->tableWidget->columnWidth(1) );
}

void FormBetriebListe::readSettings()
{
    QSettings settings("FaSa-Soft", "FormBetriebListe");
    QRect rect = settings.value("formgeometrie", QRect(0,0,400,480)).toRect();
    setGeometry(rect);

    int w0 = settings.value("colw0", 40).toInt();
    int w1 = settings.value("colw1", 200).toInt();
    ui->tableWidget->setColumnWidth(0, w0);
    ui->tableWidget->setColumnWidth(1, w1);

}
