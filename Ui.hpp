#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

#include <cmath>

#include <QApplication>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include "Util.hpp"

#define BORDER_SIZE 20

namespace Quadro {

class UiFrame
  : public QFrame
{
  Q_OBJECT

  signals:
    void resizeBy(const QMargins & offset);

  public:
    UiFrame(QWidget * parent)
      : QFrame(parent)
      , m_DoResize(false)
    {
      this->setMouseTracking(true);
    }

  protected:
    void
    mouseMoveEvent(QMouseEvent * e)
    {
      if (m_DoResize) {
        QMargins offset;

        switch (m_ResizeDirection) {
          case Left:
            offset.setLeft(m_ResizeOffset.x() - e->globalX());
            break;
          case Right:
            offset.setRight(e->globalX() - m_ResizeOffset.x());
            break;
          case Top:
            offset.setTop(m_ResizeOffset.y() - e->globalY());
            break;
          case Bottom:
            offset.setBottom(e->globalY() - m_ResizeOffset.y());
            break;
          case TopLeft:
            offset.setTop(m_ResizeOffset.y() - e->globalY());
            offset.setLeft(m_ResizeOffset.x() - e->globalX());
            break;
          case TopRight:
            offset.setTop(m_ResizeOffset.y() - e->globalY());
            offset.setRight(e->globalX() - m_ResizeOffset.x());
            break;
          case BottomLeft:
            offset.setBottom(m_ResizeOffset.y() - e->globalY());
            offset.setLeft(m_ResizeOffset.x() - e->globalX());
            break;
          case BottomRight:
            offset.setBottom(m_ResizeOffset.y() - e->globalY());
            offset.setRight(e->globalX() - m_ResizeOffset.x());
            break;
          case None:
            break;
        };

        switch (m_ResizeDirection) {
          case None: break;
          default:
            emit resizeBy(offset);
        };

        m_ResizeOffset = e->globalPos();

      } else {
        Qt::CursorShape shape = cursorShape(e->pos(), this->geometry());
        if (shape != m_CurrentShape) {
          m_CurrentShape = shape;
          QGuiApplication::changeOverrideCursor(shape);
        }
      }
    }

    void
    mousePressEvent(QMouseEvent * e)
    {
      if (e->button() == Qt::LeftButton) {
        m_DoResize = true;
        m_ResizeOffset = e->globalPos();
        m_ResizeDirection = direction(e->pos(), this->geometry());
      }
    }

    void
    mouseReleaseEvent(QMouseEvent * e)
    {
      if (e->button() == Qt::LeftButton) {
        m_DoResize = false;
      }
    }

    void
    enterEvent(QEvent *)
    {
      QApplication::setOverrideCursor(cursorShape(this->cursor().pos(), this->geometry()));
    }

    void
    leaveEvent(QEvent *)
    {
      QApplication::restoreOverrideCursor();
    }

  private:
    bool m_DoResize;
    QPoint m_ResizeOffset;
    Direction m_ResizeDirection;
    Qt::CursorShape m_CurrentShape;
}; // class UiFrame

class UiWidget
  : public QWidget
{
  public:
    UiWidget(QWidget * parent)
      : QWidget(parent)
    {
      this->setObjectName("UiWidget");
      this->setStyleSheet("#UiWidget { background-color: rgba(0,0,0,25%); }");
    }
}; // class UiWidget

class Ui
  : public QWidget
{
  Q_OBJECT

  signals:
    void loadUrl(const QString & url);
    void moveBy(const QPoint & offset);
    void resizeBy(const QMargins & offset);

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
    }

    void
    onHide(void)
    {
      this->hide();
    }

  public:
    Ui(QWidget * parent)
      : QWidget(parent)
      , m_Frame(this)
      , m_UiWidget(this)
      , m_DoMove(false)
    {
      m_HideButton.setIcon(QIcon("app_hide.svg"));

      m_GridLayout.setMargin(0);
      m_GridLayout.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
      m_GridLayout.addWidget(&m_UrlBar, 0, 0);
      m_GridLayout.addWidget(&m_HideButton, 0, 1);

      m_UiWidget.setLayout(&m_GridLayout);

      m_Frame.setLineWidth(BORDER_SIZE);
      m_Frame.setFrameStyle(QFrame::Panel | QFrame::Raised);

      connect(&m_Frame, &UiFrame::resizeBy, this, &Ui::resizeBy);
      connect(&m_HideButton, &QPushButton::pressed, this, &Ui::onHide);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Ui::onReturnPressed);

      this->hide();
    }

  protected:
    void
    resizeEvent(QResizeEvent * e)
    {
      m_Frame.resize(e->size());
      m_UiWidget.move(BORDER_SIZE, BORDER_SIZE);
      m_UiWidget.resize(e->size() - QSize(2 * BORDER_SIZE, 2 * BORDER_SIZE));
      QWidget::resizeEvent(e);
    }

    void
    onReturnPressed(void)
    {
      m_UrlBar.selectAll();
      emit loadUrl(m_UrlBar.text());
    }

    void
    mousePressEvent(QMouseEvent * e)
    {
      if (e->button() == Qt::LeftButton) {
        m_DoMove = true;
        m_MoveOffset = e->pos();
        QApplication::setOverrideCursor(Qt::SizeAllCursor);
      }
    }

    void
    mouseReleaseEvent(QMouseEvent * e)
    {
      if (e->button() == Qt::LeftButton) {
        m_DoMove = false;
        QApplication::restoreOverrideCursor();
      }
    }

    void
    mouseMoveEvent(QMouseEvent * e)
    {
      if (m_DoMove) {
        emit moveBy(e->pos() - m_MoveOffset);
      }
    }

  private:
    bool m_DoMove;
    QPoint m_MoveOffset;

    UiFrame m_Frame;
    UiWidget m_UiWidget;
    QGridLayout m_GridLayout;
    QLineEdit m_UrlBar;
    QPushButton m_HideButton;
}; // class Ui

}; // namespace Quadro

#endif // _QUADRO_BROWSER_HPP
