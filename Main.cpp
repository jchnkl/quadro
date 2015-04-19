#include <iostream>
#include <QApplication>
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
    {
      QCommandLineParser parser;

      parser.addHelpOption();
      parser.addVersionOption();

      QCommandLineOption urlOption(
          QStringList() << "u" << "url",
          QCoreApplication::translate("main", "URL to load at startup"),
          QCoreApplication::translate("main", "url"));

      parser.addOption(urlOption);

      QCommandLineOption fullscreenOption(
          QStringList() << "f" << "fullscreen",
          QCoreApplication::translate("main", "Set window to fullscreen mode"));

      parser.addOption(fullscreenOption);

      parser.process(app);

      m_url = parser.value(urlOption);
      m_fullscreen = parser.isSet(fullscreenOption);
    }

    bool hasUrl(void) const { return m_url.length() > 0; }
    const QString & url(void) const { return m_url; }
    bool fullscreen(void) const { return m_fullscreen; }

  private:
    QString m_url;
    bool m_fullscreen;
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

      windowType.clear(NetWmWindowType::Same).add(NetWmWindowType::Desktop);

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

      connect(&m_View, &QWebView::loadFinished, this, &Browser::renderToImage);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Browser::loadUrlFromBar);

      if (config.fullscreen()) {
        this->showFullScreen();
      } else {
        this->show();
      }

      if (config.hasUrl()) {
        this->loadUrl(QUrl(config.url()));
      }
    }

    void loadUrl(const QUrl & url)
    {
      m_View.load(url);
    }

  protected:
    void loadUrlFromBar(void)
    {
      loadUrl(QUrl(m_UrlBar.text(), QUrl::TolerantMode));
      m_UrlBar.selectAll();
    }

    void
    renderToImage(bool ok)
    {
      if (ok) {
        QPixmap pixmap(m_View.size());
        m_View.render(&pixmap, QPoint(), m_View.rect());
        pixmap.save("foo.jpg");
      }
      std::cerr << "done" << std::endl;
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
  QApplication::setApplicationName("Salsa");
  QApplication::setApplicationVersion("1.0");

  Config config(app);
  Browser browser(config);
  return app.exec();
}