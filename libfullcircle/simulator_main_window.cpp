#include "simulator_main_window.hpp"
#include <iostream>
#include <fstream>

using namespace fullcircle;

SimulatorMainWindow::SimulatorMainWindow(
    bfs::path sequence_file
  ) : _ui(new Ui::SimpleUI)
    , _scene(new QGraphicsScene())
    , _sequence_file(sequence_file)
    , _timer(new QTimer(this))
    , _current_frame(0)
{
  _ui->setupUi(this);
  _ui->graphics_view->setScene(_scene);
  load_sequence();
  draw_frame(_current_frame);
  // connect the slider and the spinbox: reflects the frame number
  connect(_ui->frame_slider, SIGNAL(valueChanged(int)),
          this, SLOT(draw_frame(int)));
  connect(this, SIGNAL(valueChanged(int)),
          _ui->frame_slider, SLOT(setValue(int)));
  connect(_ui->frame_spinbox, SIGNAL(valueChanged(int)),
          this, SLOT(draw_frame(int)));
  connect(this, SIGNAL(valueChanged(int)),
          _ui->frame_spinbox, SLOT(setValue(int)));
  // connect the timer: set to framerate
  connect(_timer, SIGNAL(timeout()), this, SLOT(draw_next_frame()));
}

SimulatorMainWindow::~SimulatorMainWindow() {
  delete _timer;
  delete _scene;
  delete _ui;
}

void SimulatorMainWindow::draw_next_frame() {
  if ((uint32_t)_current_frame < _seq->size()) {
    draw_frame(_current_frame++);
  } else {
    _timer->stop();
  }
}

void SimulatorMainWindow::draw_frame(int frameID) {
  _scene->clear();
  //QSize size=_ui->frame_slider->size();
  //std::cout << "Size: " << size.width() << "x" << size.height() << std::endl;
  //qreal pixelwidth=size.width()/_seq->width();
  //qreal pixelheight=size.height()/_seq->height();
  qreal pixelwidth=800/_seq->width();
  qreal pixelheight=600/_seq->height();
  //std::cout << "Pixel: " << pixelwidth << "x" << pixelheight << std::endl;
  fullcircle::Frame::Ptr frame=_seq->get_frame(frameID);
  QPen pen(Qt::green);
  for (uint8_t x=0; x < _seq->width(); ++x) {
    for (uint8_t y=0; y < _seq->height(); ++y) {
      RGB_t pixelcolor=frame->get_pixel(x,y);
      QBrush brush(
          QColor(pixelcolor.red, pixelcolor.green, pixelcolor.blue)
        );
      QRectF pixel(x*pixelwidth, y*pixelheight,
                  pixelwidth, pixelheight);
      _scene->addRect(pixel, pen, brush);
    }
  }
  emit valueChanged(frameID);
}

void SimulatorMainWindow::on_stop_PB_clicked() {
  _timer->stop();
}

void SimulatorMainWindow::on_play_PB_clicked() {
  if ((unsigned int)_current_frame == _seq->size()) {
    _current_frame = 0;
  }
  int timer_interval =(1000/_seq->fps());
  _timer->start(timer_interval);
}

void SimulatorMainWindow::load_sequence() {
  std::cout << "Loading sequence from file " << _sequence_file << std::endl;
  std::fstream input(_sequence_file.c_str(), std::ios::in | std::ios::binary);
  _seq=fullcircle::Sequence::Ptr(new fullcircle::Sequence(input));
  input.close();
  _ui->frame_slider->setMinimum(0);
  _ui->frame_slider->setMaximum(_seq->size()-1);
  _ui->frame_slider->setSingleStep(1);
  _ui->frame_slider->setValue(0);
  _ui->frame_spinbox->setMinimum(0);
  _ui->frame_spinbox->setMaximum(_seq->size()-1);
  _ui->frame_spinbox->setSingleStep(1);
  _ui->frame_spinbox->setValue(0);
}

void SimulatorMainWindow::on_reload_PB_clicked() {
  load_sequence();
  _current_frame=0;
}

void SimulatorMainWindow::closeEvent(QCloseEvent *event) {
  std::cout << "Goodbye." << std::endl;
}
