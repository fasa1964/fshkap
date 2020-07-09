#ifndef CLASSFRAGE_H
#define CLASSFRAGE_H

#include <QString>
#include <QDataStream>

class ClassFrage
{
public:
    ClassFrage();

    int frageNr() const;
    void setFrageNr(int frageNr);

    QString kennung() const;
    void setKennung(const QString &kennung);

    QString frage() const;
    void setFrage(const QString &frage);

    int maxPunkte() const;
    void setMaxPunkte(int maxPunkte);

    int erreichtePunkte() const;
    void setErreichtePunkte(int erreichtePunkte);

private:
    int m_frageNr;
    QString m_kennung;
    QString m_frage;
    int m_maxPunkte;
    int m_erreichtePunkte;

};

QDataStream &operator<<(QDataStream &out, const ClassFrage &dat);
QDataStream &operator>>(QDataStream &in, ClassFrage &dat);

#endif // CLASSFRAGE_H
