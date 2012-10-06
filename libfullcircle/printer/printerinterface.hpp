#ifndef LIBFULLCIRCLE_PRINTER_PRINTERINTERFACE_HPP
#define LIBFULLCIRCLE_PRINTER_PRINTERINTERFACE_HPP 1

#include <libfullcircle/common.hpp>
#include <boost/filesystem.hpp>
namespace bfs=boost::filesystem;

namespace fullcircle {
  class PrinterInterface {
    public:
      typedef boost::shared_ptr<PrinterInterface> Ptr;
      PrinterInterface (bfs::path serial_device);
      virtual ~PrinterInterface(); 

      void print(const std::string& badge);
      void cut_paper();

    private:
      PrinterInterface (const PrinterInterface& original);
      PrinterInterface& operator= (const PrinterInterface& rhs);
      void open_device(bfs::path serial_device);
      bfs::path _serial_device;
      int _tty_fd;
  };
};


#endif /* LIBFULLCIRCLE_PRINTER_PRINTERINTERFACE_HPP */

