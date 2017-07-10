#ifndef DATATYPES_H
#define DATATYPES_H

#include <QVector>
#include "Common/nation.h"


enum ActionState { MOVE_UNIT, ATTACK_MELEE, ATTACK_RANGE, ATTACK_CITY, FOUND_CITY, AI_FOUND_CITY, FIND_UNIT, FIND_CITY, INVADE, IDLE, CONQUER, AI_DECLARE_WAR };
enum ViewState { DISCOVERED, HIDDEN, VISIBLE };
enum MessageTypes { PLAYER_JOINED = -6, GAME_START = -5, PLAYER_SETUP_UPDATE = -4, UNKNOWN = -3, CLIENT_TO_ALL = -2, SYSTEM_MESSAGE = -1 /* CLIENT_WHISPER_0, CLIENT_WHISPER_1, ... CLIENT_WHISPER_11 */ };

typedef struct {bool updateBorders, updateCitizens, productionFinished, cityHealed; QVector<int> cityIndecies;} Update_t;

typedef struct {int column; int row;} TileID;
typedef struct {int column, row; bool newData, relocateOrderGiven;} TileData;
typedef struct {int index; bool player, target; } SelectData;
typedef struct {int index; ViewState state;} ViewData;

typedef struct {int gold, production, research, food, culture;} YieldStruct;

typedef struct{int IconIndex; QString ToolTipMessage;} Notification;
typedef struct{bool atWar; QVector<int> warringCivListIndex;} WarData;

typedef struct{int cityIndex; int producedUnitIndex; bool isUnit; int civIndex;} CityProdData;

typedef struct{int type; QString sender; QString data; } MessageDataType;

#endif // DATATYPES_H
