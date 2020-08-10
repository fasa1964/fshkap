#ifndef CLASSPROJEKT_H
#define CLASSPROJEKT_H

#include <QString>
#include <QMap>
#include <QDataStream>
#include <QDateTime>

#include <classfrage.h>

class ClassProjekt
{
public:
    ClassProjekt();


    QString name() const;
    void setName(const QString &name);

    QString identifier() const;
    void setIdentifier(const QString &identifier);

    int nr() const;
    void setNr(int nr);

    int countQuestion() const;
    void setCountQuestion(int countQuestion);

    int maxPoints() const;
    void setMaxPoints(int maxPoints);

    int points() const;
    void setPoints(int points);

    double percent() const;
    void setPercent(double percent);

    int note() const;
    void setNote(int note);

    QMap<int, ClassFrage> questionMap() const;
    void setQuestionMap(const QMap<int, ClassFrage> &questionMap);

    QString document() const;
    void setDocument(const QString &document);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    QString createTime() const;
    void setCreateTime(const QString &createTime);

    bool lockSubject() const;
    void setLockSubject(bool lockSubject);

    QString getKey();

    bool getEvaluated() const;
    void setEvaluated(bool evaluated);

    void setQuestionPoints(int nr, int points);

    double getFactor() const;
    void setFactor(double factor);

    int getDuration() const;
    void setDuration(int duration);

private:

    QString m_name;
    QString m_identifier;
    int m_nr;
    int m_countQuestion;
    int m_maxPoints;
    int m_points;
    double m_percent;
    int m_note;
    QMap<int, ClassFrage> m_questionMap;
    QString m_document;
    QDateTime m_dateTime;
    QString m_createTime;
    bool m_lockSubject;
    bool m_evaluated;
    double m_factor;
    int m_duration;     // in minutes

};

QDataStream &operator<<(QDataStream &out, const ClassProjekt &dat);
QDataStream &operator>>(QDataStream &in, ClassProjekt &dat);

#endif // CLASSPROJEKT_H
