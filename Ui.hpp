#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

#include <cmath>

#include <QApplication>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

namespace Quadro {

enum Direction
  { None
  , Left
  , Right
  , Top
  , Bottom
  , TopRight
  , TopLeft
  , BottomRight
  , BottomLeft
  };

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
      , m_EmitResizeSignal(false)
    {
      this->setMouseTracking(true);
    }

    Qt::CursorShape
    cursorShape(const QPoint & abspos)
    {
      switch (direction(abspos)) {
        case Left:
        case Right:
          return Qt::SizeHorCursor;
        case Top:
        case Bottom:
          return Qt::SizeVerCursor;
        case TopLeft:
        case BottomRight:
          return Qt::SizeFDiagCursor;
        case TopRight:
        case BottomLeft:
          return Qt::SizeBDiagCursor;
        case None:
        default:
          return Qt::WhatsThisCursor;
      };
    }

    Quadro::Direction
    direction(const QPoint & abspos)
    {
      QRect rect = this->geometry();
      QPoint center = QPoint(rect.width() / 2, rect.height() / 2);

      // transpose relative to widget center
      // necessary to get the proper quadrants in the unit circle
      QPoint relpos = QPoint(abspos.x() - center.x(), center.y() - abspos.y());

      // fix aspect ratio for rects where width != height
      relpos.setY(relpos.y() * (rect.width() / (double)rect.height()));

      double angle = std::atan(relpos.y() / (double)relpos.x());

      // 1. quadrant
      if (relpos.x() > 0 && relpos.y() > 0) {
        angle += 0.0;
      // 2. quadrant
      } else if (relpos.x() < 0 && relpos.y() > 0) {
        angle += M_PI;
      // 3. quadrant
      } else if (relpos.x() < 0 && relpos.y() < 0) {
        angle += M_PI;
      // 4. quadrant
      } else if (relpos.x() > 0 && relpos.y() < 0) {
        angle += 2 * M_PI;
      }

      // const double corner  = 0.01 * M_PI;
      const double corner  = 5.0 * (M_PI / 180.0);
      const double rad_000 = 0;
      const double rad_045 = M_PI_4;
      const double rad_090 = M_PI_2;
      const double rad_135 = M_PI_2 + M_PI_4;
      const double rad_180 = M_PI;
      const double rad_225 = M_PI + M_PI_4;
      const double rad_270 = M_PI + M_PI_2;
      const double rad_315 = M_PI + M_PI_2 + M_PI_4;
      const double rad_360 = 2 * M_PI;

      bool right  = (angle >= rad_000 && angle <= rad_045)
                 || (angle >= rad_315 && angle <= rad_360);
      bool left   =  angle >= rad_135 && angle <= rad_225;
      bool top    =  angle >= rad_045 && angle <= rad_135;
      bool bottom =  angle >= rad_225 && angle <= rad_315;

      bool top_right    = angle > rad_045 - corner && angle < rad_045 + corner;
      bool top_left     = angle > rad_135 - corner && angle < rad_135 + corner;
      bool bottom_left  = angle > rad_225 - corner && angle < rad_225 + corner;
      bool bottom_right = angle > rad_315 - corner && angle < rad_315 + corner;

      if (left) {
        return Left;
      } else if (right) {
        return Right;
      } else if (top) {
        return Top;
      } else if (bottom) {
        return Bottom;
      } else if (top_right) {
        return TopRight;
      } else if (top_left) {
        return TopLeft;
      } else if (bottom_right) {
        return BottomRight;
      } else if (bottom_left) {
        return BottomLeft;
      }

      return None;
    }

  protected:
    void
    mouseMoveEvent(QMouseEvent * e)
    {
      emit mouseMoved(e);
      Qt::CursorShape shape = cursorShape(e->pos());
      if (shape != m_CurrentShape) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(shape);
        m_CurrentShape = shape;
      }
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    mousePressEvent(QMouseEvent * e)
    {
      emit mousePressed(e);
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    mouseReleaseEvent(QMouseEvent * e)
    {
      emit mouseReleased(e);
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    enterEvent(QEvent *)
    {
      QApplication::setOverrideCursor(cursorShape(this->cursor().pos()));
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

    void
    leaveEvent(QEvent *)
    {
      QApplication::restoreOverrideCursor();
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
    }

  private:
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
  : public QGroupBox
{
  Q_OBJECT

  signals:
    void loadUrl(const QString & url);
    void changed(bool show);

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
      , m_DoMouseMove(false)
    {
      this->setObjectName("QuadroUi");
      this->setWindowFlags(Qt::FramelessWindowHint);
      this->setStyleSheet("QGroupBox#QuadroUi { border: 0px; background-color: rgba(0,0,0,25%); }");

      m_HideButton.setIcon(QIcon("app_hide.svg"));

      m_UrlBar.setWindowFlags(Qt::FramelessWindowHint);

      m_GridLayout.setMargin(0);
      m_GridLayout.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
      m_GridLayout.addWidget(&m_UrlBar, 0, 0);
      m_GridLayout.addWidget(&m_HideButton, 0, 1);

      this->setLayout(&m_GridLayout);

      connect(&m_HideButton, &QPushButton::pressed, this, &Ui::onHide);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Ui::onReturnPressed);

      this->hide();
    }

  protected:
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
    QGridLayout m_GridLayout;
    QLineEdit m_UrlBar;
    QPushButton m_HideButton;
}; // class Ui

}; // namespace Quadro

#endif // _QUADRO_BROWSER_HPP
