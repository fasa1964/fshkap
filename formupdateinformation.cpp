#include "formupdateinformation.h"
#include "ui_formupdateinformation.h"

#include <QTableWidgetItem>

FormUpdateInformation::FormUpdateInformation(const QMap<QString, ClassSkills> &skillMap, const QMap<QString, ClassLehrling> &azuMap, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormUpdateInformation)
{
    ui->setupUi(this);

    updateSkillTable(skillMap);
    updateAzubiTable(azuMap);
    connect(ui->okButton, &QPushButton::clicked, this, &FormUpdateInformation::close);

}

FormUpdateInformation::~FormUpdateInformation()
{
    delete ui;
}

void FormUpdateInformation::updateSkillTable(const QMap<QString, ClassSkills> &sMap)
{
    ui->skillTableWidget->setRowCount(sMap.size());
    ui->skillTableWidget->setColumnCount(1);

    int row = 0;
    QMapIterator<QString, ClassSkills> it(sMap);
    while (it.hasNext()) {
        it.next();
        ClassSkills skill = it.value();
        QTableWidgetItem *itemName = new QTableWidgetItem(skill.getKey());
        ui->skillTableWidget->setItem(row, 0, itemName);
        row++;
    }

    ui->skillTableWidget->resizeColumnToContents(0);
}

void FormUpdateInformation::updateAzubiTable(const QMap<QString, ClassLehrling> &lMap)
{
    ui->azubiTableWidget->setRowCount(lMap.size());
    ui->azubiTableWidget->setColumnCount(1);

    int row = 0;
    QMapIterator<QString, ClassLehrling> it(lMap);
    while (it.hasNext()) {
        it.next();
        ClassLehrling azubi = it.value();
        QTableWidgetItem *itemName = new QTableWidgetItem(azubi.getKey());
        ui->azubiTableWidget->setItem(row, 0, itemName);
        row++;
    }

    ui->azubiTableWidget->resizeColumnToContents(0);
}

