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
	int volumeTimeIntervalIndex(const std::string& str);
	int priceTimeIntervalIndex(const std::string& str);
	bool vaildTVolumeTime(const std::string& str); 
	bool vaildPriceTime(const std::string& str); 

};



#endif