#include "classbetrieb.h"

ClassBetrieb::ClassBetrieb()
{
    m_nr = 0;
    m_name = "";
    m_strasse = "";
    m_ort = "";
    m_telefon = "";
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

QString ClassBetrieb::telefon() const
{
    return m_telefon;
}

void ClassBetrieb::setTelefon(const QString &telefon)
{
    m_telefon = telefon;
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
    out << dat.nr() << dat.name() << dat.strasse() << dat.ort() << dat.telefon() << dat.azubiMap();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassBetrieb &dat)
{
    int nr;
    QString name;
    QString strasse;
    QString ort;
    QString telefon;
    QMap<QString, ClassLehrling> aMap;

    in >> nr >> name >> strasse >> ort >> telefon >> aMap;

    dat.setNr( nr );
    dat.setName( name );
    dat.setStrasse( strasse );
    dat.setOrt( ort );
    dat.setTelefon( telefon );
    dat.setAzubiMap( aMap );

    return in;
}
