#include <QApplication>
#include "Browser.hpp"

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("Quadro");
  QApplication::setApplicationVersion("1.0");

  Quadro::Config config(app);
  Quadro::Window browser_window(config);
  return app.exec();
}
