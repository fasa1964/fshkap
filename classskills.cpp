#include "classskills.h"

ClassSkills::ClassSkills()
{
    m_nr = 0;
    m_name = "";
    m_kennung = "";
    m_wert = 0;
    m_date = QDate();
}

QString ClassSkills::name() const
{
    return m_name;
}

void ClassSkills::setName(const QString &name)
{
    m_name = name;
}

QDate ClassSkills::date() const
{
    return m_date;
}

void ClassSkills::setDate(const QDate &date)
{
    m_date = date;
}

QString ClassSkills::kennung() const
{
    return m_kennung;
}

void ClassSkills::setKennung(const QString &kennung)
{
    m_kennung = kennung;
}

QMap<QString, ClassProjekt> ClassSkills::getProjektMap() const
{
    return projektMap;
}

void ClassSkills::setProjektMap(const QMap<QString, ClassProjekt> &value)
{
    projektMap = value;
}

bool ClassSkills::addProjekt(const ClassProjekt &pro)
{
    QString key = pro.name()+"."+pro.kennung();
    if(projektMap.keys().contains(key))
        return false;
    else
        projektMap.insert(key, pro);

    return true;
}

bool ClassSkills::removeProjekt(const ClassProjekt &pro)
{
    QString key = pro.name()+"."+pro.kennung();
    int result = projektMap.remove(key);

    if(result == 0)
        return false;

    return true;
}

QDateTime ClassSkills::getCreatedDate() const
{
    return m_createdDate;
}

void ClassSkills::setCreatedDate(const QDateTime &createdDate)
{
    m_createdDate = createdDate;
}

int ClassSkills::getWert() const
{
    return m_wert;
}

void ClassSkills::setWert(int wert)
{
    m_wert = wert;
}

QString ClassSkills::getKey()
{
    QString key = name()+"."+kennung();
    return key;
}

int ClassSkills::maxPunkte()
{
    int maxpunkte = 0;
    QMapIterator<QString, ClassProjekt> it(projektMap);
    while (it.hasNext()){
         it.next();
         maxpunkte = maxpunkte + it.value().maxPunkte();
    }

    return maxpunkte;
}

int ClassSkills::erreichtePunkte()
{
    int punkte = 0;
    QMapIterator<QString, ClassProjekt> it(projektMap);
    while (it.hasNext()){
         it.next();
         punkte = punkte + it.value().erreichtePunkte();
    }

    return punkte;
}

int ClassSkills::getNr() const
{
    return m_nr;
}

void ClassSkills::setNr(int nr)
{
    m_nr = nr;
}

QDataStream &operator<<(QDataStream &out, const ClassSkills &dat)
{
    out << dat.getNr() << dat.name() << dat.kennung() << dat.date() << dat.getWert() <<
           dat.getCreatedDate() << dat.getProjektMap();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassSkills &dat)
{
    int nr;
    QString name;
    QString kennung;
    QDate date;
    QDateTime createdDate;
    int wert;
    QMap<QString, ClassProjekt> pMap;

    in >> nr >> name >> kennung >> date >> wert >> createdDate >> pMap;

    dat.setNr( nr );
    dat.setName( name );
    dat.setKennung( kennung );
    dat.setWert( wert );
    dat.setDate( date );
    dat.setCreatedDate( createdDate );
    dat.setProjektMap( pMap );

    return in;
}
