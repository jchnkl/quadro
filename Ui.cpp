#ifndef _QUADRO_UI_CPP
#define _QUADRO_UI_CPP

#include "Ui.hpp"

namespace Quadro {

UiFrame::UiFrame(QWidget * parent)
  : QFrame(parent)
  , m_DoResize(false)
{
  this->setMouseTracking(true);
}

void
UiFrame::mouseMoveEvent(QMouseEvent * e)
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
        offset.setBottom(e->globalY() - m_ResizeOffset.y());
        offset.setLeft(m_ResizeOffset.x() - e->globalX());
        break;
      case BottomRight:
        offset.setBottom(e->globalY() - m_ResizeOffset.y());
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
    QGuiApplication::changeOverrideCursor(cursorShape(m_ResizeDirection));

  } else {
    Qt::CursorShape shape = cursorShape(e->pos(), this->geometry());
    QGuiApplication::changeOverrideCursor(shape);
  }
}

void
UiFrame::mousePressEvent(QMouseEvent * e)
{
  if (e->button() == Qt::LeftButton) {
    m_DoResize = true;
    m_ResizeOffset = e->globalPos();
    m_ResizeDirection = direction(e->pos(), this->geometry());
  }
}

void
UiFrame::mouseReleaseEvent(QMouseEvent * e)
{
  if (e->button() == Qt::LeftButton) {
    m_DoResize = false;
  }
}

void
UiFrame::enterEvent(QEvent *)
{
  QApplication::setOverrideCursor(cursorShape(this->cursor().pos(), this->geometry()));
}

void
UiFrame::leaveEvent(QEvent *)
{
  QApplication::restoreOverrideCursor();
}

UiWidget::UiWidget(QWidget * parent)
  : QWidget(parent)
{
  this->setObjectName("UiWidget");
  this->setStyleSheet("#UiWidget { background-color: rgba(0,0,0,25%); }");
}

void
Ui::onUrlChanged(const QUrl & url)
{
  m_UrlBar.setText(url.toString());
}

void
Ui::onShow(void)
{
  this->show();
  this->raise();
}

void
Ui::onHide(void)
{
  this->hide();
}

Ui::Ui(QWidget * parent)
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

void
Ui::resizeEvent(QResizeEvent * e)
{
  m_Frame.resize(e->size());
  m_UiWidget.move(BORDER_SIZE, BORDER_SIZE);
  m_UiWidget.resize(e->size() - QSize(2 * BORDER_SIZE, 2 * BORDER_SIZE));
  QWidget::resizeEvent(e);
}

void
Ui::onReturnPressed(void)
{
  m_UrlBar.selectAll();
  emit loadUrl(m_UrlBar.text());
}

void
Ui::mousePressEvent(QMouseEvent * e)
{
  if (e->button() == Qt::LeftButton) {
    m_DoMove = true;
    m_MoveOffset = e->pos();
    QApplication::setOverrideCursor(Qt::SizeAllCursor);
  }
}

void
Ui::mouseReleaseEvent(QMouseEvent * e)
{
  if (e->button() == Qt::LeftButton) {
    m_DoMove = false;
    QApplication::restoreOverrideCursor();
  }
}

void
Ui::mouseMoveEvent(QMouseEvent * e)
{
  if (m_DoMove) {
    emit moveBy(e->pos() - m_MoveOffset);
  }
}

}; // namespace Quadro

#endif // _QUADRO_UI_CPP
