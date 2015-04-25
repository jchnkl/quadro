#include <QApplication>
#include "WebView.hpp"

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("Quadro");
  QApplication::setApplicationVersion("1.0");

  Quadro::Config config(app);
  Quadro::WebView webview(config);
  return app.exec();
}
