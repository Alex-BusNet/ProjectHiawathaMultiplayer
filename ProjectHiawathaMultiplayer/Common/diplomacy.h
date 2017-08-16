#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include "nation.h"

class Diplomacy : public QWidget
{
    Q_OBJECT
public:
    explicit Diplomacy(QWidget *parent = 0);
    ~Diplomacy();

    void SetLocalPlayer(Nation player, int civListIndex);

    void AddLeader(QString _name, QPixmap _image, Nation _nation, bool isPlayer);
    void RemoveLeader(Nation nation);
    void DeclareWarOn(Nation target, int targetIndex, Nation aggressor, int aggressorIndex);

    void UpdateTurn();
    void UpdateLeader(int row);
    void MeetPlayer(int row, int playerIndex);
    void SetLeaderImage(int index, QPixmap &image);

    void WriteDiploSaveData(QJsonObject &obj) const;
    void ReadDiploSaveData(const QJsonObject &obj);
    void WriteDiploUpdateData(QJsonObject &obj) const;
    void ReadDiploUpdateData(const QJsonObject &obj);

    int GetLengthOfWar(Nation ai);
    int GetNumberOfWars(Nation ai);
    int GetIndex();

    bool MakePeaceWith(int resolverIndex, Nation resolverNation, int targetIndex, Nation targetNation);
    bool AtPermanentWar(Nation ai);

    QPushButton *declareWar;
    QPushButton *makePeace;

private:
    enum WarStatus {SELF, AT_WAR, AT_PEACE, PERMANENT_WAR};
    enum UpdateFunc{ UDPATE, REMOVE };
    typedef struct {Nation nation; int warStartedOn, timesAtWarWith; WarStatus warStat; bool hasMet;} WarHistory;
    typedef struct {QPixmap image; QString leaderName, displayString; Nation nation; QVector<WarHistory> warChart;  } DiplomacyItem;
    typedef struct {int diIndex; UpdateFunc func; } UpdateStatus;

    QPushButton *closeDiplo;
    QLabel *leaderImage, *leaderName, *warStatus, *nationName, *atWarWith;

    QListWidget *leaderListArea;

    QVector<DiplomacyItem*> diploItemList;
    QVector<UpdateStatus> updatedDiploItems;

    Nation localPlayer;
    int localPlayerIndex;
    int turn;


signals:

public slots:
    void selectLeader(QListWidgetItem *item);
    void slot_delcareWar();
    void slot_makePeace();
};

#endif // DIPLOMACY_H
