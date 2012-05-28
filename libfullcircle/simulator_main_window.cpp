#include "simulator_main_window.hpp"
#include <iostream>

using namespace fullcircle;

SimulatorMainWindow::SimulatorMainWindow(
    Sequence::Ptr seq
  ) : _ui(new Ui::SimpleUI)
    , _scene(new QGraphicsScene())
    , _seq(seq)
{
  _ui->setupUi(this);
  _scene->addText("Uschi");
  _ui->graphics_view->setScene(_scene);
  _ui->frame_slider->setMinimum(0);
  _ui->frame_slider->setMaximum(_seq->size()-1);
  _ui->frame_slider->setSingleStep(1);
  _ui->frame_slider->setValue(0);
  draw_frame(0);
  connect(_ui->frame_slider, SIGNAL(valueChanged(int)),
          this, SLOT(draw_frame(int)));
}

SimulatorMainWindow::~SimulatorMainWindow() {
  delete _scene;
  delete _ui;
}

void SimulatorMainWindow::draw_frame(int frameID) {
  std::cout << "Drawing frame " << frameID << std::endl;
  _scene->clear();
  qreal pixelwidth=_scene->width()/_seq->x_dim();
  qreal pixelheight=_scene->height()/_seq->y_dim();
  fullcircle::Frame::Ptr frame=_seq->get_frame(frameID);
  QPen pen(Qt::green);
  for (uint8_t x=0; x < _seq->x_dim(); ++x) {
    for (uint8_t y=0; y < _seq->y_dim(); ++y) {
      RGB_t pixelcolor=frame->get_pixel(x,y);
      QBrush brush(QColor(
            pixelcolor.red, pixelcolor.green, pixelcolor.blue
            ));
      QRectF pixel(x*pixelwidth, y*pixelheight,
          pixelwidth, pixelheight);
      _scene->addRect(pixel, pen, brush);
    }
  }
}

void SimulatorMainWindow::on_stop_PB_clicked() {
  _scene->clear();
  _scene->addText("ursel");
}

void SimulatorMainWindow::on_play_PB_clicked() {
  _scene->clear();
  _scene->addText("horst");
}

void SimulatorMainWindow::closeEvent(QCloseEvent *event) {
  std::cout << "Goodbye." << std::endl;
}
