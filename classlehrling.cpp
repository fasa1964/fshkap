#include "classlehrling.h"

ClassLehrling::ClassLehrling()
{
    m_nr = 0;
    m_vorname = "";
    m_nachname = "";
    m_strasse = "";
    m_ort = "";
    m_telefon = "";
    m_gebDatum = QDate();
    m_ausbildungsDatum = QDate();
    m_klasse = "";
    m_betrieb = "";
    m_notiz = "";
}

int ClassLehrling::nr() const
{
    return m_nr;
}

void ClassLehrling::setNr(int nr)
{
    m_nr = nr;
}

QString ClassLehrling::vorname() const
{
    return m_vorname;
}

void ClassLehrling::setVorname(const QString &vorname)
{
    m_vorname = vorname;
}

QString ClassLehrling::nachname() const
{
    return m_nachname;
}

void ClassLehrling::setNachname(const QString &nachname)
{
    m_nachname = nachname;
}

QString ClassLehrling::strasse() const
{
    return m_strasse;
}

void ClassLehrling::setStrasse(const QString &strasse)
{
    m_strasse = strasse;
}

QString ClassLehrling::ort() const
{
    return m_ort;
}

void ClassLehrling::setOrt(const QString &ort)
{
    m_ort = ort;
}

QString ClassLehrling::telefon() const
{
    return m_telefon;
}

void ClassLehrling::setTelefon(const QString &telefon)
{
    m_telefon = telefon;
}

QDate ClassLehrling::gebDatum() const
{
    return m_gebDatum;
}

void ClassLehrling::setGebDatum(const QDate &gebDatum)
{
    m_gebDatum = gebDatum;
}

QDate ClassLehrling::ausbildungsDatum() const
{
    return m_ausbildungsDatum;
}

void ClassLehrling::setAusbildungsDatum(const QDate &ausbildungsDatum)
{
    m_ausbildungsDatum = ausbildungsDatum;
}

QString ClassLehrling::klasse() const
{
    return m_klasse;
}

void ClassLehrling::setKlasse(const QString &klasse)
{
    m_klasse = klasse;
}

QString ClassLehrling::betrieb() const
{
    return m_betrieb;
}

void ClassLehrling::setBetrieb(const QString &betrieb)
{
    m_betrieb = betrieb;
}

bool ClassLehrling::isValid()
{
    bool status = false;
    if(nr() != 0 && !vorname().isEmpty() && nachname().isEmpty())
        status = true;

    return status;
}

QString ClassLehrling::getKey() const
{
    QString key = vorname()+"."+nachname()+"."+QString::number(nr(),10);
    return key;
}

QMap<QString, ClassSkills> ClassLehrling::getSkillMap() const
{
    return m_skillMap;
}

void ClassLehrling::setSkillMap(const QMap<QString, ClassSkills> &skillMap)
{
    m_skillMap = skillMap;
}

bool ClassLehrling::skillContain(ClassSkills skill)
{
    QMapIterator<QString, ClassSkills> it(getSkillMap());
    while (it.hasNext()) {
        it.next();
        ClassSkills s = it.value();
        if(s.getKey() == skill.getKey())
            return true;
    }

    return false;
}

//bool ClassLehrling::skillIsUpToDate(ClassSkills skill)
//{
//    QMapIterator<QString, ClassSkills> it(getSkillMap());
//    while (it.hasNext()) {
//        it.next();
//        ClassSkills s = it.value();
//        if(s.getKey() == skill.getKey()){
//            if(skill.date())
//            return true;
//        }
//    }

//    return false;
//}

QString ClassLehrling::notiz() const
{
    return m_notiz;
}

void ClassLehrling::setNotiz(const QString &notiz)
{
    m_notiz = notiz;
}

QDataStream &operator<<(QDataStream &out, const ClassLehrling &dat)
{
    out << dat.nr() << dat.vorname() << dat.nachname() << dat.strasse() <<
           dat.ort() << dat.telefon() << dat.gebDatum() << dat.ausbildungsDatum() <<
           dat.klasse() << dat.betrieb() << dat.notiz() << dat.getSkillMap();

    return out;
}

QDataStream &operator>>(QDataStream &in, ClassLehrling &dat)
{

    int nr;
    QString vorn;
    QString nachn;
    QString str;
    QString ort;
    QString tel;
    QDate gebDatum;
    QDate ausbildungsDatum;
    QString klasse;
    QString betrieb;
    QString notiz;
    QMap<QString, ClassSkills> smap;

    in >> nr >> vorn >> nachn >> str >> ort >> tel >> gebDatum >>
        ausbildungsDatum >> klasse >> betrieb >> notiz >> smap;

    dat.setNr(nr);
    dat.setVorname(vorn);
    dat.setNachname(nachn);
    dat.setStrasse(str);
    dat.setOrt(ort);
    dat.setTelefon(tel);
    dat.setGebDatum(gebDatum);
    dat.setAusbildungsDatum(ausbildungsDatum);
    dat.setKlasse(klasse);
    dat.setBetrieb(betrieb);
    dat.setNotiz(notiz);
    dat.setSkillMap(smap);

    return in;
}









