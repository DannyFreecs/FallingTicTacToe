#include "gamescene.h"
#include <QGraphicsSceneMouseEvent>

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent)
{

}

GameScene::~GameScene()
{

}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QPointF mpos = mouseEvent->scenePos();
  emit SceneClick((int)(mpos.x() / 34),(int)(mpos.y() / 34));
}
