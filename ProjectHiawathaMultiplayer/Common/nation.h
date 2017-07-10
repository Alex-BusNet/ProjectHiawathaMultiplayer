#ifndef Nation_H
#define Nation_H

#include <QString>


enum Nation
{
    America, Germany, India, China, Mongolia, Aztec, France, Iroquois, Greece, Rome, Russia, England, Egypt, Japan, Persia, Arabia, Random, NO_NATION
};

static QString NationName(Nation nation)
{
    switch(nation)
    {
    case America:
        return QString("United States of America");
    case Germany:
        return QString("Germany");
    case India:
        return QString("India");
    case China:
        return QString("China");
    case Mongolia:
        return QString("Mongolia");
    case Aztec:
        return QString("Aztec");
    case France:
        return QString("France");
    case Iroquois:
        return QString("Iroquois");
    case Greece:
        return QString("Greece");
    case Rome:
        return QString("Rome");
    case England:
        return QString("England");
    case Arabia:
        return QString("Arabia");
    case Persia:
        return QString("Persia");
    case Russia:
        return QString("Russia");
    case Japan:
        return QString("Japan");
    case Egypt:
        return QString("Egypt");
    case Random:
        return QString("Random");
    default:
        return QString("No Nation");
    }
}

static Nation GetNationEnum(QString nation)
{
    if(nation == QString("United States of America"))
        return America;
    else if(nation == QString("Germany"))
        return Germany;
    else if(nation == QString("India"))
        return India;
    else if(nation == QString("China"))
        return China;
    else if(nation == QString("Mongolia"))
        return Mongolia;
    else if(nation == QString("Aztec"))
        return Aztec;
    else if(nation == QString("France"))
        return France;
    else if(nation == QString("Iroquois"))
        return Iroquois;
    else if(nation == QString("Greece"))
        return Greece;
    else if(nation == QString("Rome"))
        return Rome;
    else if(nation == QString("England"))
        return England;
    else if(nation == QString("Arabia"))
        return Arabia;
    else if(nation == QString("Persia"))
        return Persia;
    else if(nation == QString("Russia"))
        return Russia;
    else if(nation == QString("Japan"))
        return Japan;
    else if(nation == QString("Egypt"))
        return Egypt;
    else if(nation == QString("Random"))
        return Random;
    else
        return NO_NATION;
}

#endif
