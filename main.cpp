#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QWebView>

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QWebView browserView;

  // create & set transparent palette for browser window
  QPalette palette = browserView.palette();
  palette.setBrush(QPalette::Base, Qt::transparent);
  browserView.page()->setPalette(palette);

  // enable transparency for underlying window
  browserView.setAttribute(Qt::WA_TranslucentBackground, true);
  browserView.load(QUrl(argv[1]));

  QLineEdit browserUriBar;

  QGridLayout browserLayout;
  browserLayout.addWidget(&browserUriBar, 0, 0);
  browserLayout.addWidget(&browserView, 1, 0);

  QGroupBox browserGroup;
  browserGroup.setLayout(&browserLayout);

  browserGroup.show();

  return app.exec();
}
