#ifndef _QUADRO_SYSTEMBRIDGE_HPP
#define _QUADRO_SYSTEMBRIDGE_HPP

#include <QObject>
#include <QVariantMap>

class System
  : public QObject
{
  Q_OBJECT

  public:
    Q_INVOKABLE
    bool exec(const QString & prog, const QStringList & args);

}; // class System

#endif // _QUADRO_SYSTEMBRIDGE_HPP
