#ifndef FORMBETRIEBLISTE_H
#define FORMBETRIEBLISTE_H

#include <QWidget>
#include <QDialog>
#include <QTableWidgetItem>
#include <QMap>
#include <classbetrieb.h>

namespace Ui {
class FormBetriebListe;
}

class FormBetriebListe : public QDialog
{
    Q_OBJECT

public:
    explicit FormBetriebListe(QWidget *parent = nullptr);
    ~FormBetriebListe();

    void updateTable(const QMap<int, ClassBetrieb> &map);
    QString verify();


private slots:
    void tableClicked(QTableWidgetItem *item);
    void rejectForm();

private:
    Ui::FormBetriebListe *ui;

    void writeSettings();
    void readSettings();

};

#endif // FORMBETRIEBLISTE_H
