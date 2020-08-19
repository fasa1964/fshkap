#ifndef FORMEVALUATION_H
#define FORMEVALUATION_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

#include <classlehrling.h>
#include <classprojekt.h>
#include <classskills.h>
#include <classfrage.h>


namespace Ui {
class FormEvaluation;
}

class FormEvaluation : public QWidget
{
    Q_OBJECT

public:
    explicit FormEvaluation(QWidget *parent = nullptr);
    ~FormEvaluation();

     void updateSortBox();

     QMap<QString, ClassLehrling> azubiMap() const;
     void setAzubiMap(const QMap<QString, ClassLehrling> &azubiMap);

signals:
     void saveAzubiMap(QMap<QString, ClassLehrling> azuMap);

private slots:
     void closeButtonClicked();
     void saveButtonClicked();
     void sortBoxTextChanged(const QString &text);
     void azubiBoxTextChanged(const QString &text);
     void skillBoxTextChanged(const QString &text);
     void projectBoxTextChanged(const QString &text);

     void evaluatedCheckBoxChanged(int status);
     void questionTableCellChanged(int row, int column);
     void resultTableItemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::FormEvaluation *ui;

    bool dirty;
    ClassLehrling selectedLehrling;
    ClassSkills selectedSkill;
    ClassProjekt selectedProjekt;

    QMap<QString, ClassLehrling> m_azubiMap;

    void setupProjectValue();
    void setupQuestionTable(ClassProjekt pro);
    void updateSkillBox(const ClassLehrling &azu);
    QMap<QString, ClassLehrling> apprenticeship(int year);
    void setTextColor(QWidget *widget, QColor color);

    // Test
    void setupResultWidget(const ClassLehrling &azu);
    void setupIdentifier(const ClassLehrling &azu);


    QStringList questionsIdentifierList(const ClassLehrling &azu);

    QList<ClassFrage> getQuestions(const ClassProjekt &pro, const QString &key);
    double getResultIdentifier(const QList<ClassFrage> questList);


    double getProjectPercent(const ClassProjekt &pro);
    int getProjectMaxPoint(const ClassProjekt &pro);
    int getProjectPoints(const ClassProjekt &pro);


};


#endif // FORMEVALUATION_H
