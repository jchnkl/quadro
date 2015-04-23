#ifndef _QUADRO_UI_HPP
#define _QUADRO_UI_HPP

#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

namespace Browser {

class Ui
  : public QGroupBox
{
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

  private:
    QHBoxLayout m_Layout;
    QLineEdit m_UrlBar;
    QPushButton m_MoveButton;
    QPushButton m_ResizeButton;
}; // class Ui

}; // namespace Browser

#endif // _QUADRO_BROWSER_HPP
