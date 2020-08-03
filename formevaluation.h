#ifndef FORMEVALUATION_H
#define FORMEVALUATION_H

#include <QWidget>
#include <QTableWidgetItem>

#include <classlehrling.h>
#include <classprojekt.h>
#include <classskills.h>
#include <classfrage.h>


class LehrlingModel: public QAbstractListModel
{
    Q_OBJECT

 public:
    LehrlingModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_sortedMap.values().size(); }
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
//class LehrlingModel;
}

class FormEvaluation : public QWidget
{
    Q_OBJECT

public:
    explicit FormEvaluation(QWidget *parent = nullptr);
    ~FormEvaluation();

     QMap<QString, ClassLehrling> azubiMap;
     void updateSortBox();

private slots:
    void sortBoxTextChanged(const QString &text);
    void azubiBoxTextChanged(const QString &text);

private:
    Ui::FormEvaluation *ui;

    ClassLehrling selectedLehrling;

    QMap<QString, ClassLehrling> apprenticeship(int year);
    void setTextColor(QWidget *widget, QColor color);
};


#endif // FORMEVALUATION_H
