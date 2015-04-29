#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

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
    UiFrame(QWidget * parent);

  protected:
    void
    mouseMoveEvent(QMouseEvent * e);

    void
    mousePressEvent(QMouseEvent * e);

    void
    mouseReleaseEvent(QMouseEvent * e);

    void
    enterEvent(QEvent *);

    void
    leaveEvent(QEvent *);

  private:
    bool m_DoResize;
    QPoint m_ResizeOffset;
    Direction m_ResizeDirection;
}; // class UiFrame

class UiWidget
  : public QWidget
{
  public:
    UiWidget(QWidget * parent);
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
    onUrlChanged(const QUrl & url);

    void
    onShow(void);

    void
    onHide(void);

  public:
    Ui(QWidget * parent);

  protected:
    void
    resizeEvent(QResizeEvent * e);

    void
    onReturnPressed(void);

    void
    mousePressEvent(QMouseEvent * e);

    void
    mouseReleaseEvent(QMouseEvent * e);

    void
    mouseMoveEvent(QMouseEvent * e);

  private:
    UiFrame m_Frame;
    UiWidget m_UiWidget;
    QGridLayout m_GridLayout;
    QLineEdit m_UrlBar;
    QPushButton m_HideButton;

    bool m_DoMove;
    QPoint m_MoveOffset;
}; // class Ui

}; // namespace Quadro

#endif // _QUADRO_UI_HPP
