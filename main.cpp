#include <iostream>
#include <QPixmap>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QtWidgets>
#include <QRect>
#include <QWebView>

class Browser : public QGroupBox
{
  public:
    Browser(void)
      : m_UrlBarAnimation(&m_UrlBar, "visible")
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
      // this->grabMouse();

      // connect(&m_Layout, &QWidget::mouseMoveEvent, this, &Browser::showUrlBar);
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

    void
    enterEvent(QEvent * event)
    {
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;

      m_UrlBar.setVisible(false);

      // m_UrlBar.show();
      m_UrlBarAnimation.setDuration(500);
      m_UrlBarAnimation.setDirection(QAbstractAnimation::Forward);
      // QRect startRect(Rect_Relative_to_Opener_Button);
      // QRect endRect(Shifted_Rect_Relative_to_Opener_Button);
      // animation.setStartValue(startRect);
      // animation.setEndValue(endRect);
      // animation.setStartValue(QRect(0, 0, 0, 0));
      // animation.setEndValue(QRect(0, 0, m_UrlBar.width(), m_UrlBar.height()));
      // m_UrlBarAnimation.setStartValue(QSize(m_UrlBar.width(), 0));
      // m_UrlBarAnimation.setEndValue(QSize(m_UrlBar.width(), m_UrlBar.height()));
      // m_UrlBarAnimation.setStartValue(0);
      // m_UrlBarAnimation.setEndValue(m_UrlBar.height());
      // m_UrlBarAnimation.start();

      m_UrlBarAnimation.setStartValue(false);
      m_UrlBarAnimation.setEndValue(true);
    }

    void
    leaveEvent(QEvent * event)
    {
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;

      m_UrlBar.setVisible(true);
      m_UrlBarAnimation.setDirection(QAbstractAnimation::Backward);

      m_UrlBarAnimation.setStartValue(true);
      m_UrlBarAnimation.setEndValue(false);

      // m_UrlBarAnimation.setStartValue(QSize(m_UrlBar.width(), m_UrlBar.height()));
      // m_UrlBarAnimation.setEndValue(QSize(m_UrlBar.width(), 0));

      m_UrlBarAnimation.start();
      // m_UrlBar.hide();
    }

    // void
    // mouseMoveEvent(QMouseEvent * event)
    // {
    //   std::cerr << "x: " << event->pos().x()
    //             << "; y: " << event->pos().y()
    //             << "; rx: " << event->pos().rx()
    //             << "; ry: " << event->pos().ry()
    //             << std::endl;
    // }

  private:
    QGridLayout m_Layout;
    QWebView m_View;
    QLineEdit m_UrlBar;
    QPropertyAnimation m_UrlBarAnimation;
}; // Browser

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);

  Browser browser;
  browser.loadUrl(QUrl(argv[1]));
  browser.show();

  return app.exec();
}
