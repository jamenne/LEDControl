#include "RS232communication.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

struct termios oldtio,newtio;

int number_of_comports(){
	char search[50],output[2];
	sprintf(search,"ls /dev/ttyUSB* | wc -w"); // get word count of list of newtioUSB* devices
	FILE* pipe = popen(search, "r");
	if (!pipe) perror("popen()");
	fscanf(pipe,"%s", output);
	pclose(pipe);
	string output1 = output; // convert char array to string
	istringstream buf(output1);
	int n;
	buf >> n; // convert string to int
	return n;
}

//Usage: comhandel = OpenRS232(baudrate, USBport); baudrate statically set to 9600
int OpenRS232(long BaudRate, int PortNum){
	int ComHandle;
	std::string text = "/dev/ttyUSB" + to_string(PortNum);
	ComHandle = open( text.c_str() , O_RDWR | O_NOCTTY);// | O_NONBLOCK);
	cout << "Comhandle: " << ComHandle << endl;
	if(ComHandle<0) return INVALID_HANDLE_VALUE;
	else
	{

		cout << "Port " << PortNum << " is open..." <<endl;
		tcgetattr(ComHandle ,&oldtio); /* save current serial port settings */
		//bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
		memset (&newtio, 0, sizeof(newtio));

		cfsetospeed (&newtio, (speed_t)B9600);
		cfsetispeed (&newtio, (speed_t)B9600);

		/* Setting other Port Stuff */
		newtio.c_cflag     &=  ~PARENB;        // Make 8n1
		newtio.c_cflag     &=  ~CSTOPB;
		newtio.c_cflag     &=  ~CSIZE;
		newtio.c_cflag     |=  CS8;
		
		newtio.c_cflag     &=  ~CRTSCTS;       // no flow control
		newtio.c_cc[VMIN]      =   1;                  // read block (characters)
		newtio.c_cc[VTIME]     =   8;                  // n*100msec read timeout
		newtio.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
		fcntl(ComHandle, F_SETFL, FNDELAY);
        /*
          BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
          CRTSCTS : output hardware flow control (only used if the cable has
                    all necessary lines. See sect. 7 of Serial-HOWTO)
          CS8     : 8n1 (8bit,no parity,1 stopbit)
          CLOCAL  : local connection, no modem contol
          CREAD   : enable receiving characters
        
         newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
        
          IGNPAR  : ignore bytes with parity errors
          ICRNL   : map CR to NL (otherwise a CR input on the other computer
                    will not terminate input)
          otherwise make device raw (no other input processing)
        
         newtio.c_iflag = IGNPAR | ICRNL;
        
         Raw output.
        
         newtio.c_oflag = 0;
					//set input mode (non-canonical, no echo...)
         newtio.c_lflag = 0;
        
          ICANON  : enable canonical input
          disable all echo functionality, and don't send signals to calling program
        
         //newtio.c_lflag = ICANON;
        
          initialize all control characters
          default values can be found in /usr/include/termios.h, and are given
          in the comments, but we don't need them here
        
         newtio.c_cc[VTIME]    = 5;     // inter-character timer 100ms 
         newtio.c_cc[VMIN]     = 1;     // blocking read until n character arrives 
        
          now clean the modem line and activate the settings for the port
        */
				 cfmakeraw(&newtio);
         tcflush(ComHandle, TCIFLUSH);
         tcsetattr(ComHandle ,TCSANOW, &newtio);

	return ComHandle;
  }
}


//Close the serial port
// Usage: close(comhandel);
void CloseRS232(int Handle)
{
	close(Handle);
}

