#include "classlehrling.h"

ClassLehrling::ClassLehrling()
{
    m_nr = 0;
    m_vorname = "";
    m_nachname = "";
    m_strasse = "";
    m_ort = "";
    m_phone = "";
    m_birthDate = QDate();
    m_apprenticeshipDate = QDate();
    m_klasse = "";
    m_company = "";
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

QString ClassLehrling::phone() const
{
    return m_phone;
}

void ClassLehrling::setPhone(const QString &phone)
{
    m_phone = phone;
}

QDate ClassLehrling::birthDate() const
{
    return m_birthDate;
}

void ClassLehrling::setBirthDate(const QDate &birthDate)
{
    m_birthDate = birthDate;
}

QDate ClassLehrling::apprenticeshipDate() const
{
    return m_apprenticeshipDate;
}

void ClassLehrling::setApprenticeshipDate(const QDate &apprenticeshipDate)
{
    m_apprenticeshipDate = apprenticeshipDate;
}

QString ClassLehrling::klasse() const
{
    return m_klasse;
}

void ClassLehrling::setKlasse(const QString &klasse)
{
    m_klasse = klasse;
}

QString ClassLehrling::company() const
{
    return m_company;
}

void ClassLehrling::setCompany(const QString &company)
{
    m_company = company;
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

QMap<QString, QVariant> ClassLehrling::getColorMap() const
{
    return m_colorMap;
}

void ClassLehrling::setColorMap(const QMap<QString, QVariant> &colorMap)
{
    m_colorMap = colorMap;
}

QColor ClassLehrling::getColor(const QString &key)
{
    QVariant var = getColorMap().value(key);
    return var.value<QColor>();
}


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
           dat.ort() << dat.phone() << dat.birthDate() << dat.apprenticeshipDate() <<
           dat.klasse() << dat.company() << dat.notiz() << dat.getSkillMap();

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
    QDate birthDate;
    QDate apprenticeshipDate;
    QString klasse;
    QString company;
    QString notiz;
    QMap<QString, ClassSkills> smap;

    in >> nr >> vorn >> nachn >> str >> ort >> tel >> birthDate >>
        apprenticeshipDate >> klasse >> company >> notiz >> smap;

    dat.setNr(nr);
    dat.setVorname(vorn);
    dat.setNachname(nachn);
    dat.setStrasse(str);
    dat.setOrt(ort);
    dat.setPhone(tel);
    dat.setBirthDate(birthDate);
    dat.setApprenticeshipDate(apprenticeshipDate);
    dat.setKlasse(klasse);
    dat.setCompany(company);
    dat.setNotiz(notiz);
    dat.setSkillMap(smap);

    return in;
}









