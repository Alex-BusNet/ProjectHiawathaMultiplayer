#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QPainter>
#include <QWheelEvent>
#include "Common/tile.h"
#include "Common/datatypes.h"

class GameScene : public QGraphicsScene
{
public:
    GameScene(QObject *parent = 0);
    ~GameScene();

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    TileData ProcessTile(bool unitAwaitingRelocation);

    bool isTileSelected, eventQueued, redrawTile, unitMoveOrdered, citySelected, findUnit;

    int column, row;

private:

    TileData processedData;
    QPointF mpScenePos, mpScreenPos, mrScenePos, mrScreenPos, lastScenePos, lastScreenPos;

};

#endif // GAMESCENE_H
