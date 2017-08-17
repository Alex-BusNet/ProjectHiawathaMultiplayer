#ifndef DATATYPES_H
#define DATATYPES_H

#include <QJsonArray>
#include <QVector>
#include <QIcon>
#include "Common/nation.h"


enum ActionState { MOVE_UNIT, ATTACK_MELEE, ATTACK_RANGE, ATTACK_CITY, FOUND_CITY, AI_FOUND_CITY, FIND_UNIT, FIND_CITY, INVADE, IDLE, CONQUER, AI_DECLARE_WAR };
enum ViewState { DISCOVERED, HIDDEN, VISIBLE };
enum MessageTypes { TURN_START = -17, TURN_END = -16,
                    INITIAL_DIPLO_DATA = -15, INITIAL_MAP_DATA = -14, INITIAL_CIV_DATA = -13,
                    BUILDING_UPDATE_DATA = -12, UNIT_UPDATE_DATA = -11,
                    DIPLO_UPDATE_DATA = -10, MAP_UPDATE_DATA = -9, CIV_UPDATE_DATA = -8,
                    GAME_SETUP_INFO = -7, PLAYER_JOINED = -6, GAME_START = -5,
                    PLAYER_SETUP_UPDATE = -4, UNKNOWN = -3, CLIENT_TO_ALL = -2,
                    SYSTEM_MESSAGE = -1 /* CLIENT_WHISPER_0, CLIENT_WHISPER_1, ... CLIENT_WHISPER_11 */
                  };

typedef struct {bool updateBorders, updateCitizens, productionFinished, cityHealed; QVector<int> cityIndecies;} Update_t;

typedef struct {int column; int row;} TileID;
typedef struct {int column, row; bool newData, relocateOrderGiven;} TileData;
typedef struct {int index; bool player, target; } SelectData;
typedef struct {int tileIndex; int civIndex; ViewState state;} ViewData;

typedef struct {int gold, production, research, food, culture;} YieldStruct;

typedef struct{int IconIndex; QString ToolTipMessage;} Notification;
typedef struct{bool atWar; QVector<int> warringCivListIndex;} WarData;

typedef struct{int cityIndex; int producedUnitIndex; bool isUnit; int civIndex;} CityProdData;

typedef struct{int type; QString sender; QString data; } MessageDataType;
typedef struct {QString leaderName; Nation nation; bool isAi; QIcon* civIcon; QString userName;} CivInfo;

#endif // DATATYPES_H
