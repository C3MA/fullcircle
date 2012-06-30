#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[1]) {
	int i;
	char buffer[128];
  struct termios tio;
  int tty_fd;
	
  memset(&tio,0,sizeof(tio));
  tio.c_iflag=0;
  tio.c_oflag=0;
  tio.c_cflag=CS8|CREAD|CLOCAL;			// 8n1, see termios.h for more information
  tio.c_lflag=0;
  tio.c_cc[VMIN]=1;
  tio.c_cc[VTIME]=5;

  tty_fd=open("/dev/cu.usbserial-FTFO10Y3", O_RDWR);			//  | O_NONBLOCK);
  cfsetospeed(&tio,B9600);			// 9600 baud
  cfsetispeed(&tio,B9600);			// 9600 baud

  tcsetattr(tty_fd,TCSANOW,&tio);

	if(tty_fd == -1) {
		fprintf(stderr,"Cannot open serial device\n");
		return 1;
	}
  tcsetattr(tty_fd,TCSANOW,&tio);
	write(tty_fd,"Ruebezahl Naturkost\n",strlen("Ruebezahl Naturkost\n"));

	for(i=0; i<8; i++) {
		sprintf(buffer, "%.4f EUR\n",(double)i*.987654321);
		write(tty_fd,buffer,strlen(buffer));
	}
  std::ostringstream oss;
  oss << 
//    "  __      " << "\n" <<
//    " / _| ___ " << "\n" <<
//    "| |_ / __|" << "\n" <<
//    "|  _| (__ " << "\n" <<
//    "|_|  \\___|"<< "\n";
//"  __       _ _ " << "\n" <<
//" / _|_   _| | |" << "\n" <<
//"| |_| | | | | |" << "\n" <<
//"|  _| |_| | | |" << "\n" <<
//"|_|  \\__,_|_|_|" << "\n\n" <<
//"      _          _       " << "\n" <<
//"  ___(_)_ __ ___| | ___  " << "\n" <<
//" / __| | '__/ __| |/ _ \\" << "\n" << 
//"| (__| | | | (__| |  __/ " << "\n" <<
//" \\___|_|_|  \\___|_|\\___|" << "\n\n\n\n\n\n"; 
"  __       _ _        _          _       \n" <<
" / _|_   _| | |   ___(_)_ __ ___| | ___  \n" <<
"| |_| | | | | |  / __| | '__/ __| |/ _ \\ \n" <<
"|  _| |_| | | | | (__| | | | (__| |  __/ \n" <<
"|_|  \\__,_|_|_|  \\___|_|_|  \\___|_|\\___| \n" <<
"\n\n";
  std::string logo(oss.str());

  for( uint8_t i = 0; i < 3; i++) {
    write(tty_fd, logo.c_str(), logo.length());
  }

	write(tty_fd,    "\\|/-aaaäää Test Text\n\n\n\n\n",
			strlen("ääääaaaäää Test Text\n\n\n\n\n"));
	
	write(tty_fd,"\x1D\x56\x01",3);
	close(tty_fd);
	
	return 0;
}

