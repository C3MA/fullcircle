#ifndef LIBHEXADISPLAY_MAIN_WINDOW_HPP
#define LIBHEXADISPLAY_MAIN_WINDOW_HPP 1

#include <QtGui>
#include <QGraphicsScene>
#include <libfullcircle/common.hpp>
#include <libfullcircle/ui_simulator.h>
#include <libfullcircle/sequence.hpp>

namespace fullcircle {

  class SimulatorMainWindow : public QWidget, private Ui::SimpleUI {
    Q_OBJECT 

    public:
      typedef std::tr1::shared_ptr<SimulatorMainWindow> Ptr;
      SimulatorMainWindow (Sequence::Ptr seq);
      virtual ~SimulatorMainWindow();
    private slots:
      void on_play_PB_clicked();
      void on_stop_PB_clicked();
      void draw_frame(int frameID);

    protected:
      void closeEvent(QCloseEvent *event);
    private:
      SimulatorMainWindow (const SimulatorMainWindow& original);
      SimulatorMainWindow& operator= (const SimulatorMainWindow& rhs);
      Ui::SimpleUI* _ui;
      QGraphicsScene* _scene;
      Sequence::Ptr _seq;
  };

};


#endif /* LIBHEXADISPLAY_MAIN_WINDOW_HPP */

