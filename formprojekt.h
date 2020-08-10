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

    void updateProjektTable(const QMap<QString, ClassProjekt> &proMap);

    QMap<QString, ClassProjekt> projektMap() const;
    void setProjektMap(const QMap<QString, ClassProjekt> &projektMap);

signals:
    void saveProjekte(const QMap<QString, ClassProjekt> &proMap);
    void projektChanged(const ClassProjekt &projekt);
    void projektAdded(const ClassProjekt &projekt);
    void invalidDateTime();

private slots:
    void closeForm();
    void createButtonClicked();
    void saveButtonClicked();
    void changeButtonClicked();
    void deleteButtonClicked();
    void openFileButtonClicked();

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
    void setColorTableFragen(QColor color);
    void setFormTextColor(QColor color);

    void setProjektToForm(const ClassProjekt &pro);
    void clearForm();
    void updateFragenTable(const QMap<int, ClassFrage> &fMap);
    void updateSortBox();

};

#endif // FORMPROJEKT_H
