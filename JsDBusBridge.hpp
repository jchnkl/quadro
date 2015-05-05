#ifndef _QUADRO_JSDBUSBRIDGE_HPP
#define _QUADRO_JSDBUSBRIDGE_HPP

#include <iostream>
#include <algorithm>
#include <QtDBus>
#include <QObject>
#include <QVariant>

QVariant
toVariant(const QVariant & variant);

QVariant
toVariant(const QDBusArgument & arg);

class DBusConnection
  : public QObject
{
  Q_OBJECT

  public:
    virtual const QDBusConnection & bus(void) const = 0;

    Q_INVOKABLE
    QVariant
    call(const QString & service,
         const QString & path,
         const QString & interface,
         const QString & method,
         const QVariant & arg1 = QVariant(),
         const QVariant & arg2 = QVariant(),
         const QVariant & arg3 = QVariant(),
         const QVariant & arg4 = QVariant(),
         const QVariant & arg5 = QVariant(),
         const QVariant & arg6 = QVariant(),
         const QVariant & arg7 = QVariant(),
         const QVariant & arg8 = QVariant()) const;

}; // class DBusConnection

class DBusSystemConnection
  : public DBusConnection
{
  public:
    DBusSystemConnection(void);
    DBusSystemConnection(const DBusSystemConnection &);
    const QDBusConnection & bus(void) const;

  private:
    QDBusConnection m_SystemBus;
}; // class DBusSystemConnection

class DBusSessionConnection
  : public DBusConnection
{
  public:
    DBusSessionConnection(void);
    DBusSessionConnection(const DBusSessionConnection &);
    const QDBusConnection & bus(void) const;

  private:
    QDBusConnection m_SessionBus;
}; // class DBusSessionConnection

namespace Quadro {

class DBus
  :public QObject
{
  Q_OBJECT
  Q_PROPERTY(DBusConnection * system READ system)
  Q_PROPERTY(DBusConnection * session READ session)

  signals:
    void propertiesChanged(const QVariant &);

  public slots:
    void onSignal(const QDBusMessage & msg);

  public:
    DBusConnection * system(void);
    DBusConnection * session(void);

  private:
    DBusSystemConnection m_SystemConnection;
    DBusSessionConnection m_SessionConnection;

}; // class DBus

}; // namespace Quadro

#endif // _QUADRO_JSDBUSBRIDGE_HPP
