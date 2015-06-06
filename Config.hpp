#ifndef _QUADRO_CONFIG_HPP
#define _QUADRO_CONFIG_HPP

#include <QCoreApplication>
#include <QCommandLineParser>
#include "NetWmWindowType.hpp"

namespace Quadro {

class Config
{
  public:
    Config(const QCoreApplication & app);

    int
    x(void) const;

    int
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
    int m_x;
    int m_y;
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
