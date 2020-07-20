#ifndef FORMEVALUATION_H
#define FORMEVALUATION_H

#include <QWidget>

namespace Ui {
class FormEvaluation;
}

class FormEvaluation : public QWidget
{
    Q_OBJECT

public:
    explicit FormEvaluation(QWidget *parent = nullptr);
    ~FormEvaluation();

private:
    Ui::FormEvaluation *ui;
};

#endif // FORMEVALUATION_H
