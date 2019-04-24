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
	int timeNow, timeStart, timeInterval;
	timeNow = Timer::stringToSec(str);
	timeStart = Timer::stringToSec("09:30:00.0");
	timeInterval = timeNow - timeStart;
	return floor(timeInterval / 900.0); // 900s = 15min
}

int Timer::priceTimeIntervalIndex(const std::string& str)
{
	int timeNow, timeStart, timeInterval;
	timeNow = Timer::stringToSec(str);
	timeStart = Timer::stringToSec("09:30:00.0");
	timeInterval = timeNow - timeStart;
	if (timeInterval % 900 == 0 && timeInterval >= 0)
	{
		return timeInterval / 900;
	}
	else
	{
		return -1; 
	}

}


