#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

#include <QApplication>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

namespace Browser {

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
    void moveButtonPressed(void);
    void resizeButtonPressed(void);

  public:
    Ui(void)
    {
      m_MoveButton.setIcon(QIcon("move.svg"));
      m_ResizeButton.setIcon(QIcon("resize.svg"));

      m_Layout.setMargin(0);
      m_Layout.addWidget(&m_UrlBar);
      m_Layout.addWidget(&m_MoveButton);
      m_Layout.addWidget(&m_ResizeButton);

      this->setLayout(&m_Layout);

      connect(&m_MoveButton, &QPushButton::pressed, this, &Ui::onMoveButtonPressed);
      connect(&m_ResizeButton, &QPushButton::pressed, this, &Ui::onResizeButtonPressed);
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

  private:
    QHBoxLayout m_Layout;
    QLineEdit m_UrlBar;
    QPushButton m_MoveButton;
    QPushButton m_ResizeButton;
}; // class Ui

}; // namespace Browser

#endif // _QUADRO_BROWSER_HPP
