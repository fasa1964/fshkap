#include "classbetrieb.h"

ClassBetrieb::ClassBetrieb()
{
    m_nr = 0;
    m_name = "";
    m_strasse = "";
    m_ort = "";
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

QString ClassBetrieb::strasse() const
{
    return m_strasse;
}

void ClassBetrieb::setStrasse(const QString &strasse)
{
    m_strasse = strasse;
}

QString ClassBetrieb::ort() const
{
    return m_ort;
}

void ClassBetrieb::setOrt(const QString &ort)
{
    m_ort = ort;
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
    out << dat.nr() << dat.name() << dat.strasse() << dat.ort() << dat.phone() << dat.azubiMap();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassBetrieb &dat)
{
    int nr;
    QString name;
    QString strasse;
    QString ort;
    QString phone;
    QMap<QString, ClassLehrling> aMap;

    in >> nr >> name >> strasse >> ort >> phone >> aMap;

    dat.setNr( nr );
    dat.setName( name );
    dat.setStrasse( strasse );
    dat.setOrt( ort );
    dat.setPhone( phone );
    dat.setAzubiMap( aMap );

    return in;
}
