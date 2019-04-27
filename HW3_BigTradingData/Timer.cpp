#include "Timer.h"
#include <iostream>
#include <string>
#include <math.h>


using namespace std;

int Timer::stringToSec(const std::string& str)
{
	int i = 0;
	int res = -1;
	int tmp = 0;
	int state = 0;

	while (str[i] != '.') {
		// If we got a digit
		if (str[i] >= '0' && str[i] <= '9') {
			tmp = tmp * 10 + (str[i] - '0');
		}
		// Or if we got a colon
		else if (str[i] == ':') {
			// If we were reading the hours
			if (state == 0) {
				res = 3600 * tmp;
			}
			// Or if we were reading the minutes
			else if (state == 1) {
				if (tmp > 60) {
					return -1;
				}
				res += 60 * tmp;
			}
			// Or we got an extra colon
			else {
				return -1;
			}

			state++;
			tmp = 0;
		}
		// Or we got something wrong
		else {
			return -1;
		}
		i++;
	}

	// If we were reading the seconds when we reached the end
	if (state == 2 && tmp < 60.0) {
		return res + tmp;
	}
	// Or if we were not, something is wrong in the given string
	else {
		return -1;
	}
}

int Timer::volumeTimeIntervalIndex(const std::string& str)
{
	int timeNow = Timer::stringToSec(str);
	int timeStart = Timer::stringToSec("09:30:00.0");
	return floor((timeNow - timeStart)/ 900.0); // 900s = 15min
}

int Timer::priceTimeIntervalIndex(const std::string& str)
{
	int timeNow = Timer::stringToSec(str);
	int timeStart = Timer::stringToSec("09:30:00.0");
	int timeInterval = timeNow - timeStart;

	if (timeInterval % 900 == 0 && (timeNow - timeStart) >= 0 && (timeNow - timeStart) < 23400)
	{
		return timeInterval / 900;
	}
	else
	{
		return -1; 
	}

}

bool Timer::vaildPriceTime(const std::string& str)
{
	int timeNow = Timer::stringToSec(str);
	int timeStart = Timer::stringToSec("09:30:00.0");
	if ((timeNow - timeStart) % 900 == 0 && (timeNow - timeStart) >= 0 && (timeNow - timeStart) < 23400)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Timer::vaildTVolumeTime(const std::string& str)
{
	int timeNow = Timer::stringToSec(str);
	int timeStart = Timer::stringToSec("09:30:00.0");
	if ((timeNow - timeStart) >= 0 && (timeNow - timeStart) < 23400) // time btween 9:30 am and 4:00 pm
	{
		return true;
	}
	else
	{
		return false;
	}

}


