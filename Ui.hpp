#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

#include <iostream>
#include <memory>

#include <QApplication>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>

// #include "Util.hpp"

// class MouseAction {
//   public:
//     virtual void mouseMoveEvent(QMouseEvent * e) = 0;
//     // virtual void mousePressEvent(QMouseEvent * e) = 0;
//     // virtual void mouseReleaseEvent(QMouseEvent * e) = 0;
// }; // class MouseAction

// class MouseResizable
//   : public MouseAction
// {
//   public:
//     MouseResizable(QWidget * widget)
//       : m_offset(widget->cursor().pos())
//     {}
//
//     void
//     mouseMoveEvent(QMouseEvent * e)
//     {
//       std::cerr << __PRETTY_FUNCTION__ << ": " << e->x() << "x" << e->y() << std::endl;
//     }
//
//     void
//     mousePressEvent(QMouseEvent * e)
//     {
//       std::cerr << __PRETTY_FUNCTION__ << ": " << e->x() << "x" << e->y() << std::endl;
//     }
//
//     void
//     mouseReleaseEvent(QMouseEvent * e)
//     {
//       std::cerr << __PRETTY_FUNCTION__ << ": " << e->x() << "x" << e->y() << std::endl;
//     }
//
//   private:
//     QPoint m_offset;
// }; // class MouseResizable

namespace Browser {

class Ui
  : public QGroupBox
{
  Q_OBJECT

  public:
    Ui(void)
    {
      m_MoveButton.setIcon(QIcon("move.svg"));
      m_MoveButton.setCheckable(true);
      m_ResizeButton.setIcon(QIcon("resize.svg"));
      m_ResizeButton.setCheckable(true);

      m_Layout.setMargin(0);
      m_Layout.addWidget(&m_UrlBar);
      m_Layout.addWidget(&m_MoveButton);
      m_Layout.addWidget(&m_ResizeButton);

      this->setLayout(&m_Layout);

      connect(&m_MoveButton, &QPushButton::pressed, this, &Ui::onMoveButtonPressed);
      connect(&m_ResizeButton, &QPushButton::pressed, this, &Ui::onResizeButtonPressed);
    }

    void
    checkMoveButton(bool check)
    {
      m_MoveButton.setChecked(check);
      m_ResizeButton.setChecked(check && m_ResizeButton.isChecked());
    }

    void
    checkResizeButton(bool check)
    {
      m_ResizeButton.setChecked(check);
      m_MoveButton.setChecked(check && m_MoveButton.isChecked());
    }

    bool
    moveIsChecked(void)
    {
      return m_MoveButton.isChecked();
    }

    bool
    resizeIsChecked(void)
    {
      return m_ResizeButton.isChecked();
    }

    bool
    isMoveButton(QObject * obj)
    {
      return obj == &m_MoveButton;
    }

    const QPushButton &
    moveButton(void) const
    {
      return m_MoveButton;
    }

    QLineEdit &
    urlBar(void)
    {
      return m_UrlBar;
    }

    // std::shared_ptr<QObject>
    // moveEventFilter(QWidget * widget)
    // {
    //   return std::make_shared<MoveEventFilter>(this, widget);
    // }

  protected:
    void
    onMoveButtonPressed(void)
    {
      checkMoveButton(m_MoveButton.isChecked());
      emit moveButtonChecked(m_MoveButton.isChecked());
    }

    void
    onResizeButtonPressed(void)
    {
      checkResizeButton(m_ResizeButton.isChecked());
      emit resizeButtonChecked(m_ResizeButton.isChecked());
    }

  private:
    QHBoxLayout m_Layout;
    QLineEdit m_UrlBar;
    QPushButton m_MoveButton;
    QPushButton m_ResizeButton;

  signals:
    void moveButtonChecked(bool isChecked);
    void resizeButtonChecked(bool isChecked);
}; // class Ui

class MoveEventFilter
  : public QObject
{
  public:
    MoveEventFilter(Ui * ui, QWidget * widget)
      : m_move(false)
      , m_ui(ui)
      , m_widget(widget)
    {
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
      QApplication::setOverrideCursor(Qt::SizeAllCursor);
      withAllChildren(m_widget, [&](QObject * child) {
          if (child != &m_ui->moveButton()) {
            child->installEventFilter(this);
          }
      });

    }

    ~MoveEventFilter(void)
    {
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
      withAllChildren(m_widget, [&](QObject * child) {
          child->removeEventFilter(this);
      });
      QApplication::restoreOverrideCursor();
    }

    bool
    eventFilter(QObject * watched, QEvent * event)
    {
      // if (event->type() == QEvent::MouseMove) {
        if (watched == &m_ui->moveButton()) {
          std::cerr << "(watched == &m_ui->m_MoveButton)" << std::endl;
          std::cerr << "watched: 0x" << std::hex << watched << std::dec << std::endl;
          QApplication::restoreOverrideCursor();

          return false;

        // } else if (event->type() == QEvent::MouseMove) {
        } else {
          std::cerr << "(watched != &m_ui->m_MoveButton)" << std::endl;
          // std::cerr << "type: " << event->type() << std::endl;
          std::cerr << "watched: 0x" << std::hex << watched << std::dec << std::endl;
          // QApplication::setOverrideCursor(Qt::SizeAllCursor);
          if (QApplication::overrideCursor() == 0) {
            QApplication::setOverrideCursor(Qt::SizeAllCursor);
          }
        }
      // }

          // if (QApplication::overrideCursor() == 0) {
          //   QApplication::setOverrideCursor(Qt::SizeAllCursor);
          // }

      if (m_move && event->type() == QEvent::MouseMove) {
        QMouseEvent * e = static_cast<QMouseEvent *>(event);
        // auto old_pos = m_widget->pos();
        // auto diff = e->pos() - m_offset;
        auto pos = m_widget->pos() + e->pos() - m_offset; // (m_offset - e->pos());
        // std::cerr << __PRETTY_FUNCTION__ << ": QEvent::MouseMove" << std::endl;
        // std::cerr << "\tdiff " << diff.x() << "x" << diff.y() << std::endl;
        // std::cerr << "\tmove from " << old_pos.x() << "x" << old_pos.y()
        //           << " to " << pos.x() << "x" << pos.y() << std::endl;
        m_widget->move(pos);
        return true;

      } else if (event->type() == QEvent::MouseButtonPress) {
        // std::cerr << __PRETTY_FUNCTION__ << ": QEvent::MouseButtonPress" << std::endl;
        QMouseEvent * e = static_cast<QMouseEvent *>(event);
        m_move = true;
        m_offset = e->pos();
        return true;

      } else if (event->type() == QEvent::MouseButtonRelease) {
        // std::cerr << __PRETTY_FUNCTION__ << ": QEvent::MouseButtonRelease" << std::endl;
        m_move = false;
        return true;

      } else {
        // std::cerr << __PRETTY_FUNCTION__ << ": else: " << event->type() << std::endl;
        return false;

      }

      return false;
    }

  protected:
    static
    void
    withAllChildren(QObject * obj, std::function<void(QObject *)> f)
    {
      for (auto child : obj->children()) {
        f(child);
        withAllChildren(child, f);
      }
    }

  private:
    bool m_move;
    QPoint m_offset;
    Ui * m_ui;
    QWidget * m_widget;
}; // class MoveEventFilter

}; // namespace Browser

#endif // _QUADRO_BROWSER_HPP
