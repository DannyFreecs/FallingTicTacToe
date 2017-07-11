#ifndef STORAGE_H
#define STORAGE_H
#include <QObject>

class Storage : public QObject
{
  Q_OBJECT
public:
  explicit Storage(QObject *parent = 0);
  bool SaveToFile(QString filename, QByteArray &gamedata);
  bool LoadFromFile(QString filename, QByteArray &gamedata);

signals:

public slots:
};

#endif // STORAGE_H
