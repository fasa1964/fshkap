#ifndef CLASSSKILLS_H
#define CLASSSKILLS_H


#include <QString>
#include <QMap>
#include <QDateTime>
#include <QDataStream>

#include <classprojekt.h>

class ClassSkills
{
public:
    ClassSkills();

    // Evaluation criteria
    enum Criteria{ projectNode, identifierNode };

    int getNr() const;
    void setNr(int nr);

    QString name() const;
    void setName(const QString &name);

    QDate date() const;
    void setDate(const QDate &date);

    QString identifier() const;
    void setIdentifier(const QString &identifier);

    QMap<QString, ClassProjekt> getProjektMap() const;
    void setProjektMap(const QMap<QString, ClassProjekt> &value);

    bool addProjekt(const ClassProjekt &pro);
    void insertProjekt(ClassProjekt pro);
    bool removeProjekt(const ClassProjekt &pro);

    QDateTime getCreatedDate() const;
    void setCreatedDate(const QDateTime &createdDate);

    int getWert() const;
    void setWert(int wert);

    QString getKey();

    int maxPoints();
    int points();

    bool isEvaluated();

    // Evaluation criteria
    void setCriteria (Criteria criteria);
    Criteria criteria() const;

    static QStringList supportedCriteria(){
        QStringList list;
        list << "Nach Projekten" << "Nach Kennung (Fragen)";
        return list;
    }

    Criteria convert(int index);
    int index (Criteria criteria);

private:

    int m_nr;
    QString m_name;
    QString m_identifier;
    QDate m_date;
    QDateTime m_createdDate;
    int m_wert;
    Criteria m_criteria;


    QMap<QString, ClassProjekt> projektMap;


};

QDataStream &operator<<(QDataStream &out, const ClassSkills &dat);
QDataStream &operator>>(QDataStream &in, ClassSkills &dat);

#endif // CLASSSKILLS_H
