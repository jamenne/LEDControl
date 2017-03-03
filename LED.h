#ifndef ____LED__
#define ____LED__

class LED
{
	public:
		LED();
		virtual ~LED();
		void LEDoff();
		void LEDon();
		void LED_func(std::string intref, unsigned char bitmap, double current, char* setaddress);
		bool SetCurr(double c);
		double GetCurr();

	private:
		double _Curr;
};

#endif /* defined(____LED__) */
