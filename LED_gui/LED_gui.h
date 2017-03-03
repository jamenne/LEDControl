#pragma once

#include <QWidget>


// LED Class
#include "../LED.h"

class LED_App : public QWidget {
    
	Q_OBJECT

	public:
		LED_App(QWidget *parent = 0);
		virtual ~LED_App() {};
	
	private:
		LED *led;

};