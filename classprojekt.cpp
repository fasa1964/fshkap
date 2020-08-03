#include "classprojekt.h"

ClassProjekt::ClassProjekt()
{
    m_name = "";
    m_identifier = "";
    m_nr = 0;
    m_countQuestion = 0;
    m_maxPoints = 0;
    m_points = 0;
    m_percent = 0.0;
    m_note = 0;
    m_document = "";
    m_createTime = "";
    m_lockSubject = false;
    m_evaluated = false;
}

QString ClassProjekt::name() const
{
    return m_name;
}

void ClassProjekt::setName(const QString &name)
{
    m_name = name;
}

QString ClassProjekt::identifier() const
{
    return m_identifier;
}

void ClassProjekt::setIdentifier(const QString &identifier)
{
    m_identifier = identifier;
}

int ClassProjekt::nr() const
{
    return m_nr;
}

void ClassProjekt::setNr(int nr)
{
    m_nr = nr;
}

int ClassProjekt::countQuestion() const
{
    return m_countQuestion;
}

void ClassProjekt::setCountQuestion(int countQuestion)
{
    m_countQuestion = countQuestion;
}

int ClassProjekt::maxPoints() const
{
    return m_maxPoints;
}

void ClassProjekt::setMaxPoints(int maxPoints)
{
    m_maxPoints = maxPoints;
}

int ClassProjekt::points() const
{
    return m_points;
}

void ClassProjekt::setPoints(int points)
{
    m_points = points;
}

double ClassProjekt::percent() const
{
    return m_percent;
}

void ClassProjekt::setPercent(double percent)
{
    m_percent = percent;
}

int ClassProjekt::note() const
{
    return m_note;
}

void ClassProjekt::setNote(int note)
{
    m_note = note;
}

QMap<int, ClassFrage> ClassProjekt::questionMap() const
{
    return m_questionMap;
}

void ClassProjekt::setQuestionMap(const QMap<int, ClassFrage> &questionMap)
{
    m_questionMap = questionMap;
}

QString ClassProjekt::document() const
{
    return m_document;
}

void ClassProjekt::setDocument(const QString &document)
{
    m_document = document;
}

QDateTime ClassProjekt::dateTime() const
{
    return m_dateTime;
}

void ClassProjekt::setDateTime(const QDateTime &dateTime)
{
    m_dateTime = dateTime;
}

QString ClassProjekt::createTime() const
{
    return m_createTime;
}

void ClassProjekt::setCreateTime(const QString &createTime)
{
    m_createTime = createTime;
}

bool ClassProjekt::lockSubject() const
{
    return m_lockSubject;
}

void ClassProjekt::setLockSubject(bool lockSubject)
{
    m_lockSubject = lockSubject;
}

QString ClassProjekt::getKey()
{
    QString key;
    key = name()+"."+identifier();
    return key;
}

bool ClassProjekt::getEvaluated() const
{
    return m_evaluated;
}

void ClassProjekt::setEvaluated(bool evaluated)
{
    m_evaluated = evaluated;
}

void ClassProjekt::setQuestionPoints(int nr, int points)
{
    ClassFrage frg = m_questionMap.value(nr);
    frg.setPoints(points);
    m_questionMap.insert(nr, frg);
}


QDataStream &operator<<(QDataStream &out, const ClassProjekt &dat)
{
    out << dat.nr() << dat.name() << dat.identifier() << dat.countQuestion() << dat.maxPoints() <<
           dat.points() << dat.percent() << dat.note() << dat.questionMap() <<
           dat.document() << dat.dateTime() << dat.createTime() << dat.lockSubject() << dat.getEvaluated();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassProjekt &dat)
{
    QString name;
    QString identifier;
    int nr;
    int countQuestion;
    int maxPoints;
    int points;
    double percent;
    int note;
    QMap<int, ClassFrage> frgMap;
    QString doc;
    QString dts;
    QDateTime dt;
    bool sperr;
    bool evaluated;

    in >> nr >> name >> identifier >> countQuestion >> maxPoints >> points
       >> percent >> note >> frgMap >> doc >> dt >> dts >> sperr >> evaluated;


    dat.setNr(nr);
    dat.setName(name);
    dat.setIdentifier(identifier);
    dat.setCountQuestion(countQuestion);
    dat.setMaxPoints(maxPoints);
    dat.setPoints(points);
    dat.setPercent(percent);
    dat.setNote(note);
    dat.setQuestionMap(frgMap);
    dat.setDocument(doc);
    dat.setDateTime(dt);
    dat.setCreateTime(dts);
    dat.setLockSubject(sperr);
    dat.setEvaluated(evaluated);

    return in;
}



