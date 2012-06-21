#ifndef LIBHEXADISPLAY_MAIN_WINDOW_HPP
#define LIBHEXADISPLAY_MAIN_WINDOW_HPP 1

#include <QtGui>
#include <QGraphicsScene>
#include <libfullcircle/common.hpp>
#include <libfullcircle/ui_simulator.h>
#include <libfullcircle/sequence.hpp>
#include <boost/filesystem.hpp>

namespace bfs=boost::filesystem;

namespace fullcircle {

  class SimulatorMainWindow : public QWidget, private Ui::SimpleUI {
    Q_OBJECT 

    public:
      typedef std::tr1::shared_ptr<SimulatorMainWindow> Ptr;
      SimulatorMainWindow (bfs::path sequence_file);
      virtual ~SimulatorMainWindow();

    private slots:
      void on_play_PB_clicked();
      void on_stop_PB_clicked();
      void on_reload_PB_clicked();
      void draw_frame(int frameID);
      void draw_next_frame();

    signals:
      void valueChanged(int newValue);

    protected:
      void closeEvent(QCloseEvent *event);
    private:
      SimulatorMainWindow (const SimulatorMainWindow& original);
      SimulatorMainWindow& operator= (const SimulatorMainWindow& rhs);
      void load_sequence();
      Ui::SimpleUI* _ui;
      QGraphicsScene* _scene;
      Sequence::Ptr _seq;
      bfs::path _sequence_file;
      QTimer* _timer;
      int _current_frame;
  };

};


#endif /* LIBHEXADISPLAY_MAIN_WINDOW_HPP */

