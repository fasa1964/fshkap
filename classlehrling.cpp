#include "classlehrling.h"

ClassLehrling::ClassLehrling()
{
    m_nr = 0;
    m_firstname = "";
    m_surname = "";
    m_street = "";
    m_city = "";
    m_phone = "";
    m_birthDate = QDate();
    m_apprenticeshipDate = QDate();
    m_educationClass = "";
    m_company = "";
    m_note = "";
}

int ClassLehrling::nr() const
{
    return m_nr;
}

void ClassLehrling::setNr(int nr)
{
    m_nr = nr;
}

QString ClassLehrling::firstname() const
{
    return m_firstname;
}

void ClassLehrling::setFirstname(const QString &firstname)
{
    m_firstname = firstname;
}

QString ClassLehrling::surname() const
{
    return m_surname;
}

void ClassLehrling::setSurname(const QString &surname)
{
    m_surname = surname;
}

QString ClassLehrling::street() const
{
    return m_street;
}

void ClassLehrling::setStreet(const QString &street)
{
    m_street = street;
}

QString ClassLehrling::city() const
{
    return m_city;
}

void ClassLehrling::setCity(const QString &city)
{
    m_city = city;
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

QString ClassLehrling::educationClass() const
{
    return m_educationClass;
}

void ClassLehrling::setEducationClass(const QString &educationClass)
{
    m_educationClass = educationClass;
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
    if(nr() != 0 && !firstname().isEmpty() && surname().isEmpty())
        status = true;

    return status;
}

QString ClassLehrling::getKey() const
{
    QString key = firstname()+"."+surname()+"."+QString::number(nr(),10);
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

void ClassLehrling::insertSkill(ClassSkills skill)
{
    m_skillMap.insert(skill.getKey(), skill);
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


QString ClassLehrling::note() const
{
    return m_note;
}

void ClassLehrling::setNote(const QString &note)
{
    m_note = note;
}

QDataStream &operator<<(QDataStream &out, const ClassLehrling &dat)
{
    out << dat.nr() << dat.firstname() << dat.surname() << dat.street() <<
           dat.city() << dat.phone() << dat.birthDate() << dat.apprenticeshipDate() <<
           dat.educationClass() << dat.company() << dat.note() << dat.getSkillMap();

    return out;
}

QDataStream &operator>>(QDataStream &in, ClassLehrling &dat)
{

    int nr;
    QString fn;
    QString sn;
    QString str;
    QString city;
    QString ph;
    QDate birthDate;
    QDate apprenticeshipDate;
    QString educationClass;
    QString company;
    QString note;
    QMap<QString, ClassSkills> smap;

    in >> nr >> fn >> sn >> str >> city >> ph >> birthDate >>
        apprenticeshipDate >> educationClass >> company >> note >> smap;

    dat.setNr(nr);
    dat.setFirstname(fn);
    dat.setSurname(sn);
    dat.setStreet(str);
    dat.setCity(city);
    dat.setPhone(ph);
    dat.setBirthDate(birthDate);
    dat.setApprenticeshipDate(apprenticeshipDate);
    dat.setEducationClass(educationClass);
    dat.setCompany(company);
    dat.setNote(note);
    dat.setSkillMap(smap);

    return in;
}









