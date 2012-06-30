#ifndef LIBFULLCIRCLE_PRINTER_PRINTERINTERFACE_HPP
#define LIBFULLCIRCLE_PRINTER_PRINTERINTERFACE_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {
  class PrinterInterface {
    public:
      typedef std::tr1::shared_ptr<PrinterInterface> Ptr;
      PrinterInterface (const std::string& serial_device);
      virtual ~PrinterInterface(); 

      void print(const std::string& badge);
      void cut_paper();

    private:
      PrinterInterface (const PrinterInterface& original);
      PrinterInterface& operator= (const PrinterInterface& rhs);
      void open_device(const std::string& serial_device);
      std::string _serial_device;
      int _tty_fd;
      
  };
};


#endif /* LIBFULLCIRCLE_PRINTER_PRINTERINTERFACE_HPP */

