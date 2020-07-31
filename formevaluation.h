#ifndef FORMEVALUATION_H
#define FORMEVALUATION_H

#include <QWidget>
#include <QTableWidgetItem>

#include <classlehrling.h>
#include <classprojekt.h>
#include <classskills.h>
#include <classfrage.h>


class ItemList : public QAbstractListModel
{
   Q_OBJECT
public:
   ItemList(QObject *parent = 0) : QAbstractListModel(parent) {}

   int rowCount(const QModelIndex &parent = QModelIndex()) const { return 5; }
   QVariant data(const QModelIndex &index, int role) const {
      if (!index.isValid())
          return QVariant();

      if (role == Qt::TextColorRole)
         return QColor(QColor::colorNames().at(index.row()));

      if (role == Qt::DisplayRole)
          return QString("Item %1").arg(index.row() + 1);
      else
          return QVariant();
   }
};

namespace Ui {
class FormEvaluation;
class ItemList;
}

class FormEvaluation : public QWidget
{
    Q_OBJECT

public:
    explicit FormEvaluation(QWidget *parent = nullptr);
    ~FormEvaluation();

     QMap<QString, ClassLehrling> azubiMap;
     void update();

//    QMap<QString, ClassLehrling> azubiMap() const;
//    void setAzubiMap( QMap<QString, ClassLehrling> azubiMap);

private slots:
    void azubiSortBoxChanged(const QString &text);
    void azubiListBoxChanged(const QString &text);
//    void skillListBoxChanged(const QString &text);
//    void projektListBoxChanged(const QString &text);

//    void fragenTableCellChanged(int, int);
//    void cellItemClicked(int row, int column);
//    void cellItemChanged(int row, int column);

private:
    Ui::FormEvaluation *ui;



//    bool dirty;

//    ClassLehrling selectedAzubi;
//    ClassSkills selectedSkill;
//    ClassProjekt selectedProjekt;

    bool yearExist(int year);
    void setupSortBox();

    QList<ClassLehrling> getAzubiList(int year);    // sorted by year
//    QMap<QString, ClassLehrling> m_azubiMap;
    //QMap<QString, ClassLehrling> workingAzubiMap;

//    QMap<QString, ClassSkills> currentSkillMap;
//    QMap<QString, ClassProjekt> currentProjektMap;

//    QList<ClassSkills> getSkillList(const ClassLehrling &azu);
//    QList<ClassProjekt> getProjektList(const ClassSkills &skill);

//    void setupFragenTable(ClassProjekt pro);
//    void updateAzubi(int points, double percent);

//    void sortAzubi(const QMap<QString, ClassLehrling> &aMap);
//    QStringList getAzubiKeys(int jahrgang);

//    void setupFragenTable(const ClassProjekt &pro);
//    void setupErgebnisTable(const ClassLehrling &azu);
    void setTextColor(QWidget *widget, QColor color);

//    int getSkillRow(ClassSkills skill);
//    int getProjektRow(ClassProjekt pro);
//    int getCurrentPoints();

//    QMap<int, ClassSkills> cellSkillMap;
//    QMap<int, ClassProjekt> cellProMap;

};

#endif // FORMEVALUATION_H
