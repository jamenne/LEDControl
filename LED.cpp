/* Created by Janine Müller, 28.02.2017

based in code by Morag Williams, July 2015

 A C++ code to replace the existing LED LabVIEW code. The code will turn on and off the LEDs automatically.
 													
 Command line arguments, in order: internal reference, bitmap, current, dac address, USB port.

 	internal reference: 	on or off
 	bitmap: 		to turn on the 1st LED = 1, 2nd = 2, 3rd = 4, ..., 8th = 128, all = 255.
 	current: 		changes the current to, and therefore the brightness of the LEDs (in mA).
 	dac address: 		For *   type *
				DAC A	A
				DAC B	B
				DAC C	C
				DAC D	D
				DAC E	E
				DAC F	F
				DAC G	G  
				DAC H	H
				All DAC	a 
	USBport:		enter the value of the correct USB port for the device.

 Example call: (to turn on all the LEDs with a current of 10mA, use DAC address A and use USB port 0)
 	sudo ./LED on 255 10 A 0

*/

#include <iostream>
#include <stdbool.h>
#include <fstream>
#include "/home/xytable/src/RS232com/RS232communication.h"
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "LED.h"

using namespace std;

LED::LED(){}

LED::~LED(){

}

bool LED::SetCurr(double c){

    if ((c>=0.) && (c<=20.)){

        this->_Curr = c;
        return true;
    }

    else {
        cout << "LED only allows between 0 and 20mA" << endl;
        return false;
    }
}

double LED::GetCurr(){

    return this->_Curr;
}

void LED::LEDoff(){ // switches all LED off

    cout << "Switching LED off..." << endl;

    LED_func("on", 0, 0, "a");

}

void LED::LEDon(){ // switches all LED on

    cout << "Switching LED on with " << this->_Curr << " mA" << endl;

    LED_func("on", 255, this->_Curr, "a");

}

void LED::LED_func(string intref, unsigned char bitmap, double current, char* setaddress){
    
    //values entered in, in order: int reference, bitmap, current, dac address, USB port
    
    /* a check for the number of command line arguments 
    if (argc != 6) {
	cout << "Wrong number of command line arguments." << endl;
	return -1; 
    }*/
    /* initialisation */
    string b, c, d;
    int n, comhandel, dac, var;
    long baudrate, address;
    unsigned char setting, high, low;
    int rest = 150000; // sets the time to wait after writing to the machine before the machine is read.



    /*reading in command line arguments
    intref = argv[1];
    b = argv[2];  // note b, c and d are temporary values for string-to-integer conversion
    bitmap = stoi(b); // converts a string to an integer
    c = argv[3];
    current = stoi(c); 
    setaddress = argv[4]; // for the switch statement to find the binary address
    d = argv[5];
    USBport = stoi(d);*/

    
    /*switch statement to give binary value for the address*/
    switch (*setaddress){
	case 'A' :
		address = 0;
		break;
	case 'B' :
		address = 1;
		break;
	case 'C' :
		address = 2;
		break;
	case 'D' :
		address = 3;
		break;
	case 'E' :
		address = 4;
		break;
	case 'F' :
		address = 5;
		break;	
	case 'G' :
		address = 6;
		break;
	case 'H' :
		address = 7;
		break;	
	case 'a' :
		address = 15;
		break;
	default :
		cout << "Invalid dac address. Please input a value A - H or a for all." << endl;
		return;
    }
    
    /* 1) Read in the service USB port. */
    
    //n = number_of_comports();
    //cout << "No. of comports: " << n <<endl;
    baudrate = 9600; // number is set statistically
    comhandel = OpenRS232_dev("/dev/currentsource", baudrate);
    //cout << comhandel << endl;
    
    /* 2) Take in a string set by the user and write 'DRstring\r\n' to the resource. Wait 150ms. Then read 11 bytes of the resource and output the result. */
    if (intref.compare("on") == 0)
        setting  = 1;
    
    else if (intref.compare("off") == 0)
        setting = 0;
    else {
	cout << "Invalid internal reference. Please input 'on' or 'off'." << endl;
	return;
    }
	
    /* write DRsetting\r\n to the resource */

    unsigned char command[10]="";
    unsigned char commandresult[100]="";
    command[0] = 'D';
    command[1] = 'R';
    command[2] = setting;
    command[3] = '\r';
    command[4] = '\n';
    command[5] = 0;
    //cout << "Command to machine: " << command <<endl;
    var = write(comhandel, command, 5);
    //cout << "Bytes written: " << var << endl;
    usleep (rest);  // allows time for processing
    
    /* read 1st 11bytes of the resource */
    var = read(comhandel, commandresult, 13);
    //cout << "Bytes read: " << var << endl;
    //cout << "Command result: " << commandresult <<endl;
    
    
    /* 3) Read in a user set bitmap. Then write to the resource 'Lbitmap\r\n'. Wait 150ms. Then read the first 12 bytes of the resource and output it. */
    
    /* write Lbitmap\r\n to the resource */
    command[0] = 'L';
    command[1] = bitmap;
    command[2] = '\r';
    command[3] = '\n';
    //cout << '\n' << "2nd Command to machine: " << command <<endl;
    var = write(comhandel, command, 4);
    //cout << "Bytes written: " << var << endl;
    usleep (rest);// allows time for processing
    
    /* read first 12 bytes of the resource*/
    var = read(comhandel, commandresult, 14);
    //cout << "Bytes read: " << var << endl;
    //cout << "2nd Command result: " << commandresult <<endl;
    
    
    /* 4) Read in the user set current value. Multiply it by 1/0.000378 to get the dac value. */
    
    dac = current*(1.0/0.000378);
    
    
    /* 5a) Make the DAC value in the range 0 to 65535, and output this forced DAC value. Split the integer in 2 and output both values (low and high).*/

    if (dac > 65535)
        dac = 65535;
    //cout << '\n' << "Forced DAC value is " << dac << endl;
    
    high = dac >> 8;    // need to split the dac as the machine needs two end arguments: for high, dac is moved 8 bytes to the right; for low the first 8 bytes are taken.
    low = dac & 0xFF;
    /* printing high and low as int, not as hex*/
    unsigned int h, l;
    /*std::stringstream ss;
    ss << std::hex << high;
    ss >> h;
    ss << std::hex << low;
    ss >> l;*/
    //cout << "High: " << high << ", Low: " << low << endl; // change to print out as an int, not hex
    
    
    /* 5b) Write to the resource 'DVaddresshighlow\r\n'. Wait 150ms. Read the first 17 bytes of data in the resource and output them. */
    
    /*write DVaddresshighlow\r\n to the resource*/
    command[0] = 'D';
    command[1] = 'V';
    command[2] = address;
    command[3] = high;
    command[4] = low;
    command[5] = '\r';
    command[6] = '\n';
    //cout << '\n' << "3rd Command to machine: " << command <<endl;
    //cout << "Bytes written: " << var << endl;
    write(comhandel, command, 6);
    
    usleep (rest); // allows time for processing
    
    /*read first 17 bytes of the resource*/
    read(comhandel, commandresult, 19);
    /*output the first 17 bytes of the resource */
    //cout << "Bytes read: " << var << endl;
    //cout << "3rd Command result: " << commandresult <<endl;
    cout << '\n' << endl;
    
    
    /* 6) Close the resource. */
    close(comhandel);
    //cout << "Closing port..." << endl;
    return;
}