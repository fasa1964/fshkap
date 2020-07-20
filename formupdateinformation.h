#ifndef FORMUPDATEINFORMATION_H
#define FORMUPDATEINFORMATION_H

#include <QWidget>

namespace Ui {
class FormUpdateInformation;
}

class FormUpdateInformation : public QWidget
{
    Q_OBJECT

public:
    explicit FormUpdateInformation(QWidget *parent = nullptr);
    ~FormUpdateInformation();

private:
    Ui::FormUpdateInformation *ui;
};

#endif // FORMUPDATEINFORMATION_H
