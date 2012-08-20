#include "printerinterface.hpp"
#include <libfullcircle/error.hpp>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

using namespace fullcircle;

PrinterInterface::PrinterInterface  (bfs::path serial_device)
  : _serial_device(serial_device) 
{
  open_device(_serial_device);
}


PrinterInterface::~PrinterInterface() {
  close(_tty_fd);
}

void PrinterInterface::open_device(bfs::path serial_device) {
  struct termios tio;

  memset(&tio,0,sizeof(tio));
  tio.c_iflag=0;
  tio.c_oflag=0;
  tio.c_cflag=CS8|CREAD|CLOCAL;			// 8n1, see termios.h for more information
  tio.c_lflag=0;
  tio.c_cc[VMIN]=1;
  tio.c_cc[VTIME]=5;

  _tty_fd=open(serial_device.c_str(), O_RDWR);			//  | O_NONBLOCK);
  cfsetospeed(&tio,B9600);			// 9600 baud
  cfsetispeed(&tio,B9600);			// 9600 baud

  tcsetattr(_tty_fd,TCSANOW,&tio);

  if(_tty_fd == -1) {
    std::ostringstream oss;
    oss << "Cannot open device " << _serial_device;
    throw CommunicationException(oss.str());
  }
  tcsetattr(_tty_fd,TCSANOW,&tio);
}

void PrinterInterface::print(const std::string& badge) {
  if(_tty_fd == -1) {
    std::ostringstream oss;
    oss << "Device " << _serial_device << " not ready";
    throw CommunicationException(oss.str());
  }
  ssize_t success = write(_tty_fd, badge.c_str(), badge.length());
  if (success) {};
}


void PrinterInterface::cut_paper() {
  if(_tty_fd == -1) {
    std::ostringstream oss;
    oss << "Device " << _serial_device << " not ready";
    throw CommunicationException(oss.str());
  }
	ssize_t success = write(_tty_fd,"\n\n\n\n\x1D\x56\x01",7);
  if (success) {};
}
