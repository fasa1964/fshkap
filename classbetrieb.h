#ifndef CLASSBETRIEB_H
#define CLASSBETRIEB_H

#include <QString>
#include <QDataStream>
#include <QMap>

#include <classlehrling.h>

class ClassBetrieb
{
public:
    ClassBetrieb();

    int nr() const;
    void setNr(int nr);

    QString name() const;
    void setName(const QString &name);

    QString street() const;
    void setStreet(const QString &street);

     QString city() const;
     void setCity(const QString &city);

     QString phone() const;
     void setPhone(const QString &phone);

     QMap<QString, ClassLehrling> azubiMap() const;
     void setAzubiMap(const QMap<QString, ClassLehrling> &azubiMap);

     bool removeLehrling(const ClassLehrling &azu);
     int countLehrling();

private:
     int m_nr;
     QString m_name;
     QString m_street;
     QString m_city;
     QString m_phone;

     QMap<QString, ClassLehrling> m_azubiMap;

};

QDataStream &operator<<(QDataStream &out, const ClassBetrieb &dat);
QDataStream &operator>>(QDataStream &in, ClassBetrieb &dat);

#endif // CLASSBETRIEB_H
