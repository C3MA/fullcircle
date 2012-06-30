#include <libfullcircle/common.hpp>
#include <libfullcircle/printer/printerinterface.hpp>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[1]) {
  fullcircle::PrinterInterface::Ptr printer(
      new fullcircle::PrinterInterface("/dev/cu.usbserial-FTFO10Y3"));

  std::ostringstream oss;
  oss << 
"  __       _ _        _          _       \n" <<
" / _|_   _| | |   ___(_)_ __ ___| | ___  \n" <<
"| |_| | | | | |  / __| | '__/ __| |/ _ \\ \n" <<
"|  _| |_| | | | | (__| | | | (__| |  __/ \n" <<
"|_|  \\__,_|_|_|  \\___|_|_|  \\___|_|\\___| \n" <<
"\n\n";
  std::string logo(oss.str());

  printer->print(logo);
  printer->cut_paper();

	return 0;
}

