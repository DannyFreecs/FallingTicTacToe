#ifndef GAME_H
#define GAME_H
#include <QObject>


class Game : public QObject
{
  Q_OBJECT

public:
  explicit Game(QObject *parent = 0);
  ~Game();

  enum TableCell { NoPlayer, XPlayer, OPlayer, NA };
  Q_ENUM(TableCell)

  void NewGame(int numofcols, int numofrows);
  bool isGameStarted() const { return gamestarted; }
  bool isGameSaved() const { return gamesaved; }
  int Cols() const { return cols; }
  int Rows() const { return rows; }
  TableCell CurrPlayer() const { return player; }
  TableCell Cell(int col, int row);
  int LastEmptyRow(int col);
  void GameStep(int col,int row);
  void SaveGame();
  bool LoadGame(QString filename);

private:
  bool gamestarted;
  bool gamesaved;
  int cols;
  int rows;
  QVector<QVector<TableCell>* > *table;
  TableCell player;

  void BuildTable(QByteArray *map);
  bool CheckTable(int col, int row);

signals:
  void EnableSave(bool);
  void ShowSbMsg(QString,int);
  void SetPlayer(int);
  void GameOver(int);

public slots:

};

#endif // GAME_H
