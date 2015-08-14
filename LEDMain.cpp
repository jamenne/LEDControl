#include <iostream>
#include "LED.h"


using namespace std;

int main(){

	double current=0;
    cout << "Which current you would like to use?" << endl;
    cin >> current;

  	cout << "Switching LED on..." << endl;

  	LEDon(current);

  	char input;
  	cout << "Do you want to switch them off again? [y/n]" << endl;
  	cin >> input;

  	if((input=='y') | (input=='Y')){

  		cout << "Switching LED of..." << endl;
  		LEDoff();

  	}

  	else if((input=='n') | (input=='N')){
  		
  	}

    return 0;
}