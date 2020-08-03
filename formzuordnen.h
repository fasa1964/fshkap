#ifndef FORMZUORDNEN_H
#define FORMZUORDNEN_H

#include <QWidget>
#include <QTableWidgetItem>

#include <classlehrling.h>
#include <classskills.h>

namespace Ui {
class FormZuordnen;
}

class FormZuordnen : public QWidget
{
    Q_OBJECT

public:
    explicit FormZuordnen(QWidget *parent = nullptr);
    ~FormZuordnen();

    QMap<QString, ClassLehrling> azubiMap() const;
    void setAzubiMap(const QMap<QString, ClassLehrling> &azubiMap);

    QMap<QString, ClassSkills> skillMap() const;
    void setSkillMap(const QMap<QString, ClassSkills> &skillMap);

    void setupAzuSortBox();
    void setupSkillSortBox();

signals:
    void saveAzubiMap(const QMap<QString, ClassLehrling> &aubiMap);

private slots:
    void importSkillButtonClicked();
    void deleteSkillButtonClicked();
    void azubiSortBoxCurrentTextChanged(const QString &text);
    void skillSortBoxCurrentTextChanged(const QString &text);

    void azubiTableClicked(QTableWidgetItem *item);

private:
    Ui::FormZuordnen *ui;

    ClassLehrling selectedAzubi;
    QMap<QString, ClassLehrling> m_azubiMap;
    QMap<QString, ClassSkills> m_skillMap;

    QMap<int, QString> azuIndexMap;

    void updateSkillTable(const QMap<QString, ClassSkills> &sMap);
    void updateAzubiTable(const QMap<QString, ClassLehrling> &azuMap);


    QMap<QString, ClassLehrling> getSelectedAzubi();
    QMap<QString, ClassLehrling> getLehrjahrMap(int jahrgang);
    QMap<QString, ClassSkills> getSkillMap(const QString &identifier);
};

#endif // FORMZUORDNEN_H
