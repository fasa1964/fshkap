#include "formzuordnen.h"
#include "ui_formzuordnen.h"

#include <QDebug>

FormZuordnen::FormZuordnen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormZuordnen)
{
    ui->setupUi(this);

    selectedAzubi = ClassLehrling();

    azuIndexMap.insert(1, "1.Lehrjahr");
    azuIndexMap.insert(2, "2.Lehrjahr");
    azuIndexMap.insert(3, "3.Lehrjahr");
    azuIndexMap.insert(4, "4.Lehrjahr");
    azuIndexMap.insert(5, "Nachholer");


    connect(ui->closeButton, &QPushButton::clicked, this, &FormZuordnen::close);
    connect(ui->azubiSortBox , &QComboBox::currentTextChanged, this, &FormZuordnen::azubiSortBoxCurrentTextChanged);
    connect(ui->skillSortBox , &QComboBox::currentTextChanged, this, &FormZuordnen::skillSortBoxCurrentTextChanged);
    connect(ui->importSkillButton , &QPushButton::clicked, this, &FormZuordnen::importSkillButtonClicked);
    connect(ui->deleteSkillButton , &QPushButton::clicked, this, &FormZuordnen::deleteSkillButtonClicked);

    connect(ui->azubiTable, &QTableWidget::itemClicked, this, &FormZuordnen::azubiTableClicked);

}

FormZuordnen::~FormZuordnen()
{
    delete ui;
}

QMap<QString, ClassLehrling> FormZuordnen::azubiMap() const
{
    return m_azubiMap;
}

void FormZuordnen::setAzubiMap(const QMap<QString, ClassLehrling> &azubiMap)
{
    m_azubiMap = azubiMap;
    updateAzubiTable(azubiMap);
}

QMap<QString, ClassSkills> FormZuordnen::skillMap() const
{
    return m_skillMap;
}

void FormZuordnen::setSkillMap(const QMap<QString, ClassSkills> &skillMap)
{
    m_skillMap = skillMap;
    updateSkillTable(skillMap);
}

void FormZuordnen::azubiSortBoxCurrentTextChanged(const QString &text)
{
    if(text == "Alle"){
        updateAzubiTable(azubiMap());
        ui->importSkillButton->setEnabled(false);
        return;
    }

    int jahr = azuIndexMap.key(text);
    updateAzubiTable(getLehrjahrMap(jahr));

    if(ui->skillSortBox->currentText() != "Alle")
        ui->importSkillButton->setEnabled(true);
}

void FormZuordnen::skillSortBoxCurrentTextChanged(const QString &text)
{
    if(text == "Alle"){
        updateSkillTable(skillMap());
        ui->importSkillButton->setEnabled(false);
        return;
    }

    updateSkillTable(getSkillMap(text));
    if(ui->azubiSortBox->currentText() != "Alle")
        ui->importSkillButton->setEnabled(true);

}

void FormZuordnen::azubiTableClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString key = ui->azubiTable->item(row,1)->text()+"."+ui->azubiTable->item(row,0)->text();
    selectedAzubi = azubiMap().value(key);

    if(selectedAzubi.getSkillMap().size() > 0)
        ui->deleteSkillButton->setEnabled(true);
    else
        ui->deleteSkillButton->setEnabled(false);


}

void FormZuordnen::updateSkillTable(const QMap<QString, ClassSkills> &sMap)
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
        QTableWidgetItem *itemKennung = new QTableWidgetItem( skill.kennung() );

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

void FormZuordnen::updateAzubiTable(const QMap<QString, ClassLehrling> &azuMap)
{
    ui->azubiTable->clear();
    ui->azubiTable->setColumnCount(6);
    ui->azubiTable->setRowCount(azuMap.size());

    QFont tableFont = ui->azubiTable->font();

    QStringList headers;
    headers << "Nr." << "Name" << "Klasse" << "Betrieb" << "AP" << "Marker";
    ui->azubiTable->setHorizontalHeaderLabels(headers);

    int row = 0;
    QMapIterator<QString, ClassLehrling> it(azuMap);
    while (it.hasNext()) {
       it.next();

       QString ap = "";
       if(it.value().getSkillMap().size() > 0){

           foreach(ClassSkills skill, it.value().getSkillMap().values()){
                ap.append(skill.getKey());
                ap.append("\n");
           }
       }

       QTableWidgetItem *itemNr = new QTableWidgetItem( QString::number(it.value().nr(),10) );
       QTableWidgetItem *itemName = new QTableWidgetItem(it.value().vorname()+"."+it.value().nachname());
       QTableWidgetItem *itemKlasse = new QTableWidgetItem(it.value().klasse());
       QTableWidgetItem *itemBetrieb = new QTableWidgetItem(it.value().betrieb());
       QTableWidgetItem *itemAP = new QTableWidgetItem(ap);
       QTableWidgetItem *itemMarker = new QTableWidgetItem();

       ui->azubiTable->setItem(row,0, itemNr);
       ui->azubiTable->setItem(row,1, itemName);
       ui->azubiTable->setItem(row,2, itemKlasse);
       ui->azubiTable->setItem(row,3, itemBetrieb);
       ui->azubiTable->setItem(row,4, itemAP);
       ui->azubiTable->setItem(row,5, itemMarker);

       itemNr->setTextColor(QColor(0,85,127,255));
       itemName->setTextColor(QColor(0,85,127,255));
       itemBetrieb->setSizeHint(QSize(100,30));
       if(ui->azubiSortBox->currentText() != "Alle")
           itemMarker->setCheckState(Qt::Checked);
       else
           itemMarker->setCheckState(Qt::Unchecked);

       itemAP->setData(Qt::FontRole, QFont(tableFont.family(),8));
       itemAP->setToolTip(tr("Zugeordente AP's"));

//       itemNr->setFlags(Qt::ItemIsEnabled);
//       itemName->setFlags(Qt::ItemIsEnabled);
//       itemKlasse->setFlags(Qt::ItemIsEnabled);
//       itemBetrieb->setFlags(Qt::ItemIsEnabled);

       row++;
    }

    ui->azubiTable->resizeColumnToContents(0);
    ui->azubiTable->resizeColumnToContents(1);
    ui->azubiTable->resizeColumnToContents(2);
    ui->azubiTable->resizeColumnToContents(4);
    ui->azubiTable->resizeColumnToContents(5);
    ui->azubiTable->resizeRowsToContents();
    //ui->azubiTable->resizeColumnToContents(4);

//    ui->azubiTable->setWordWrap(true);
    //    ui->azubiTable->update();
}

