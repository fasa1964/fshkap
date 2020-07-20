#ifndef FORMUPDATEINFORMATION_H
#define FORMUPDATEINFORMATION_H

#include <QWidget>
#include <QMap>

#include <classskills.h>
#include <classlehrling.h>

namespace Ui {
class FormUpdateInformation;
}

class FormUpdateInformation : public QWidget
{
    Q_OBJECT

public:
    explicit FormUpdateInformation(const QMap<QString, ClassSkills> &skillMap, const QMap<QString, ClassLehrling> &azuMap, QWidget *parent = nullptr);
    ~FormUpdateInformation();


private:
    Ui::FormUpdateInformation *ui;

    void updateSkillTable(const QMap<QString, ClassSkills> &sMap);
    void updateAzubiTable(const QMap<QString, ClassLehrling> &lMap);
};

#endif // FORMUPDATEINFORMATION_H
