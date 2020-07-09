#include "classfrage.h"

ClassFrage::ClassFrage()
{
    m_frageNr = 0;
    m_kennung = "";
    m_frage = "";
    m_maxPunkte = 0;
    m_erreichtePunkte = 0;
}

int ClassFrage::frageNr() const
{
    return m_frageNr;
}

void ClassFrage::setFrageNr(int frageNr)
{
    m_frageNr = frageNr;
}

QString ClassFrage::kennung() const
{
    return m_kennung;
}

void ClassFrage::setKennung(const QString &kennung)
{
    m_kennung = kennung;
}

QString ClassFrage::frage() const
{
    return m_frage;
}

void ClassFrage::setFrage(const QString &frage)
{
    m_frage = frage;
}

int ClassFrage::maxPunkte() const
{
    return m_maxPunkte;
}

void ClassFrage::setMaxPunkte(int maxPunkte)
{
    m_maxPunkte = maxPunkte;
}

int ClassFrage::erreichtePunkte() const
{
    return m_erreichtePunkte;
}

void ClassFrage::setErreichtePunkte(int erreichtePunkte)
{
    m_erreichtePunkte = erreichtePunkte;
}

QDataStream &operator<<(QDataStream &out, const ClassFrage &dat)
{
    out << dat.frageNr() << dat.kennung() << dat.frage() << dat.maxPunkte() << dat.erreichtePunkte();
    return out;
}

QDataStream &operator>>(QDataStream &in, ClassFrage &dat)
{
    int frageNr;
    QString kennung;
    QString frage;
    int maxPunkte;
    int erreichtePunkte;

    in >> frageNr >> kennung >> frage >> maxPunkte >> erreichtePunkte;

    dat.setFrageNr(frageNr);
    dat.setKennung(kennung);
    dat.setMaxPunkte(maxPunkte);
    dat.setFrage(frage);
    dat.setErreichtePunkte(erreichtePunkte);
    return  in;
}
