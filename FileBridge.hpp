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

#endif // _QUADRO_FILEBRIDGE_HPP
