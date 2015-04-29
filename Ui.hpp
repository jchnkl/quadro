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
    void mousePressed(QMouseEvent * e);
    void mouseReleased(QMouseEvent * e);
    void mouseMoved(QMouseEvent * e);

  public:
    UiFrame(QWidget * parent)
      : QFrame(parent)
      , m_MouseButtonPressed(false)
      , m_EmitResizeSignal(false)
    {
      this->setMouseTracking(true);
    }


  protected:
    void
    mouseMoveEvent(QMouseEvent * e)
    {
      emit mouseMoved(e);

      if (! m_MouseButtonPressed) {
        Qt::CursorShape shape = cursorShape(e->pos(), this->geometry());
        if (shape != m_CurrentShape) {
          QGuiApplication::changeOverrideCursor(shape);
          m_CurrentShape = shape;
        }
      }
    }

    void
    mousePressEvent(QMouseEvent * e)
    {
      m_MouseButtonPressed = true;
      emit mousePressed(e);
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    mouseReleaseEvent(QMouseEvent * e)
    {
      m_MouseButtonPressed = false;
      emit mouseReleased(e);
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    enterEvent(QEvent *)
    {
      QApplication::setOverrideCursor(cursorShape(this->cursor().pos(), this->geometry()));
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    leaveEvent(QEvent *)
    {
      QApplication::restoreOverrideCursor();
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

  private:
    bool m_MouseButtonPressed;
    bool m_EmitResizeSignal;
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
      , m_DoMouseMove(false)
    {
      m_HideButton.setIcon(QIcon("app_hide.svg"));

      m_GridLayout.setMargin(0);
      m_GridLayout.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
      m_GridLayout.addWidget(&m_UrlBar, 0, 0);
      m_GridLayout.addWidget(&m_HideButton, 0, 1);

      m_UiWidget.setLayout(&m_GridLayout);

      m_Frame.setLineWidth(BORDER_SIZE);
      m_Frame.setFrameStyle(QFrame::Panel | QFrame::Raised);

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
        m_DoMouseMove = true;
        m_MouseOffset = e->pos();
        QApplication::setOverrideCursor(Qt::SizeAllCursor);
      }
    }

    void
    mouseReleaseEvent(QMouseEvent * e)
    {
      if (e->button() == Qt::LeftButton) {
        m_DoMouseMove = false;
        QApplication::restoreOverrideCursor();
      }
    }

    void
    mouseMoveEvent(QMouseEvent * e)
    {
      if (m_DoMouseMove) {
        QWidget * parent = this->parentWidget();
        parent->move(parent->pos() + e->pos() - m_MouseOffset);
      }
    }

  private:
    bool m_DoMouseMove;
    QPoint m_MouseOffset;

    UiFrame m_Frame;
    UiWidget m_UiWidget;
    QGridLayout m_GridLayout;
    QLineEdit m_UrlBar;
    QPushButton m_HideButton;
}; // class Ui

}; // namespace Quadro

#endif // _QUADRO_BROWSER_HPP
