#include "classskills.h"

ClassSkills::ClassSkills()
{
    m_nr = 0;
    m_name = "";
    m_identifier = "";
    m_wert = 0;
    m_date = QDate();
    m_criteria = Criteria::projectNode;
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

QString ClassSkills::identifier() const
{
    return m_identifier;
}

void ClassSkills::setIdentifier(const QString &identifier)
{
    m_identifier = identifier;
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
    QString key = pro.name()+"."+pro.identifier();
    if(projektMap.keys().contains(key))
        return false;
    else
        projektMap.insert(key, pro);

    return true;
}

void ClassSkills::insertProjekt( ClassProjekt pro)
{
    projektMap.insert(pro.getKey(), pro);
}



bool ClassSkills::removeProjekt(const ClassProjekt &pro)
{
    QString key = pro.name()+"."+pro.identifier();
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
    QString key = name()+"."+identifier();
    return key;
}

int ClassSkills::maxPoints()
{
    int maxpunkte = 0;
    QMapIterator<QString, ClassProjekt> it(projektMap);
    while (it.hasNext()){
         it.next();
         maxpunkte = maxpunkte + it.value().maxPoints();
    }

    return maxpunkte;
}

int ClassSkills::points()
{
    int punkte = 0;
    QMapIterator<QString, ClassProjekt> it(projektMap);
    while (it.hasNext()){
         it.next();
         punkte = punkte + it.value().points();
    }

    return punkte;
}

bool ClassSkills::isEvaluated()
{
    bool status = false;

    QMapIterator<QString, ClassProjekt> it(projektMap);
    while (it.hasNext()) {
        it.next();
        if(it.value().getEvaluated()){
            status = true;
            break;
        }
    }

    return status;
}

void ClassSkills::setCriteria(ClassSkills::Criteria criteria)
{
    m_criteria = criteria;
}

ClassSkills::Criteria ClassSkills::criteria() const
{
    return m_criteria;
}

ClassSkills::Criteria ClassSkills::convert(int index)
{
    Criteria cr;
    if(index == 0)
        cr = Criteria::projectNode;

    if(index == 1)
        cr = Criteria::identifierNode;

    return cr;
}

int ClassSkills::index(ClassSkills::Criteria criteria)
{
    int in = -1;

    if(criteria == Criteria::projectNode)
        in = 0;
    if(criteria == Criteria::identifierNode)
        in = 1;

    return in;
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
    out << dat.getNr() << dat.name() << dat.identifier() << dat.date() << dat.getWert() <<
           dat.getCreatedDate() << dat.getProjektMap() << dat.criteria();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassSkills &dat)
{
    int nr;
    QString name;
    QString identifier;
    QDate date;
    QDateTime createdDate;
    int wert;
    QMap<QString, ClassProjekt> pMap;
    int criteriaIndex;

    in >> nr >> name >> identifier >> date >> wert >> createdDate >> pMap >> criteriaIndex;

    dat.setNr( nr );
    dat.setName( name );
    dat.setIdentifier( identifier );
    dat.setWert( wert );
    dat.setDate( date );
    dat.setCreatedDate( createdDate );
    dat.setProjektMap( pMap );
    dat.setCriteria(dat.convert(criteriaIndex));

    return in;
}
