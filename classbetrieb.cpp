#include "classbetrieb.h"

ClassBetrieb::ClassBetrieb()
{
    m_nr = 0;
    m_name = "";
    m_street = "";
    m_city = "";
    m_phone = "";
}

int ClassBetrieb::nr() const
{
    return m_nr;
}

void ClassBetrieb::setNr(int nr)
{
    m_nr = nr;
}

QString ClassBetrieb::name() const
{
    return m_name;
}

void ClassBetrieb::setName(const QString &name)
{
    m_name = name;
}

QString ClassBetrieb::street() const
{
    return m_street;
}

void ClassBetrieb::setStreet(const QString &street)
{
    m_street = street;
}

QString ClassBetrieb::city() const
{
    return m_city;
}

void ClassBetrieb::setCity(const QString &city)
{
    m_city = city;
}

QString ClassBetrieb::phone() const
{
    return m_phone;
}

void ClassBetrieb::setPhone(const QString &phone)
{
    m_phone = phone;
}

QMap<QString, ClassLehrling> ClassBetrieb::azubiMap() const
{
    return m_azubiMap;
}

void ClassBetrieb::setAzubiMap(const QMap<QString, ClassLehrling> &azubiMap)
{
    m_azubiMap = azubiMap;
}

bool ClassBetrieb::removeLehrling(const ClassLehrling &azu)
{
    bool status = false;
    QString aKey = azu.getKey();

    int result = azubiMap().remove(aKey);
    if(result != 0)
        status = true;

    return status;
}

int ClassBetrieb::countLehrling()
{
    return azubiMap().size();
}

QDataStream &operator<<(QDataStream &out, const ClassBetrieb &dat)
{
    out << dat.nr() << dat.name() << dat.street() << dat.city() << dat.phone() << dat.azubiMap();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassBetrieb &dat)
{
    int nr;
    QString name;
    QString street;
    QString city;
    QString phone;
    QMap<QString, ClassLehrling> aMap;

    in >> nr >> name >> street >> city >> phone >> aMap;

    dat.setNr( nr );
    dat.setName( name );
    dat.setStreet( street );
    dat.setCity( city );
    dat.setPhone( phone );
    dat.setAzubiMap( aMap );

    return in;
}
