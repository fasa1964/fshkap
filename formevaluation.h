#ifndef FORMEVALUATION_H
#define FORMEVALUATION_H

#include <QWidget>
#include <QTableWidgetItem>

#include <classlehrling.h>
#include <classprojekt.h>
#include <classskills.h>
#include <classfrage.h>


class LehrlingModel: public QAbstractItemModel
{
    Q_OBJECT

 public:
    int rowCount(const QModelIndex& = QModelIndex()) const { return m_sortedMap.values().size(); }
    QVariant data(const QModelIndex &index, int role) const {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
            return QString("%1").arg( m_sortedMap.keys().at( index.row() ));
        else
            return QVariant();
    }

    void setSortedMap(const QMap<QString, ClassLehrling> &sortedMap) { m_sortedMap = sortedMap; }

private:
    QMap<QString, ClassLehrling> m_sortedMap;
};

//class ItemList : public QAbstractListModel
//{
//   Q_OBJECT
//public:
//   ItemList(QObject *parent = 0) : QAbstractListModel(parent) {}

//   int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_labels.count(); }
//   QVariant data(const QModelIndex &index, int role) const {
//      if (!index.isValid())
//          return QVariant();

//      if (role == Qt::TextColorRole){
//         if(redList().contains(index.row()))
//             return QColor(255,0,0);
//         else
//             return QColor(27,0,255);
//      }

//      if (role == Qt::DisplayRole)
//          return QString("%1").arg( labels().at( index.row() ));
//      else
//          return QVariant();
//   }

//   QStringList labels() const;
//   void setLabels(const QStringList &labels);

//   QList<int> redList() const;
//   void setRedList(const QList<int> &redList);

//private:
//   QStringList m_labels;
//   QList<int> m_redList;
//};

namespace Ui {
class FormEvaluation;
class LehrlingModel;
}

class FormEvaluation : public QWidget
{
    Q_OBJECT

public:
    explicit FormEvaluation(QWidget *parent = nullptr);
    ~FormEvaluation();

     QMap<QString, ClassLehrling> azubiMap;
     void update();

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

    //LehrlingModel *model;


    bool yearExist(int year);
    void setupSortBox();
    QMap<QString, ClassLehrling> apprenticeship(int year);


    QList<ClassLehrling> getAzubiList(int year);    // sorted by year
    void setTextColor(QWidget *widget, QColor color);

























//    bool dirty;

//    ClassLehrling selectedAzubi;
//    ClassSkills selectedSkill;
//    ClassProjekt selectedProjekt;
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


//    int getSkillRow(ClassSkills skill);
//    int getProjektRow(ClassProjekt pro);
//    int getCurrentPoints();

//    QMap<int, ClassSkills> cellSkillMap;
//    QMap<int, ClassProjekt> cellProMap;

};


#endif // FORMEVALUATION_H
