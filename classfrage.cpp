#include "classfrage.h"

ClassFrage::ClassFrage()
{
    m_questionNr = 0;
    m_identifier = "";
    m_question = "";
    m_maxPoints = 0;
    m_points = 0;
}

int ClassFrage::questionNr() const
{
    return m_questionNr;
}

void ClassFrage::setQuestionNr(int questionNr)
{
    m_questionNr = questionNr;
}

QString ClassFrage::identifier() const
{
    return m_identifier;
}

void ClassFrage::setIdentifier(const QString &identifier)
{
    m_identifier = identifier;
}

QString ClassFrage::question() const
{
    return m_question;
}

void ClassFrage::setQuestion(const QString &question)
{
    m_question = question;
}

int ClassFrage::maxPoints() const
{
    return m_maxPoints;
}

void ClassFrage::setMaxPoints(int maxPoints)
{
    m_maxPoints = maxPoints;
}

int ClassFrage::points() const
{
    return m_points;
}

void ClassFrage::setPoints(int points)
{
    m_points = points;
}

QDataStream &operator<<(QDataStream &out, const ClassFrage &dat)
{
    out << dat.questionNr() << dat.identifier() << dat.question() << dat.maxPoints() << dat.points();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassFrage &dat)
{
    int questionNr;
    QString identifier;
    QString question;
    int maxPoints;
    int points;

    in >> questionNr >> identifier >> question >> maxPoints >> points;

    dat.setQuestionNr(questionNr);
    dat.setIdentifier(identifier);
    dat.setMaxPoints(maxPoints);
    dat.setQuestion(question);
    dat.setPoints(points);
    return  in;
}
