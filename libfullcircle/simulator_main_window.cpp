#include "simulator_main_window.hpp"
#include <iostream>

using namespace fullcircle;

SimulatorMainWindow::SimulatorMainWindow(
  ) : _ui(new Ui::SimpleUI)
{
  _ui->setupUi(this);
  _ui->off_PB->setEnabled(false);
}

SimulatorMainWindow::~SimulatorMainWindow() {
  delete _ui;
}

void SimulatorMainWindow::on_refresh_PB_clicked() {
    _ui->temperature_display->setText("foo");
}

void SimulatorMainWindow::on_on_PB_clicked() {
  std::cout << "Switching on..." << std::endl;
  _ui->on_PB->setEnabled(false);
  _ui->off_PB->setEnabled(true);
}

void SimulatorMainWindow::on_off_PB_clicked() {
  std::cout << "Switching off..." << std::endl;
  _ui->on_PB->setEnabled(true);
  _ui->off_PB->setEnabled(false);
}

void SimulatorMainWindow::on_toggle_PB_clicked() {
  std::cout << "Toggling ..." << std::endl;
    _ui->on_PB->setEnabled(false);
    _ui->off_PB->setEnabled(true);
}

void SimulatorMainWindow::closeEvent(QCloseEvent *event) {
  std::cout << "Goodbye." << std::endl;
}
