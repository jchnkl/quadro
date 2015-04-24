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
    // Ui(void)
    Ui(QWidget * parent)
      : QGroupBox(parent)
      // , m_SpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum)
    {
      this->setObjectName("Ui");
      this->setWindowFlags(Qt::FramelessWindowHint);
      this->setStyleSheet("QGroupBox#Ui { border: 0px; background-color: rgba(0,0,0,25%); }");
      // this->setStyleSheet("QGroupBox#Ui { background-color: rgba(0,0,0,25%); }");

      m_MoveButton.setIcon(QIcon("move.svg"));
      // m_MoveButton.setCheckable(true);
      m_ResizeButton.setIcon(QIcon("resize.svg"));
      // m_ResizeButton.setCheckable(true);

      // m_UrlBar.setStyleSheet("border: 0px;");
      // m_MoveButton.setStyleSheet("border: 0px;");
      // m_ResizeButton.setStyleSheet("border: 0px;");

      m_UrlBar.setWindowFlags(Qt::FramelessWindowHint);
      m_MoveButton.setWindowFlags(Qt::FramelessWindowHint);
      m_ResizeButton.setWindowFlags(Qt::FramelessWindowHint);

      /*
      m_HBoxLayout.setMargin(0);
      // m_HBoxLayout.setSpacing(0);
      m_HBoxLayout.addWidget(&m_UrlBar);
      m_HBoxLayout.addWidget(&m_MoveButton);
      m_HBoxLayout.addWidget(&m_ResizeButton);

      // m_HBoxGroup.setLayout(&m_HBoxLayout);

      m_VBoxLayout.setMargin(0);
      // m_VBoxLayout.setSpacing(0);
      // m_VBoxLayout.addWidget(&m_HBoxGroup);
      m_VBoxLayout.addLayout(&m_HBoxLayout);
      m_VBoxLayout.addStretch();

      this->setLayout(&m_VBoxLayout);
      */

      m_GridLayout.setMargin(0);
      // m_GridLayout.setSpacing(0);
      m_GridLayout.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
      m_GridLayout.addWidget(&m_UrlBar, 0, 0);
      m_GridLayout.addWidget(&m_MoveButton, 0, 1);
      m_GridLayout.addWidget(&m_ResizeButton, 0, 2);

      // m_GridLayout.setRowMinimumHeight(0, m_UrlBar.height());
      // m_GridLayout.setRowStretch(0, 0);
      // m_GridLayout.setRowStretch(1, 1);

      this->setLayout(&m_GridLayout);

      connect(&m_MoveButton, &QPushButton::pressed, this, &Ui::onMoveButtonPressed);
      connect(&m_ResizeButton, &QPushButton::pressed, this, &Ui::onResizeButtonPressed);
    }

    void
    checkMoveButton(bool check)
    {
      // m_MoveButton.setChecked(check);
      // m_ResizeButton.setChecked(check && m_ResizeButton.isChecked());
    }

    void
    checkResizeButton(bool check)
    {
      // m_ResizeButton.setChecked(check);
      // m_MoveButton.setChecked(check && m_MoveButton.isChecked());
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
      // checkMoveButton(m_MoveButton.isChecked());
      emit moveButtonPressed();
    }

    void
    onResizeButtonPressed(void)
    {
      // checkResizeButton(m_ResizeButton.isChecked());
      emit resizeButtonPressed();
    }

  private:
    QGridLayout m_GridLayout;
    QHBoxLayout m_HBoxLayout;
    QVBoxLayout m_VBoxLayout;
    QGroupBox m_HBoxGroup;
    // QSpacerItem m_SpacerItem;
    QLineEdit m_UrlBar;
    QPushButton m_MoveButton;
    QPushButton m_ResizeButton;
}; // class Ui

}; // namespace Browser

#endif // _QUADRO_BROWSER_HPP
