#ifndef CLASSLEHRLING_H
#define CLASSLEHRLING_H

#include <QString>
#include <QDate>
#include <QDataStream>
#include <QColor>
#include <QVariant>

#include <classskills.h>

class ClassLehrling
{
public:
    ClassLehrling();

    int nr() const;
    void setNr(int nr);

    QString firstname() const;
    void setFirstname(const QString &firstname);

    QString surname() const;
    void setSurname(const QString &surname);

    QString street() const;
    void setStreet(const QString &street);

    QString city() const;
    void setCity(const QString &city);

    QString phone() const;
    void setPhone(const QString &phone);

    QDate birthDate() const;
    void setBirthDate(const QDate &birthDate);

    QDate apprenticeshipDate() const;
    void setApprenticeshipDate(const QDate &apprenticeshipDate);

    QString educationClass() const;
    void setEducationClass(const QString &educationClass);

    QString company() const;
    void setCompany(const QString &company);

    QString note() const;
    void setNote(const QString &note);

    bool isValid();
    QString getKey() const;

    QMap<QString, ClassSkills> getSkillMap() const;
    void setSkillMap(const QMap<QString, ClassSkills> &skillMap);

    bool skillContain(ClassSkills skill);
//    bool skillIsUpToDate(ClassSkills skill);

    void insertSkill(ClassSkills skill);

    QMap<QString, QVariant> getColorMap() const;
    void setColorMap(const QMap<QString, QVariant> &colorMap);

    QColor getColor(const QString &key);

private:
    int m_nr;
    QString m_firstname;
    QString m_surname;
    QString m_street;
    QString m_city;
    QString m_phone;
    QDate m_birthDate;
    QDate m_apprenticeshipDate;
    QString m_educationClass;
    QString m_company;
    QString m_note;

    QMap<QString, ClassSkills> m_skillMap;
    QMap<QString, QVariant> m_colorMap;


};

QDataStream &operator<<(QDataStream &out, const ClassLehrling &dat);
QDataStream &operator>>(QDataStream &in, ClassLehrling &dat);

#endif // CLASSLEHRLING_H
