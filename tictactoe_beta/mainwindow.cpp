#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QThread>
#include <QCloseEvent>
#include <QFileDialog>

#include "gamescene.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  scene = new GameScene(this);
  xpm = new QPixmap(QApplication::applicationDirPath()+"/resource/xplayer.png");
  opm = new QPixmap(QApplication::applicationDirPath()+"/resource/oplayer.png");
  player = new QLabel(this);
  ui->statusBar->addPermanentWidget(player);
  ui->actionJatekMentes->setEnabled(false);
  game = new Game(this);
  connect(game,SIGNAL(EnableSave(bool)),ui->actionJatekMentes,SLOT(setEnabled(bool)));
  connect(game,SIGNAL(ShowSbMsg(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
  connect(game,SIGNAL(SetPlayer(int)),this,SLOT(onSetPlayer(int)));
  connect(game,SIGNAL(GameOver(int)),this,SLOT(on_GameOver(int)));
  QTimer::singleShot(0,ui->actionUjJatek,SLOT(trigger()));
}

MainWindow::~MainWindow()
{
  delete game;
  delete opm;
  delete xpm;
  delete scene;
  delete ui;
}

void MainWindow::DrawTable(bool rebuild)
{
  scene->clear();
  scene->setSceneRect(0,0,34*game->Cols(),34*game->Rows());
  QPen penNavy2(QBrush("#606090"),2);
  for(int col=1; col<game->Cols(); col++) {
      scene->addLine(col*34-1,0,col*34-1,game->Rows()*34-1,penNavy2);
    }
  for(int row=1; row<game->Rows(); row++) {
      scene->addLine(0,row*34-1,game->Cols()*34-1,row*34-1,penNavy2);
    }
  if(rebuild) {
      QGraphicsPixmapItem *it;
      Game::TableCell tc;
      for(int c=0; c<game->Cols(); c++) {
          for(int r=0; r<game->Rows(); r++) {
              tc = game->Cell(c,r);
              it = NULL;
              if( tc ==Game::XPlayer)
                it = scene->addPixmap(*xpm);
              else if( tc == Game::OPlayer )
                it = scene->addPixmap(*opm);
              if(it)
                it->setPos(c*34,r*34);
            }
        }
    }
  ui->gv->setScene(scene);
}

void MainWindow::FallPlayer(QGraphicsPixmapItem *item, int destrow)
{
  while(item->pos().y() < destrow*34) {
    item->setPos(item->pos().x(),item->pos().y()+17);
    QCoreApplication::processEvents();
    QThread::usleep(20000);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if( game->isGameStarted() && !game->isGameSaved()) {
      if( QMessageBox::warning(this,tr("A játék nem ért véget"),tr("A jelenlegi játék nem ért véget, és nincs mentve! Kilépsz mentés nélkül?"),QMessageBox::No,QMessageBox::Yes)!=QMessageBox::Yes ) {
          event->ignore();
        }
      else
        event->accept();
    }
  else
    event->accept();
}

void MainWindow::onSetPlayer(int playerno)
{
  if(playerno)
      player->setText(tr("Player%1").arg(playerno));
  else
      player->clear();
}

void MainWindow::on_actionKilepes_triggered()
{
  if( game->isGameStarted() && !game->isGameSaved()) {
      if( QMessageBox::warning(this,tr("A játék nem ért véget"),tr("A jelenlegi játék nem ért véget, és nincs mentve! Kilépsz mentés nélkül?"),QMessageBox::No,QMessageBox::Yes)!=QMessageBox::Yes ) {
          return;
        }
    }
  close();
}

void MainWindow::on_actionUjJatek_triggered()
{
  if( game->isGameStarted() && !game->isGameSaved()) {
      if( QMessageBox::warning(this,tr("A játék nem ért véget"),tr("A jelenlegi játék nem ért véget, és nincs mentve! Kilépsz mentés nélkül?"),QMessageBox::No,QMessageBox::Yes)!=QMessageBox::Yes ) {
          return;
        }
    }
    QDialog *dlg = new QDialog();
    dlg->setWindowTitle(tr("Új játék indítása"));
    dlg->setFixedSize(380,160);

    QLabel *dlgtxt = new QLabel(tr("Add meg a játéktábla méreteit:"),dlg);
    dlgtxt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //    dlgtxt->setFrameShape(QFrame::StyledPanel);

    QLabel *coltxt = new QLabel(tr("Oszlopok száma:"),dlg);
    QSpinBox *cols = new QSpinBox(dlg);
    cols->setRange(4,24);
    qsrand(qrand());
    cols->setValue((qrand() % 21) + 4);
    QLabel *rowtxt = new QLabel(tr("Sorok száma:"),dlg);
    QSpinBox *rows = new QSpinBox(dlg);
    rows->setRange(4,16);
    rows->setValue((qrand() % 13) + 4);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(coltxt);
    hbox->addWidget(cols);
    hbox->addWidget(rowtxt);
    hbox->addWidget(rows);

    QPushButton *btnOk = new QPushButton(tr("&OK"),dlg);
    QPushButton *btnCancel = new QPushButton(tr("&Mégsem"),dlg);
    QHBoxLayout *btnbox = new QHBoxLayout();
    btnbox->addWidget(btnOk);
    btnbox->addWidget(btnCancel);

    QVBoxLayout *vbox = new QVBoxLayout(dlg);
    vbox->addWidget(dlgtxt);
    vbox->setAlignment(dlgtxt,Qt::AlignCenter);
    vbox->addLayout(hbox);
    vbox->setAlignment(hbox,Qt::AlignCenter);
    vbox->addLayout(btnbox);
    vbox->setAlignment(btnbox,Qt::AlignBottom|Qt::AlignRight);

    connect(btnOk,SIGNAL(clicked(bool)),dlg,SLOT(accept()));
    connect(btnCancel,SIGNAL(clicked(bool)),dlg,SLOT(reject()));

    cols->selectAll();
    if( dlg->exec() == QDialog::Accepted ) {
        game->NewGame(cols->value(),rows->value());
        setFixedSize(cols->value()*34+50,rows->value()*34+90); // 32pixel image + 2pixel separator line + 30pixel (15+15) margin around
        DrawTable();
        connect(scene,SIGNAL(SceneClick(int,int)),this,SLOT(on_SceneClick(int,int)));
      }
    delete dlg;
}

void MainWindow::on_SceneClick(int col, int row)
{
  if((!game->isGameStarted()) || (col>=game->Cols()) || (row>=game->Rows()))
      return;
  int destrow = game->LastEmptyRow(col);
  if(destrow==-1) {
      ui->statusBar->showMessage(tr("ÉRVÉNYTELEN LÉPÉS"),3000);
      return;
    }
  QGraphicsPixmapItem *it;
  if(game->CurrPlayer()==Game::XPlayer)
    it = scene->addPixmap(*xpm);
  else
    it = scene->addPixmap(*opm);
  it->setPos(col*34,0);
  disconnect(scene,SIGNAL(SceneClick(int,int)),this,SLOT(on_SceneClick(int,int)));
  FallPlayer(it,destrow);
  game->GameStep(col,destrow);
  if(game->isGameStarted())
      connect(scene,SIGNAL(SceneClick(int,int)),this,SLOT(on_SceneClick(int,int)));

}

void MainWindow::on_GameOver(int winner)
{
  QString msg;
  if(!winner) {
      msg = tr("A játék DÖNTETLENNEL ért véget.");
    }
  else {
      msg = tr("A játék véget ért.\n\nA nyertes: PLAYER%1").arg(winner);
    }
  QMessageBox::information(this,tr("Játék vége"),msg);
  QTimer::singleShot(0,this,SLOT(on_actionUjJatek_triggered()));
}



void MainWindow::on_actionJatekMentes_triggered()
{
  QString filename = QFileDialog::getSaveFileName(this,tr("JÁTÉK MENTÉS"),QString(),tr("Játékmentések (*.sav)"));
  if(!filename.endsWith(".sav")) {
      filename.append(".sav");
    }
  if(!filename.isEmpty())
    game->SaveGame(filename);
}

void MainWindow::on_actionJatekBetoltes_triggered()
{
  if( game->isGameStarted() && !game->isGameSaved()) {
      if( QMessageBox::warning(this,tr("A játék nincs mentve"),tr("A jelenlegi játék nem ért véget, és nincs mentve! Egy mentett játék betöltése az aktuális mentése nélkül?"),QMessageBox::No,QMessageBox::Yes)!=QMessageBox::Yes ) {
          return;
        }
    }
  QString filename = QFileDialog::getOpenFileName(this,tr("JÁTÉK BETÖLTÉS"),QString(),tr("Játékmentések (*.sav)"));
  if(!filename.isEmpty()) {
    if(game->LoadGame(filename)) {
        setFixedSize(game->Cols()*34+50,game->Rows()*34+90); // 32pixel image + 2pixel separator line + 30pixel (15+15) margin around
        DrawTable(true);
      }
    }

}
