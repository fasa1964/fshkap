#ifndef FORMLEHRLING_H
#define FORMLEHRLING_H

#include <QWidget>

namespace Ui {
class FormLehrling;
}

class FormLehrling : public QWidget
{
    Q_OBJECT

public:
    explicit FormLehrling(QWidget *parent = nullptr);
    ~FormLehrling();

private:
    Ui::FormLehrling *ui;
};

#endif // FORMLEHRLING_H
