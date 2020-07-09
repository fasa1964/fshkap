#ifndef FORMPROJEKT_H
#define FORMPROJEKT_H

#include <QWidget>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <classfrage.h>
#include <classprojekt.h>

namespace Ui {
class FormProjekt;
}

class FormProjekt : public QWidget
{
    Q_OBJECT

public:
    explicit FormProjekt(QWidget *parent = nullptr);
    ~FormProjekt();

    void updateProjektListe();

    QMap<QString, ClassProjekt> projektMap() const;
    void setProjektMap(const QMap<QString, ClassProjekt> &projektMap);

signals:
    void saveProjekte(const QMap<QString, ClassProjekt> &proMap);
    void invalidDateTime();

private slots:
    void closeForm();
    void createButtonClicked();
    void saveButtonClicked();
    void changeButtonClicked();
    void deleteButtonClicked();
    void removeFrageClicked();
    void openFileButtonClicked();

    void addFrageClicked();
    void addFrageButtonClicked();
    void removeFrageButtonClicked();
    void anzahlFragenChanged(int value);
    void projekteTableClicked(QTableWidgetItem *item);

    void sortBoxTextChanged(const QString &text);
private:
    Ui::FormProjekt *ui;
    bool changeProjekt;

    ClassProjekt selectedProjekt;
    QMap<QString, ClassProjekt> m_projektMap;

    void setFormReadOnly(bool status);
    ClassProjekt readFromForm();

    void clearTableFragen();
    void editableTableFragen(bool);
    void setColorTableFragen(QColor color);

    void setProjektToForm(const ClassProjekt &pro);
    void clearForm();
    void updateFragenTable(const QMap<int, ClassFrage> &fMap);
    void updateProjektTable(const QMap<QString, ClassProjekt> &pMap);

    QString createKey(const ClassProjekt &pro);
};

#endif // FORMPROJEKT_H
