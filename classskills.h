#ifndef CLASSSKILLS_H
#define CLASSSKILLS_H


#include <QString>
#include <QMap>
#include <QDateTime>
#include <QDataStream>

#include <classprojekt.h>

class ClassSkills
{
public:
    ClassSkills();

    int getNr() const;
    void setNr(int nr);

    QString name() const;
    void setName(const QString &name);

    QDate date() const;
    void setDate(const QDate &date);

    QString kennung() const;
    void setKennung(const QString &kennung);

    QMap<QString, ClassProjekt> getProjektMap() const;
    void setProjektMap(const QMap<QString, ClassProjekt> &value);

    bool addProjekt(const ClassProjekt &pro);
    bool removeProjekt(const ClassProjekt &pro);

    QDateTime getCreatedDate() const;
    void setCreatedDate(const QDateTime &createdDate);

    int getWert() const;
    void setWert(int wert);

    QString getKey();

    int maxPunkte();
    int erreichtePunkte();

private:

    int m_nr;
    QString m_name;
    QString m_kennung;
    QDate m_date;
    QDateTime m_createdDate;
    int m_wert;

    QMap<QString, ClassProjekt> projektMap;


};

QDataStream &operator<<(QDataStream &out, const ClassSkills &dat);
QDataStream &operator>>(QDataStream &in, ClassSkills &dat);

#endif // CLASSSKILLS_H
