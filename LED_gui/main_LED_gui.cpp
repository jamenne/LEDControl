#include "LED_gui.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    
  QApplication app(argc, argv);  
    
  LED_App window;

  window.resize(300, 190);
  window.setWindowTitle("LED");
  window.show();

  return app.exec();
}