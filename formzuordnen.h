#ifndef FORMZUORDNEN_H
#define FORMZUORDNEN_H

#include <QWidget>

namespace Ui {
class FormZuordnen;
}

class FormZuordnen : public QWidget
{
    Q_OBJECT

public:
    explicit FormZuordnen(QWidget *parent = nullptr);
    ~FormZuordnen();

private:
    Ui::FormZuordnen *ui;
};

#endif // FORMZUORDNEN_H
