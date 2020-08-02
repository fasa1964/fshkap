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

    QString vorname() const;
    void setVorname(const QString &vorname);

    QString nachname() const;
    void setNachname(const QString &nachname);

    QString strasse() const;
    void setStrasse(const QString &strasse);

    QString ort() const;
    void setOrt(const QString &ort);

    QString phone() const;
    void setPhone(const QString &phone);

    QDate birthDate() const;
    void setBirthDate(const QDate &birthDate);

    QDate apprenticeshipDate() const;
    void setApprenticeshipDate(const QDate &apprenticeshipDate);

    QString klasse() const;
    void setKlasse(const QString &klasse);

    QString company() const;
    void setCompany(const QString &company);

    QString notiz() const;
    void setNotiz(const QString &notiz);

    bool isValid();
    QString getKey() const;

    QMap<QString, ClassSkills> getSkillMap() const;
    void setSkillMap(const QMap<QString, ClassSkills> &skillMap);

    bool skillContain(ClassSkills skill);
//    bool skillIsUpToDate(ClassSkills skill);



    QMap<QString, QVariant> getColorMap() const;
    void setColorMap(const QMap<QString, QVariant> &colorMap);

    QColor getColor(const QString &key);

private:
    int m_nr;
    QString m_vorname;
    QString m_nachname;
    QString m_strasse;
    QString m_ort;
    QString m_phone;
    QDate m_birthDate;
    QDate m_apprenticeshipDate;
    QString m_klasse;
    QString m_company;
    QString m_notiz;

    QMap<QString, ClassSkills> m_skillMap;
    QMap<QString, QVariant> m_colorMap;


};

QDataStream &operator<<(QDataStream &out, const ClassLehrling &dat);
QDataStream &operator>>(QDataStream &in, ClassLehrling &dat);

#endif // CLASSLEHRLING_H
