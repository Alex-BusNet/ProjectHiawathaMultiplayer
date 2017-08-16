#ifndef AI_OPERATIONAL_H
#define AI_OPERATIONAL_H

#include "Common/civilization.h"
#include "ai_tactical.h"
#include "Common/map.h"
#include "Common/unitcontroller.h"
class AI_Operational
{
public:
    AI_Operational(QVector<Tile *> CityToBeFounded, Civilization *civ, Civilization *player, Map *map);
private:
    void threatScan(Civilization *civ, Civilization *player);
    void theaterAtWar(Civilization *civ, Civilization *player);
     City *cityTarget;
     AI_Tactical *aiTact;
     UnitController *uc;
};

#endif // AI_OPERATIONAL_H
