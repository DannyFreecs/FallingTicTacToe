#include "storage.h"
#include <QFile>

Storage::Storage(QObject *parent) : QObject(parent)
{

}

bool Storage::SaveToFile(QString filename, QByteArray &gamedata)
{
  QFile outfile(filename);
  bool result = outfile.open(QFile::WriteOnly);
  if(result) {
      result = outfile.write(gamedata)==gamedata.length();
    }
  return(result);
}

bool Storage::LoadFromFile(QString filename, QByteArray &gamedata)
{
  QFile infile(filename);
  bool result = infile.open(QFile::ReadOnly);
  if(result) {
     QByteArray data = infile.readAll();
     result = data.length()>=19;
     gamedata = data;
    }
  return(result);
}
