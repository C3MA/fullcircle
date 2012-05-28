#include <QApplication>
#include <libfullcircle/common.hpp>
#include <libfullcircle/simulator_main_window.hpp>
#include <iostream>



int main (int argc, char* argv[]) {
  std::cout << "foo." << std::endl;

  QApplication app(argc, argv);
  Q_INIT_RESOURCE(default);
  //app.setStyle("plastique");
  fullcircle::SimulatorMainWindow mainWindow;
  mainWindow.show();
  return app.exec();

  return 0;
}


