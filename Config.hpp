#ifndef _QUADRO_CONFIG_HPP
#define _QUADRO_CONFIG_HPP

#include <QCoreApplication>
#include <QCommandLineParser>
#include "NetWmWindowType.hpp"

namespace Quadro {

template <typename Derived>
class PositionValue : public Derived {
  public:
    template <typename T>
    PositionValue(T t) : Derived(t) {}
    bool isNegative(void) const
    {
      return std::signbit(static_cast<const Derived *>(this)->value());
    }
};

template <typename ValueT>
class Value {
  public:
    Value(ValueT value) : m_value(value) {}
    ValueT value(void) const { return m_value; }
  protected:
    ValueT m_value;
};

class X : public Value<double> {
  public:
    X(double v) : Value(v) {}
};

class Y : public Value<double> {
  public:
    Y(double v) : Value(v) {}
};

class Config
{
  public:
    Config(const QCoreApplication & app);

    const PositionValue<X> &
    x(void) const;

    const PositionValue<Y> &
    y(void) const;

    unsigned int
    width(void) const;

    unsigned int
    height(void) const;

    bool
    hasUrl(void) const;

    const QString &
    url(void) const;

    bool
    fullscreen(void) const;

    bool
    max_vertical(void) const;

    bool
    max_horizontal(void) const;

    NetWmWindowType::Hint
    windowTypeHint(void) const;

  protected:
    QCommandLineOption
    makeOption(QStringList & list, const char * description, const char * valueName);

  private:
    PositionValue<X> m_x;
    PositionValue<Y> m_y;
    unsigned int m_width;
    unsigned int m_height;
    QString m_url;
    bool m_fullscreen;
    bool m_max_vertical;
    bool m_max_horizontal;
    NetWmWindowType::Hint m_window_type_hint;
}; // Config

}; // namespace Quadro

#endif // _QUADRO_CONFIG_HPP
