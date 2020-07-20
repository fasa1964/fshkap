#ifndef FORMLEHRLING_H
#define FORMLEHRLING_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMap>
#include <QStandardItemModel>
#include <QModelIndex>

#include <classlehrling.h>
#include <formbetriebliste.h>

namespace Ui {
class FormLehrling;
}

class FormLehrling : public QWidget
{
    Q_OBJECT

public:
    explicit FormLehrling(QWidget *parent = nullptr);
    ~FormLehrling();

    QMap<QString, ClassLehrling> lehrlingMap() const;
    void setLehrlingMap(const QMap<QString, ClassLehrling> &lehrlingMap);

    void updateLehrlingTable(const QMap<QString, ClassLehrling> &aMap);
    void setLastModified(const QDateTime &date);

    QMap<int, ClassBetrieb> betriebMap() const;
    void setBetriebMap(const QMap<int, ClassBetrieb> &betriebMap);

signals:
    void saveLehrlingMap(const QMap<QString, ClassLehrling> &azuMap);
    void updateBetriebe(const QString &betriebName, const ClassLehrling &azu);


private slots:
    void createButtonClicked();
    void deleteButtonClicked();
    void deleteSkillButtonClicked();
    void changeButtonClicked();
    void saveButtonClicked();

    void klasseBoxChanged(const QString &text);

    void slotchanged(const QModelIndex &index, const QModelIndex &);

    void azubiItemClicked(QTableWidgetItem *item);

    void openBetriebsListe();



private:
    Ui::FormLehrling *ui;

    // For the combobox
    QStandardItemModel *model;


    bool changeLehrling;
    ClassLehrling selectedLehrling;
    QMap<QString, ClassLehrling> m_lehrlingMap;

    void updateTable(QTableWidget *widget, const QList<ClassLehrling> &azuList);
    void createSortMap(const QMap<QString, ClassLehrling> &aMap);
    bool pruefnummerExist(int year, int nr);

    ClassLehrling readFromForm();
    void setLehrlingToForm(const ClassLehrling &azubi);
    void setFormTextColor(QColor color);
    void setFormReadOnly(bool status);
    void clearForm();

    QMap<int, ClassBetrieb> m_betriebMap;


    QMap<QString, QDate> klasseMap;
    void setupKlasseMap();
};

#endif // FORMLEHRLING_H
