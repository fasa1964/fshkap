#ifndef FORMBETRIEB_H
#define FORMBETRIEB_H

#include <QWidget>
#include <QMap>
#include <QTableWidgetItem>

#include <classbetrieb.h>

namespace Ui {
class FormBetrieb;
}

class FormBetrieb : public QWidget
{
    Q_OBJECT

public:
    explicit FormBetrieb(QWidget *parent = nullptr);
    ~FormBetrieb();

    QMap<int, ClassBetrieb> betriebMap() const;
    void setBetriebMap(const QMap<int, ClassBetrieb> &betriebMap);

    void updateBetriebTable(const QMap<int, ClassBetrieb> &bMap);
    void setLastModified(const QDateTime &date);

signals:
    void saveBetriebMap(const QMap<int, ClassBetrieb> &bMap);
    void betriebRemoved(const ClassBetrieb &company);
    void azubiRemoved(const ClassLehrling &azu, const ClassBetrieb &company);


private slots:
    void createButtonClicked();
    void deleteButtonClicked();
    void changeButtonClicked();
    void saveButtonClicked();
    void deleteLehrlingButtonClicked();

    void betriebTableClicked(QTableWidgetItem *item);
    void lehrlingTableClicked(QTableWidgetItem *item);

    void sortBoxTextChanged(const QString &text);

private:
    Ui::FormBetrieb *ui;

    bool changeBetrieb;
    ClassBetrieb selectedBetrieb;
    QMap<int, ClassBetrieb> m_betriebMap;

    ClassLehrling selectedLehrling;
    void updateLehrlingTable(const QMap<QString, ClassLehrling> &azubiMap);

    ClassBetrieb readFromForm();
    void setBetriebToForm(const ClassBetrieb company);
    void setFormTextColor(QColor color);
    void setFormReadOnly(bool status);
    void clearForm();
};

#endif // FORMBETRIEB_H
