#ifndef STORAGE_H
#define STORAGE_H
#include <QObject>
#include <QString>

class Storage
{

public:
  Storage() {};
  virtual bool SaveToFile(QString filename, QByteArray &gamedata) = 0;
  virtual bool LoadFromFile(QString filename, QByteArray &gamedata) = 0;

signals:

public slots:
};

#endif // STORAGE_H
