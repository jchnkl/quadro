#include <QApplication>
#include "Browser.hpp"

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("Quadro");
  QApplication::setApplicationVersion("1.0");

  Config config(app);
  Browser browser(config);
  return app.exec();
}
