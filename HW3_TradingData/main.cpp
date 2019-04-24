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
	string strPrice;
	string dateNow;
	string timeNow;
	long float vloumne;
	float price;
	const int MAX_INDEX = 25;
	float sumVolume[MAX_INDEX+1] = { 0.0 };
	int avgVolumne[MAX_INDEX + 1] = { 0.0 };
	float sumPrice[MAX_INDEX + 1] = { 0.0 };
	float avgPrice[MAX_INDEX + 1] = { 0.0 };


	Timer t;
	int volumeTimeIndex;
	int priceTimeIndex;
	string type;


	while (file >> row)
	{
		dateNow = row[1];
		timeNow = row[2];
		strPrice = row[5];
		strVolumne = row[6]; 
		type = row[4];
		volumeTimeIndex = t.volumeTimeIntervalIndex(timeNow);
		priceTimeIndex = t.priceTimeIntervalIndex(timeNow);
		if (volumeTimeIndex >= 0 && volumeTimeIndex <= MAX_INDEX && type == "Trade")
		{
			price = strtof(strPrice.c_str(), NULL);
			vloumne = strtof(strVolumne.c_str(), NULL);
			sumVolume[volumeTimeIndex] += vloumne;

			// cout << dateNow << "  " << timeNow << "  " << type << "  " << strPrice << "      " << priceTimeIndex << "     "  << strVolumne << "      " << volumeTimeIndex << "       " << sumVolume[volumeTimeIndex] << endl;
			
		}

		if (priceTimeIndex >= 0 && priceTimeIndex <= MAX_INDEX && type == "Trade")
		{
			price = strtof(strPrice.c_str(), NULL);
			sumPrice[priceTimeIndex] += price;
		}

				
	}

	cout << "done." << endl;

	cout << "===================================" << endl;
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		avgVolumne[i] = int(sumVolume[i] / 14.0);
		cout << avgVolumne[i] <<endl;
	}

	cout << "===================================" << endl;
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		avgPrice[i] = int(sumPrice[i] / 14.0);
		cout << avgPrice[i] << endl;
	}
	


	//Timer t;
	//std::cout << t.timeIntervalIndex("8:01:01.1") << std::endl;


	system("Pause");
	return 0;
}