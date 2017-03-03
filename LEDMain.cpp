#include <iostream>
#include "LED.h"


using namespace std;

int main(){

  LED* led = new LED();

  double current=0;
  cout << "Which current you would like to use?" << endl;
  cin >> current;
  led->SetCurr(current);

  cout << "Switching LED on..." << endl;

  led->LEDon();

  char input;
  cout << "Do you want to switch them off again? [y/n]" << endl;
  cin >> input;

  if((input=='y') | (input=='Y')){

  	cout << "Switching LED off..." << endl;
  	led->LEDoff();

  }

  else{
    cout << "Switching LED off..." << endl;
    led->LEDoff();
  }

  return 0;
}