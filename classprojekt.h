#ifndef CLASSPROJEKT_H
#define CLASSPROJEKT_H

#include <QString>
#include <QMap>
#include <QDataStream>
#include <QDateTime>

#include <classfrage.h>

class ClassProjekt
{
public:
    ClassProjekt();


    QString name() const;
    void setName(const QString &name);

    QString kennung() const;
    void setKennung(const QString &kennung);

    int nr() const;
    void setNr(int nr);

    int anzahlFragen() const;
    void setAnzahlFragen(int anzahlFragen);

    int maxPunkte() const;
    void setMaxPunkte(int maxPunkte);

    int erreichtePunkte() const;
    void setErreichtePunkte(int erreichtePunkte);

    double ergebnisProzent() const;
    void setErgebnisProzent(double ergebnisProzent);

    int note() const;
    void setNote(int note);

    QMap<int, ClassFrage> fragenMap() const;
    void setFragenMap(const QMap<int, ClassFrage> &fragenMap);

    QString document() const;
    void setDocument(const QString &document);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    QString createTime() const;
    void setCreateTime(const QString &createTime);

    bool sperrfach() const;
    void setSperrfach(bool sperrfach);

private:

    QString m_name;
    QString m_kennung;
    int m_nr;
    int m_anzahlFragen;
    int m_maxPunkte;
    int m_erreichtePunkte;
    double m_ergebnisProzent;
    int m_note;
    QMap<int, ClassFrage> m_fragenMap;
    QString m_document;
    QDateTime m_dateTime;
    QString m_createTime;
    bool m_sperrfach;

};

QDataStream &operator<<(QDataStream &out, const ClassProjekt &dat);
QDataStream &operator>>(QDataStream &in, ClassProjekt &dat);

#endif // CLASSPROJEKT_H
