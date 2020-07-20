#include "classprojekt.h"

ClassProjekt::ClassProjekt()
{
    m_name = "";
    m_kennung = "";
    m_nr = 0;
    m_anzahlFragen = 0;
    m_maxPunkte = 0;
    m_erreichtePunkte = 0;
    m_ergebnisProzent = 0.0;
    m_note = 0;
    m_document = "";
    m_createTime = "";
    m_sperrfach = false;
    m_ausgewertet = false;
}

QString ClassProjekt::name() const
{
    return m_name;
}

void ClassProjekt::setName(const QString &name)
{
    m_name = name;
}

QString ClassProjekt::kennung() const
{
    return m_kennung;
}

void ClassProjekt::setKennung(const QString &kennung)
{
    m_kennung = kennung;
}

int ClassProjekt::nr() const
{
    return m_nr;
}

void ClassProjekt::setNr(int nr)
{
    m_nr = nr;
}

int ClassProjekt::anzahlFragen() const
{
    return m_anzahlFragen;
}

void ClassProjekt::setAnzahlFragen(int anzahlFragen)
{
    m_anzahlFragen = anzahlFragen;
}

int ClassProjekt::maxPunkte() const
{
    return m_maxPunkte;
}

void ClassProjekt::setMaxPunkte(int maxPunkte)
{
    m_maxPunkte = maxPunkte;
}

int ClassProjekt::erreichtePunkte() const
{
    return m_erreichtePunkte;
}

void ClassProjekt::setErreichtePunkte(int erreichtePunkte)
{
    m_erreichtePunkte = erreichtePunkte;
}

double ClassProjekt::ergebnisProzent() const
{
    return m_ergebnisProzent;
}

void ClassProjekt::setErgebnisProzent(double ergebnisProzent)
{
    m_ergebnisProzent = ergebnisProzent;
}

int ClassProjekt::note() const
{
    return m_note;
}

void ClassProjekt::setNote(int note)
{
    m_note = note;
}

QMap<int, ClassFrage> ClassProjekt::fragenMap() const
{
    return m_fragenMap;
}

void ClassProjekt::setFragenMap(const QMap<int, ClassFrage> &fragenMap)
{
    m_fragenMap = fragenMap;
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

bool ClassProjekt::sperrfach() const
{
    return m_sperrfach;
}

void ClassProjekt::setSperrfach(bool sperrfach)
{
    m_sperrfach = sperrfach;
}

QString ClassProjekt::getKey()
{
    QString key;
    key = name()+"."+kennung();
    return key;
}

bool ClassProjekt::getAusgewertet() const
{
    return m_ausgewertet;
}

void ClassProjekt::setAusgewertet(bool ausgewertet)
{
    m_ausgewertet = ausgewertet;
}


QDataStream &operator<<(QDataStream &out, const ClassProjekt &dat)
{
    out << dat.nr() << dat.name() << dat.kennung() << dat.anzahlFragen() << dat.maxPunkte() <<
           dat.erreichtePunkte() << dat.ergebnisProzent() << dat.note() << dat.fragenMap() <<
           dat.document() << dat.dateTime() << dat.createTime() << dat.sperrfach() << dat.getAusgewertet();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassProjekt &dat)
{
    QString name;
    QString kennung;
    int nr;
    int anzahlFragen;
    int maxPunkte;
    int erreichtePunkte;
    double ergebnisProzent;
    int note;
    QMap<int, ClassFrage> frgMap;
    QString doc;
    QString dts;
    QDateTime dt;
    bool sperr;
    bool ausgewertet;

    in >> nr >> name >> kennung >> anzahlFragen >> maxPunkte >> erreichtePunkte
       >> ergebnisProzent >> note >> frgMap >> doc >> dt >> dts >> sperr >> ausgewertet;


    dat.setNr(nr);
    dat.setName(name);
    dat.setKennung(kennung);
    dat.setAnzahlFragen(anzahlFragen);
    dat.setMaxPunkte(maxPunkte);
    dat.setErreichtePunkte(erreichtePunkte);
    dat.setErgebnisProzent(ergebnisProzent);
    dat.setNote(note);
    dat.setFragenMap(frgMap);
    dat.setDocument(doc);
    dat.setDateTime(dt);
    dat.setCreateTime(dts);
    dat.setSperrfach(sperr);
    dat.setAusgewertet(ausgewertet);

    return in;
}



