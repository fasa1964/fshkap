#ifndef CLASSFRAGE_H
#define CLASSFRAGE_H

#include <QString>
#include <QDataStream>

class ClassFrage
{
public:
    ClassFrage();

    int questionNr() const;
    void setQuestionNr(int questionNr);

    QString identifier() const;
    void setIdentifier(const QString &identifier);

    QString question() const;
    void setQuestion(const QString &question);

    int maxPoints() const;
    void setMaxPoints(int maxPoints);

    int points() const;
    void setPoints(int points);

private:
    int m_questionNr;
    QString m_identifier;
    QString m_question;
    int m_maxPoints;
    int m_points;

};

QDataStream &operator<<(QDataStream &out, const ClassFrage &dat);
QDataStream &operator>>(QDataStream &in, ClassFrage &dat);

#endif // CLASSFRAGE_H
