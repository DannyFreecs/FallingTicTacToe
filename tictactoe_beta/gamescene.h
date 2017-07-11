#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>

class GameScene : public QGraphicsScene
{
  Q_OBJECT

public:
  explicit GameScene(QObject *parent = Q_NULLPTR);
  ~GameScene();

signals:
  void SceneClick(int,int);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

};

#endif // GAMESCENE_H
