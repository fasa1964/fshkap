#ifndef FORMSKILLS_H
#define FORMSKILLS_H

#include <QWidget>

namespace Ui {
class FormSkills;
}

class FormSkills : public QWidget
{
    Q_OBJECT

public:
    explicit FormSkills(QWidget *parent = nullptr);
    ~FormSkills();

private:
    Ui::FormSkills *ui;
};

#endif // FORMSKILLS_H
