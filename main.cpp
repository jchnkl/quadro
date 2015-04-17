#include <iostream>
#include <QPixmap>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QtWidgets>
#include <QWebView>

class Browser : public QGroupBox
{
  public:
    Browser(void)
    {
      // create & set transparent palette for browser window
      QPalette palette = m_View.palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      m_View.page()->setPalette(palette);

      // enable transparency for underlying window
      m_View.setAttribute(Qt::WA_TranslucentBackground, true);

      m_Layout.addWidget(&m_UrlBar, 0, 0);
      m_Layout.addWidget(&m_View, 1, 0);

      this->setWindowFlags(Qt::FramelessWindowHint);
      this->setAttribute(Qt::WA_TranslucentBackground);
      this->setLayout(&m_Layout);

      connect(&m_View, &QWebView::loadFinished, this, &Browser::renderToImage);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Browser::loadUrlFromBar);
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

  private:
    QWebView m_View;
    QLineEdit m_UrlBar;
    QGridLayout m_Layout;
}; // Browser

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);

  Browser browser;
  browser.loadUrl(QUrl(argv[1]));
  browser.show();

  return app.exec();
}
