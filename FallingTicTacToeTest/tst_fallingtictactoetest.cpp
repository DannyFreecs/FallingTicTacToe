#include <QString>
#include <QtTest>
#include "game.h"
#include "storage.h"

class FallingTicTacToeTest : public QObject
{
    Q_OBJECT

private:
    Game* game;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testNewGame();
    void testIsGameStarted();
    void testColsRows();
    void testCurrPlayer();
    void testLastEmptyRow();
    void testGameStep();
    void testSaveGame();
    void testLoadGame();
};

void FallingTicTacToeTest::initTestCase()
{
    game = new Game(this);
    QCOMPARE(game->isGameStarted(), false);
}

void FallingTicTacToeTest::cleanupTestCase()
{
    delete game;
}

void FallingTicTacToeTest::testIsGameStarted()
{
    game->NewGame(4, 4);
    QVERIFY(game->isGameStarted() == true);
}

void FallingTicTacToeTest::testNewGame()
{
    game->NewGame(4, 4);

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            QCOMPARE(game->Cell(i, j), Game::NoPlayer);
}

void FallingTicTacToeTest::testColsRows()
{
    game->NewGame(10, 8);
    QCOMPARE(game->Cols(), 10);
    QCOMPARE(game->Rows(), 8);
}

void FallingTicTacToeTest::testCurrPlayer()
{
    game->NewGame(5, 5);

    QCOMPARE(game->CurrPlayer(), Game::XPlayer);
    game->GameStep(4, 0);
    QCOMPARE(game->CurrPlayer(), Game::OPlayer);
}

void FallingTicTacToeTest::testLastEmptyRow()
{
    game->NewGame(8,8);
    game->GameStep(0, 7);
    QCOMPARE(game->LastEmptyRow(2), 7);
    QCOMPARE(game->LastEmptyRow(0), 6);
}

void FallingTicTacToeTest::testGameStep()
{
    game->NewGame(6, 6);
    game->GameStep(0, 5);
    game->GameStep(3, 5);
    QCOMPARE(game->Cell(0, 5), Game::XPlayer);
    QCOMPARE(game->Cell(3, 5), Game::OPlayer);
}

void FallingTicTacToeTest::testSaveGame()
{
    game->NewGame(4,4);
    game->GameStep(0,3);
    game->GameStep(0,2);
    game->GameStep(2,3);
    game->GameStep(1,3);

    game->SaveGame();
}

void FallingTicTacToeTest::testLoadGame()
{
    game->NewGame(12,6);
    game->GameStep(0,3);
    game->GameStep(0,2);
    game->GameStep(2,3);
    game->GameStep(1,3);

    game->LoadGame("asad.sav");

    QCOMPARE(game->Cell(3,3), Game::OPlayer);
    QCOMPARE(game->Rows(), 4);
    QCOMPARE(game->Cols(), 4);
}

QTEST_APPLESS_MAIN(FallingTicTacToeTest)

#include "tst_fallingtictactoetest.moc"
