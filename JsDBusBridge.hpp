#ifndef _QUADRO_JSDBUSBRIDGE_HPP
#define _QUADRO_JSDBUSBRIDGE_HPP

#include <iostream>
#include <algorithm>
#include <memory>
#include <QtDBus>
#include <QObject>
#include <QVariant>

class DBusSignal
  : public QObject
{
  Q_OBJECT

  signals:
    void notify(const QVariant &);

  public slots:
    void onSignal(const QDBusMessage &);

  public:
    static
    std::shared_ptr<DBusSignal>
    connect(QDBusConnection & c,
            const QString & service,
            const QString & path,
            const QString & interface,
            const QString & name);
}; // DBusSignal

class DBusConnection
  : public QObject
{
  Q_OBJECT

  public:
    virtual QDBusConnection & bus(void) = 0;
    virtual const QDBusConnection & bus(void) const;

    void reset(void);

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

    Q_INVOKABLE
    DBusSignal *
    connect(const QString & service,
            const QString & path,
            const QString & interface,
            const QString & name);

    Q_INVOKABLE
    void
    disconnect(const QString & service,
               const QString & path,
               const QString & interface,
               const QString & name);

    Q_INVOKABLE
    bool
    isConnected(const QString & service,
                const QString & path,
                const QString & interface,
                const QString & name);

  private:
    QHash<QString, QPair<std::size_t, std::shared_ptr<DBusSignal>>> m_Signals;

    QString
    key(const QString &, const QString &, const QString &, const QString &);
}; // class DBusConnection

class DBusSystemConnection
  : public DBusConnection
{
  public:
    DBusSystemConnection(void);
    DBusSystemConnection(const DBusSystemConnection &);
    QDBusConnection & bus(void);

  private:
    QDBusConnection m_SystemBus;
}; // class DBusSystemConnection

class DBusSessionConnection
  : public DBusConnection
{
  public:
    DBusSessionConnection(void);
    DBusSessionConnection(const DBusSessionConnection &);
    QDBusConnection & bus(void);

  private:
    QDBusConnection m_SessionBus;
}; // class DBusSessionConnection

class DBus
  :public QObject
{
  Q_OBJECT
  Q_PROPERTY(DBusConnection * system READ system)
  Q_PROPERTY(DBusConnection * session READ session)

  public:
    DBus(void)
    {
      qRegisterMetaType<DBusSignal *>();
      qRegisterMetaType<DBusConnection *>();
    }

    void reset(void);
    DBusConnection * system(void);
    DBusConnection * session(void);

  private:
    DBusSystemConnection m_SystemConnection;
    DBusSessionConnection m_SessionConnection;
}; // class DBus

#endif // _QUADRO_JSDBUSBRIDGE_HPP
