#ifndef _QUADRO_CONFIG_CPP
#define _QUADRO_CONFIG_CPP

#include "Config.hpp"

namespace Quadro {

Config::Config(const QCoreApplication & app)
  : m_x(0)
  , m_y(0)
  , m_width(800)
  , m_height(600)
  , m_fullscreen(false)
  , m_window_type_hint(NetWmWindowType::Normal)
{
  QCommandLineParser parser;

  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption xOption = makeOption(QStringList() << "x", "x position on screen", "x");
  QCommandLineOption yOption = makeOption(QStringList() << "y", "y position on screen", "y");
  QCommandLineOption widthOption = makeOption(QStringList() << "width", "window width", "width");
  QCommandLineOption heightOption = makeOption(QStringList() << "height", "window height", "height");
  QCommandLineOption urlOption = makeOption(QStringList() << "u" << "url", "URL to load at startup", "url");
  QCommandLineOption fullscreenOption(QStringList() << "f" << "fullscreen", "Set window to fullscreen mode");
  QCommandLineOption maxVertOption(QStringList() << "maximize-vertical", "Maximize window vertically");
  QCommandLineOption maxHorzOption(QStringList() << "maximize-horizontal", "Maximize window horizontally");
  QCommandLineOption windowTypeOption = makeOption(QStringList() << "t" << "type", "Set window type: normal, desktop, dock (default: normal)", "type");

  parser.addOption(xOption);
  parser.addOption(yOption);
  parser.addOption(widthOption);
  parser.addOption(heightOption);
  parser.addOption(urlOption);
  parser.addOption(fullscreenOption);
  parser.addOption(maxVertOption);
  parser.addOption(maxHorzOption);
  parser.addOption(windowTypeOption);

  parser.process(app);

  if (parser.isSet(xOption)) {
    m_x = std::stoi(parser.value(xOption).toStdString());
  }

  if (parser.isSet(yOption)) {
    m_y = std::stoi(parser.value(yOption).toStdString());
  }

  if (parser.isSet(widthOption)) {
    m_width = std::stoul(parser.value(widthOption).toStdString());
  }

  if (parser.isSet(heightOption)) {
    m_height = std::stoul(parser.value(heightOption).toStdString());
  }

  m_url = parser.value(urlOption);
  m_fullscreen = parser.isSet(fullscreenOption);
  m_max_vertical = parser.isSet(maxVertOption);
  m_max_horizontal = parser.isSet(maxHorzOption);

  auto type = parser.value(windowTypeOption);
  if (type == "normal") {
    m_window_type_hint = NetWmWindowType::Normal;
  } else if (type == "desktop") {
    m_window_type_hint = NetWmWindowType::Desktop;
  } else if (type == "dock") {
    m_window_type_hint = NetWmWindowType::Dock;
  }
}

int
Config::x(void) const
{
  return m_x;
}

int
Config::y(void) const
{
  return m_y;
}

unsigned int
Config::width(void) const
{
  return m_width;
}

unsigned int
Config::height(void) const
{
  return m_height;
}

bool
Config::hasUrl(void) const
{
  return m_url.length() > 0;
}

const QString &
Config::url(void) const
{
  return m_url;
}

bool
Config::fullscreen(void) const
{
  return m_fullscreen;
}

bool
Config::max_vertical(void) const
{
  return m_max_vertical;
}

bool
Config::max_horizontal(void) const
{
  return m_max_horizontal;
}

NetWmWindowType::Hint
Config::windowTypeHint(void) const
{
  return m_window_type_hint;
}

QCommandLineOption
Config::makeOption(QStringList & list, const char * description, const char * valueName)
{
  return QCommandLineOption(
      list,
      QCoreApplication::translate("main", description),
      QCoreApplication::translate("main", valueName));
}

}; // namespace Quadro

#endif // _QUADRO_CONFIG_CPP
