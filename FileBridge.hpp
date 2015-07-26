#ifndef _QUADRO_FILEBRIDGE_HPP
#define _QUADRO_FILEBRIDGE_HPP

#include <QObject>
#include <QVariantMap>

class File
  : public QObject
{
  Q_OBJECT

  public:
    Q_INVOKABLE
    QVariantMap read(const QString & filename);
}; // class File

class Stat
  : public QObject
{
  Q_OBJECT

  public:
    Q_INVOKABLE
    QVariantMap fs(const QString & path);
    Q_INVOKABLE
    QVariantMap file(const QString & file);
}; // class Stat

#endif // _QUADRO_FILEBRIDGE_HPP
