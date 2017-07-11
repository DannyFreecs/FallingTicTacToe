#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>

#include "game.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QLabel *player;
  Game *game;
  QGraphicsScene *scene;
  QPixmap *xpm, *opm;

  void DrawTable(bool rebuild = false);
  void FallPlayer(QGraphicsPixmapItem *item, int destrow);

protected:
  void closeEvent (QCloseEvent *event);

private slots:
  //Exit
  void on_actionKilepes_triggered();
  //New Game
  void on_actionUjJatek_triggered();
  void on_SceneClick(int col, int row);
  void on_GameOver(int winner);
  void onSetPlayer(int playerno);


  //Save
  void on_actionJatekMentes_triggered();
  //Load
  void on_actionJatekBetoltes_triggered();
};

#endif // MAINWINDOW_H
