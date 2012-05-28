#ifndef LIBHEXADISPLAY_MAIN_WINDOW_HPP
#define LIBHEXADISPLAY_MAIN_WINDOW_HPP 1

#include <QtGui>
#include <QVBoxLayout>
#include <libfullcircle/common.hpp>
#include <libfullcircle/ui_simulator.h>

namespace fullcircle {

  class SimulatorMainWindow : public QWidget, private Ui::SimpleUI {
    Q_OBJECT 

    public:
      typedef std::tr1::shared_ptr<SimulatorMainWindow> Ptr;
      SimulatorMainWindow ();
      virtual ~SimulatorMainWindow();
    private slots:
      void on_refresh_PB_clicked();
      void on_on_PB_clicked();
      void on_off_PB_clicked();
      void on_toggle_PB_clicked();

    protected:
      void closeEvent(QCloseEvent *event);
    private:
      SimulatorMainWindow (const SimulatorMainWindow& original);
      SimulatorMainWindow& operator= (const SimulatorMainWindow& rhs);
      Ui::SimpleUI* _ui;
  };

};


#endif /* LIBHEXADISPLAY_MAIN_WINDOW_HPP */

