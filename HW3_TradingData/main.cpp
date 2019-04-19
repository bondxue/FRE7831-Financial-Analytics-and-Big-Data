#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include <time.h>
#include <stdio.h>
#include <iomanip>

#include "Timer.h"
#include "CsvRow.h"

using namespace std;



int main() 
{
	std::ifstream file("SPY_MAY_2012.csv");
	CsvRow row;


	string tm;
	string strVolumne;
	string timeNow;
	long float vloumne;
	const int MAX_INDEX = 25;
	float sumVolumne[MAX_INDEX+1] = { 0.0 };
	Timer t;
	int timeIndex;
	string type;


	while (file >> row)
	{
		timeNow = row[2];
		strVolumne = row[7];
		type = row[4];
		timeIndex = t.timeIntervalIndex(timeNow);
		if (timeIndex >= 0 && timeIndex <= MAX_INDEX && type == "Trade")
		{
			vloumne = strtof(strVolumne.c_str(), NULL);
			sumVolumne[timeIndex] += vloumne;
			cout << row[1] << "  " << row[2] << "  " << row[4] << "  " << row[7] << "            " << timeIndex << "            " << sumVolumne[timeIndex] << endl;
			
		}
		
			
	}

	cout << "done." << endl;



	for (int i = 0; i <= MAX_INDEX; i++)
	{
		cout << sumVolumne[i] << endl;
	}
	


	//Timer t;
	//std::cout << t.timeIntervalIndex("8:01:01.1") << std::endl;


	system("Pause");
	return 0;
}