#ifndef FORMBETRIEB_H
#define FORMBETRIEB_H

#include <QWidget>

namespace Ui {
class FormBetrieb;
}

class FormBetrieb : public QWidget
{
    Q_OBJECT

public:
    explicit FormBetrieb(QWidget *parent = nullptr);
    ~FormBetrieb();

private:
    Ui::FormBetrieb *ui;
};

#endif // FORMBETRIEB_H
