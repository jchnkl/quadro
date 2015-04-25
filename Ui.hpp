#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

#include <QApplication>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

namespace Quadro {

class MoveableFilter
  : public QObject
{
  public:
    MoveableFilter(QWidget * const widget)
      : m_widget(widget)
    {
      QApplication::setOverrideCursor(Qt::SizeAllCursor);
    }

    ~MoveableFilter(void)
    {
      QApplication::restoreOverrideCursor();
    }

    bool
    eventFilter(QObject * watched, QEvent * event)
    {
      bool ret = false;

      if (m_move && event->type() == QEvent::MouseMove) {
        QMouseEvent * e = static_cast<QMouseEvent *>(event);
        m_widget->move(m_widget->pos() + e->pos() - m_pos);
        ret = true;

      } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent * e = static_cast<QMouseEvent *>(event);
        m_move = true;
        m_pos = e->pos();
        ret = true;

      } else if (event->type() == QEvent::MouseButtonRelease) {
        m_move = false;
        ret = true;
      }

      return ret;
    }

  private:
    bool m_move;
    QPoint m_pos;
    QWidget * const m_widget;
}; // class Moveable

class Ui
  : public QGroupBox
{
  Q_OBJECT

  signals:
    void loadUrl(const QString & url);
    void changed(bool show);
    void moveButtonPressed(void);
    void resizeButtonPressed(void);

  public slots:
    void
    onUrlChanged(const QUrl & url)
    {
      m_UrlBar.setText(url.toString());
    }

    void
    onShow(void)
    {
      this->show();
      this->raise();
      emit changed(true);
    }

    void
    onHide(void)
    {
      this->hide();
      emit changed(false);
    }

  public:
    Ui(QWidget * parent)
      : QGroupBox(parent)
    {
      this->setObjectName("QuadroUi");
      this->setWindowFlags(Qt::FramelessWindowHint);
      this->setStyleSheet("QGroupBox#QuadroUi { border: 0px; background-color: rgba(0,0,0,25%); }");

      m_HideButton.setIcon(QIcon("app_hide.svg"));
      m_MoveButton.setIcon(QIcon("move.svg"));
      m_ResizeButton.setIcon(QIcon("resize.svg"));

      m_UrlBar.setWindowFlags(Qt::FramelessWindowHint);
      m_MoveButton.setWindowFlags(Qt::FramelessWindowHint);
      m_ResizeButton.setWindowFlags(Qt::FramelessWindowHint);

      m_GridLayout.setMargin(0);
      m_GridLayout.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
      m_GridLayout.addWidget(&m_UrlBar, 0, 0);
      m_GridLayout.addWidget(&m_MoveButton, 0, 1);
      m_GridLayout.addWidget(&m_ResizeButton, 0, 2);
      m_GridLayout.addWidget(&m_HideButton, 0, 3);

      this->setLayout(&m_GridLayout);

      connect(&m_HideButton, &QPushButton::pressed, this, &Ui::onHide);
      connect(&m_MoveButton, &QPushButton::pressed, this, &Ui::onMoveButtonPressed);
      connect(&m_ResizeButton, &QPushButton::pressed, this, &Ui::onResizeButtonPressed);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Ui::onReturnPressed);

      this->hide();
    }

    QLineEdit &
    urlBar(void)
    {
      return m_UrlBar;
    }

    QPushButton &
    moveButton(void)
    {
      return m_MoveButton;
    }

    QPushButton &
    resizeButton(void)
    {
      return m_ResizeButton;
    }

  protected:
    void
    onMoveButtonPressed(void)
    {
      emit moveButtonPressed();
    }

    void
    onResizeButtonPressed(void)
    {
      emit resizeButtonPressed();
    }

    void
    onReturnPressed(void)
    {
      m_UrlBar.selectAll();
      emit loadUrl(m_UrlBar.text());
    }

  private:
    QGridLayout m_GridLayout;
    QLineEdit m_UrlBar;
    QPushButton m_HideButton;
    QPushButton m_MoveButton;
    QPushButton m_ResizeButton;
}; // class Ui

}; // namespace Quadro

#endif // _QUADRO_BROWSER_HPP
