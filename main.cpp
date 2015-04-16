#include <QApplication>
#include <QWebView>

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QWebView view;

  // create & set transparent palette for browser window
  QPalette palette = view.palette();
  palette.setBrush(QPalette::Base, Qt::transparent);
  view.page()->setPalette(palette);

  // enable transparency for underlying window
  view.setAttribute(Qt::WA_TranslucentBackground, true);
  view.show();
  view.load(QUrl(argv[1]));

  return app.exec();
}
