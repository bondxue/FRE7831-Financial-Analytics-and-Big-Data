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
	string dateStart = " ";
	string timeNow;
	long float vloumne;
	float price;
	const int MAX_INDEX = 25;
	float sumVolume[MAX_INDEX+1] = { 0.0 };
	int avgVolumne[MAX_INDEX + 1] = { 0.0 };
	float Price[MAX_INDEX + 1] = { 0.0 };
	string Time[MAX_INDEX + 1];
	int index = -1; // current index in Price array


	Timer t;
	int volumeTimeIndex;
	int priceTimeIndex;
	string type;

	while (file >> row)
	{

		dateNow = row[1];
		if (dateNow == "22-MAY-2012")
		{
			break;
		}

		// for monitoring process 
		if (dateNow != dateStart)
		{
			cout << dateNow << " start" << endl;
			dateStart = dateNow;

		}

		timeNow = row[2];
		type = row[4];

		if (dateNow != "21-MAY-2012")
		{ 
			if (!t.vaildTVolumeTime(timeNow))
			{
				continue;
			}
			
			strVolumne = row[6];
			
			volumeTimeIndex = t.volumeTimeIntervalIndex(timeNow);
			//	priceTimeIndex = t.priceTimeIntervalIndex(timeNow);
		

			if (type == "Trade")
			{
				
				vloumne = strtof(strVolumne.c_str(), NULL);
				sumVolume[volumeTimeIndex] += vloumne;

				//cout << dateNow << "  " << timeNow << "  " << type  << "     "  << strVolumne << "      " << volumeTimeIndex << "       " << sumVolume[volumeTimeIndex] << endl;
			}		

		}
		else
		{ 
			if (!t.vaildPriceTime(timeNow))
			{
				continue;
			}

			if (type == "Trade")
			{
				if (t.priceTimeIntervalIndex(timeNow) != index)
				{
					strPrice = row[5];
					t.priceTimeIntervalIndex(timeNow);
					Price[t.priceTimeIntervalIndex(timeNow)] = strtof(strPrice.c_str(), NULL);
					Time[t.priceTimeIntervalIndex(timeNow)] = timeNow;
					index = t.priceTimeIntervalIndex(timeNow);
				}

			}
					 		 	
		}
						
	}

	cout << "Done." << endl;

	cout << "===================================" << endl;
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		cout << Time[i] << endl;
	}

	cout << "===================================" << endl;
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		avgVolumne[i] = int(sumVolume[i] / 14.0);
		cout << avgVolumne[i] <<endl;
	}


	cout << "===================================" << endl;
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		cout << Price[i] << endl;
	}

	fstream myfile;
	//writefile
	myfile.open("outputs.csv", 'w');
	myfile << "Time" << "," << "Quantity" << "," << "Price" << endl;
	for (int i = 0; i < MAX_INDEX; i++)
	{
		myfile << Time[i] << "," << avgVolumne[i] << "," << Price[i] << endl;
	}
	myfile.close();

	system("Pause");
	return 0;
}