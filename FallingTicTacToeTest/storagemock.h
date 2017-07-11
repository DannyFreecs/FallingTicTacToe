#ifndef STORAGEMOCK
#define STORAGEMOCK

#include "storage.h"
#include <QObject>
#include <QDebug>
#include <QFile>

class StorageMock : public Storage
{


public:
  //  StorageMock(){}
    bool SaveToFile(QString filename, QByteArray &gamedata)
    {
        for (int i = 0; i < gamedata.size(); i++)
            qDebug() << (int)gamedata[i] << " ";
        qDebug() << endl;

        return true;
    }

    bool LoadFromFile(QString filename, QByteArray &gamedata)
    {
        QFile infile(filename);
        infile.open(QFile::ReadOnly);
        QByteArray data = infile.readAll();
        gamedata = data;

        qDebug() << "game loaded from " << filename << endl;
        for(int i=0; i<gamedata.size(); i++)
            qDebug() << (int)gamedata[i] << " ";
        qDebug() << endl;

        return true;
    }
};

#endif // STORAGEMOCK