QMap<QString, ClassLehrling> FormZuordnen::getSelectedAzubi()
{
    QMap<QString, ClassLehrling> amap;

    int rows = ui->azubiTable->rowCount();
    for(int i = 0; i < rows; i++)
    {
        if(ui->azubiTable->item(i,5)->checkState() == Qt::Checked)
        {
             QString key = ui->azubiTable->item(i,1)->text()+"."+ui->azubiTable->item(i,0)->text();
             ClassLehrling azu = m_azubiMap.value(key);
             amap.insert(key, azu);

        }
    }

    return amap;

}

void FormZuordnen::setupSkillSortBox()
{
    QStringList sortList;
    QMapIterator<QString, ClassSkills> it(skillMap());
    while (it.hasNext()) {
        it.next();
        if(!sortList.contains(it.value().kennung()))
            sortList << it.value().kennung();
    }

    sortList << "Alle";
    ui->skillSortBox->clear();
    ui->skillSortBox->addItems(sortList);
}

void FormZuordnen::importSkillButtonClicked()
{
    QMap<QString, ClassLehrling> aMap;
    aMap = getLehrjahrMap( azuIndexMap.key(ui->azubiSortBox->currentText()));

    QMap<QString, ClassSkills> sMap = getSkillMap(ui->skillSortBox->currentText());

    QMapIterator<QString, ClassLehrling> it(aMap);
    while (it.hasNext()) {
        it.next();
        ClassLehrling azu = it.value();
        QMap<QString, ClassSkills> skillMap;
        skillMap = azu.getSkillMap();

        foreach(ClassSkills skill, sMap.values()){
            skillMap.insert(skill.getKey(), skill);
        }

        azu.setSkillMap(skillMap);
        m_azubiMap.insert(azu.getKey(), azu);
    }

    emit saveAzubiMap(azubiMap());
    azubiSortBoxCurrentTextChanged(ui->azubiSortBox->currentText());
}

void FormZuordnen::deleteSkillButtonClicked()
{
    QMap<QString, ClassLehrling> map;
    map = getSelectedAzubi();
    QMapIterator<QString, ClassLehrling> it(map);
    while (it.hasNext()) {
        it.next();
        ClassLehrling azubi = it.value();
        QMap<QString, ClassSkills> smap;
        azubi.setSkillMap(smap);
        m_azubiMap.insert(azubi.getKey(), azubi);
    }

    setAzubiMap(m_azubiMap);
    emit saveAzubiMap(m_azubiMap);
}

void FormZuordnen::setupAzuSortBox()
{

    QStringList sortList;

    for(int i = 1; i < 6; i++)
    {
        if(getLehrjahrMap(i).size() > 0)
            sortList << azuIndexMap.value(i);
    }

    sortList.sort();
    sortList << "Alle";

    ui->azubiSortBox->clear();
    ui->azubiSortBox->addItems(sortList);
}

QMap<QString, ClassLehrling> FormZuordnen::getLehrjahrMap(int jahrgang)
{
    QDate today = QDate::currentDate();
    QMap<QString, ClassLehrling> map;

    QMapIterator<QString, ClassLehrling> it(azubiMap());
    while (it.hasNext()) {
        it.next();
        int year = today.year() - it.value().ausbildungsDatum().year();

        if(year <= 0)
            year = 1;

        if(year >= 6)
            year = 5;

        if(year == jahrgang)
            map.insert(it.value().getKey(), it.value());
    }

    return map;
}

QMap<QString, ClassSkills> FormZuordnen::getSkillMap(const QString &kennung)
{
    QMap<QString, ClassSkills> map;
    QMapIterator<QString, ClassSkills> it(skillMap());
    while (it.hasNext()) {
        it.next();
        if(it.value().kennung() == kennung){
            ClassSkills skill = it.value();
            map.insert(skill.getKey(), skill);
        }
    }

    return map;
}













