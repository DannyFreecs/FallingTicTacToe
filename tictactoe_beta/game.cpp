#include "game.h"
#include <QMessageBox>
#include "storage.h"

Game::Game(QObject *parent) : QObject(parent)
{
  gamestarted = false;
  gamesaved = false;
  table = new QVector<QVector<TableCell> *>();

}

Game::~Game()
{
  delete table;
}

void Game::NewGame(int numofcols, int numofrows)
{
  cols = numofcols;
  rows = numofrows;
  BuildTable(NULL);

  gamestarted = true;
  gamesaved = false;
  player = XPlayer;
  emit EnableSave(true);
  emit ShowSbMsg(tr("A játék elindult."),3000);
  emit SetPlayer((int)player);
}

Game::TableCell Game::Cell(int col, int row)
{
  if((col>=cols) || (col<0) || (row>=rows) || (row<0))
    return(NA);
  return( table->at(col)->at(row) );
}

int Game::LastEmptyRow(int col)
{
  for(int i=rows-1; i>=0; i--) {
      if(table->at(col)->at(i)==NoPlayer)
        return(i);
    }
  return(-1);
}

void Game::GameStep(int col, int row)
{
  (*(*table)[col])[row] = player;

  if(!CheckTable(col,row)) {
      if(player==XPlayer)
        player = OPlayer;
      else
        player = XPlayer;
      emit SetPlayer((int)player);
    }
  else {
      // vége a játéknak, game->player-ben a nyertes, ill. NoPlayer ha döntetlen!
      gamestarted = false;
      emit EnableSave(false);
      emit ShowSbMsg(tr("Játék vége."),3000);
      emit GameOver(player);
    }
}

void Game::BuildTable(QByteArray *map)
{
  if( table->count()>0 ) {
    for(int c=0; c<table->count(); c++)
      delete (*table)[c];
    table->clear();
    table->squeeze();
    }
  if( map ) {
      int ind = 3;
      for(int i=0; i<cols; i++) {
          QVector<TableCell> *colv = new QVector<TableCell>();
          for(int j=0; j<rows; j++) {
              colv->append( (TableCell)(map->at(ind++)) );
            }
          table->append(colv);
        }
    }
  else {
      // Build empty table map
      for(int i=0; i<cols; i++) {
          QVector<TableCell> *colv = new QVector<TableCell>();
          for(int j=0; j<rows; j++) {
              colv->append(NoPlayer);
            }
          table->append(colv);
        }
    }
}

bool Game::CheckTable(int col, int row)
{
  TableCell ply;
  int same = 0;
  int noplayercnt = 0;
  for(int r=0; r<rows; r++) {
      ply = (*(*table)[0])[r];
      if(ply==NoPlayer)
          noplayercnt++;
      same = 1;
      for(int c=1; c<cols; c++) {
          if((*(*table)[c])[r]==NoPlayer) {
              ply = (*(*table)[c])[r];
              noplayercnt++;
              continue;
            }
          if(ply==(*(*table)[c])[r]) {
              same++;
              if(same==4) {
                  return(true);
                }
            }
          else {
              ply = (*(*table)[c])[r];
              same = 1;
            }
        }
    }
  if(!noplayercnt) {
      player = NoPlayer;
      emit SetPlayer((int)player);
      return(true);
    }

  // diagonal checking:
  same = 1;
  int c=col;
  int r =row;
  while(Cell(--c,++r)==player) {
      same++;
      if(same==4) {
          return(true);
        }
    }
  c=col;
  r =row;
  while(Cell(++c,--r)==player) {
      same++;
      if(same==4) {
          return(true);
        }
    }
  c=col;
  r =row;
  same=1;
  while(Cell(++c,++r)==player) {
      same++;
      if(same==4) {
          return(true);
        }
    }
  c=col;
  r =row;
  while(Cell(--c,--r)==player) {
      same++;
      if(same==4) {
          return(true);
        }
    }
  return(false);
}

void Game::SaveGame(QString filename)
{

  Storage *s = new Storage(this);
  QByteArray ba(rows*cols+3,0);
  ba[0] = cols;
  ba[1] = rows;
  ba[2] = player;
  int i = 3;
  for(int c=0; c<cols; c++) {
      for(int r=0; r<rows; r++) {
          ba[i++] = Cell(c,r);
        }
    }
  if(s->SaveToFile(filename,ba)) {
    gamesaved = true;
    emit ShowSbMsg(tr("A játék sikeresen mentve."),3000);
    }
  else {
      emit ShowSbMsg(tr("A játék mentése NEM SIKERÜLT!"),3000);
    }
  delete s;
}

bool Game::LoadGame(QString filename)
{
  Storage *s = new Storage(this);
  QByteArray ba;
  bool result = s->LoadFromFile(filename,ba);
  if(result) {
      cols = ba[0];
      rows = ba[1];
      BuildTable(&ba);
      player = (TableCell)ba.at(2);
      gamestarted = true;
      gamesaved = true;
      emit EnableSave(true);
      emit ShowSbMsg(tr("A játék betöltve."),3000);
      emit SetPlayer((int)player);
    }
  else {
      emit ShowSbMsg(tr("A játék betöltése NEM SIKERÜLT!"),3000);
    }
  delete s;
  return(result);
}
