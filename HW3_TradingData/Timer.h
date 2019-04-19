#ifndef TIMER
#define TIMER

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;


class Timer
{
private:
	float seconds;
	int hh, mm;
	float ss;
public:
	int stringToSec(const std::string& str);
	int timeIntervalIndex(const std::string& str);

};



#endif