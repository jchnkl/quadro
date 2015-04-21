#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPixmap>
#include <QWebView>
#include <QX11Info>
#include "NetWmWindowType.hpp"

class Config
{
  public:
    Config(const QCoreApplication & app)
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

      QCommandLineOption xOption = this->getXOption();
      QCommandLineOption yOption = this->getYOption();
      QCommandLineOption widthOption = this->getWidthOption();
      QCommandLineOption heightOption = this->getHeightOption();
      QCommandLineOption urlOption = this->getUrlOption();
      QCommandLineOption fullscreenOption = this->getFullscreenOption();
      QCommandLineOption windowTypeOption = this->getWindowTypeOption();

      parser.addOption(xOption);
      parser.addOption(yOption);
      parser.addOption(widthOption);
      parser.addOption(heightOption);
      parser.addOption(urlOption);
      parser.addOption(fullscreenOption);
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
    x(void) const
    {
      return m_x;
    }

    int
    y(void) const
    {
      return m_y;
    }

    unsigned int
    width(void) const
    {
      return m_width;
    }

    unsigned int
    height(void) const
    {
      return m_height;
    }

    bool
    hasUrl(void) const
    {
      return m_url.length() > 0;
    }

    const QString &
    url(void) const
    {
      return m_url;
    }

    bool
    fullscreen(void) const
    {
      return m_fullscreen;
    }

    NetWmWindowType::Hint
    windowTypeHint(void) const
    {
      return m_window_type_hint;
    }

  protected:
    QCommandLineOption
    getXOption(void)
    {
      QCommandLineOption xOption(
          QStringList() << "x",
          QCoreApplication::translate("main", "x position on screen"),
          QCoreApplication::translate("main", "X"));
      return xOption;
    }

    QCommandLineOption
    getYOption(void)
    {
      QCommandLineOption yOption(
          QStringList() << "y",
          QCoreApplication::translate("main", "y position on screen"),
          QCoreApplication::translate("main", "Y"));
      return yOption;
    }

    QCommandLineOption
    getWidthOption(void)
    {
      QCommandLineOption widthOption(
          QStringList() << "width",
          QCoreApplication::translate("main", "window width"),
          QCoreApplication::translate("main", "width"));
      return widthOption;
    }

    QCommandLineOption
    getHeightOption(void)
    {
      QCommandLineOption heightOption(
          QStringList() << "height",
          QCoreApplication::translate("main", "window height"),
          QCoreApplication::translate("main", "height"));
      return heightOption;
    }

    QCommandLineOption
    getUrlOption(void)
    {
      QCommandLineOption urlOption(
          QStringList() << "u" << "url",
          QCoreApplication::translate("main", "URL to load at startup"),
          QCoreApplication::translate("main", "url"));
      return urlOption;
    }

    QCommandLineOption
    getFullscreenOption(void)
    {
      QCommandLineOption fullscreenOption(
          QStringList() << "f" << "fullscreen",
          QCoreApplication::translate("main", "Set window to fullscreen mode"));
      return fullscreenOption;
    }

    QCommandLineOption
    getWindowTypeOption(void)
    {
      QCommandLineOption windowTypeOption(
          QStringList() << "t" << "type",
          QCoreApplication::translate("main", "Set window type: normal, desktop, dock (default: normal)"),
          QCoreApplication::translate("main", "hint"));
      return windowTypeOption;
    }

  private:
    int m_x;
    int m_y;
    unsigned int m_width;
    unsigned int m_height;
    QString m_url;
    bool m_fullscreen;
    NetWmWindowType::Hint m_window_type_hint;
}; // Config

class Browser
  : public QGroupBox
{
  public:
    Browser(const Config & config)
    {
      // set object name for style sheet
      this->setObjectName("Browser");

      // remove the border
      this->setStyleSheet("QGroupBox#Browser { border: 0px; }");

      // hint for window manager to leave away frames/window borders
      this->setWindowFlags(Qt::FramelessWindowHint);

      // make window background transparent
      this->setAttribute(Qt::WA_TranslucentBackground, true);

      Ewmh ewmh(QX11Info::connection());
      NetWmWindowType windowType(ewmh, this->winId());

      windowType.clear(NetWmWindowType::Same).add(config.windowTypeHint());

      // remove layout margins / padding
      m_Layout.setMargin(0);

      // add widgets to grid layout
      m_Layout.addWidget(&m_UrlBar, 0, 0);
      m_Layout.addWidget(&m_View, 1, 0);

      // use grid layout
      this->setLayout(&m_Layout);

      // create & set transparent palette for browser window
      QPalette palette = m_View.palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      m_View.page()->setPalette(palette);

      // enable transparency for underlying window
      m_View.setAttribute(Qt::WA_TranslucentBackground, true);

      connect(&m_View, &QWebView::urlChanged, this, &Browser::onUrlChanged);
      connect(&m_View, &QWebView::loadFinished, this, &Browser::onLoadFinished);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Browser::onReturnPressed);

      this->setGeometry(config.x(), config.y(), config.width(), config.height());

      if (config.hasUrl()) {
        this->loadUrl(QUrl::fromUserInput(config.url()));
      }

      if (config.fullscreen()) {
        QRect desktop_rect = QApplication::desktop()->screenGeometry();
        this->setGeometry(desktop_rect);
      }

      this->show();
    }

    void loadUrl(const QUrl & url)
    {
      m_View.load(url);
    }

  protected:
    void
    onUrlChanged(const QUrl & url)
    {
      m_UrlBar.setText(url.toString());
    }

    void
    onLoadFinished(bool ok)
    {
      if (ok) {
        QPixmap pixmap(m_View.size());
        m_View.render(&pixmap, QPoint(), m_View.rect());
        pixmap.save("foo.jpg");
      }
      std::cerr << "done" << std::endl;
    }

    void onReturnPressed(void)
    {
      loadUrl(QUrl::fromUserInput(m_UrlBar.text()));
      m_UrlBar.selectAll();
    }

    void
    enterEvent(QEvent *)
    {
      m_UrlBar.show();
    }

    void
    leaveEvent(QEvent *)
    {
      m_UrlBar.hide();
    }

  private:
    // Config m_Config;
    QGridLayout m_Layout;
    QLineEdit m_UrlBar;
    QWebView m_View;
}; // Browser

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("Quadro");
  QApplication::setApplicationVersion("1.0");

  Config config(app);
  Browser browser(config);
  return app.exec();
}
