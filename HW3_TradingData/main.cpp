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
	int count = 0; // count index in Price array


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


		if (dateNow != "21-MAY-2012")
		{ 
			if (!t.vaildTVolumeTime(timeNow))
			{
				continue;
			}
			
			strVolumne = row[6];
			type = row[4];
			volumeTimeIndex = t.volumeTimeIntervalIndex(timeNow);
			//	priceTimeIndex = t.priceTimeIntervalIndex(timeNow);

			

			if (type == "Trade")
			{
				
				vloumne = strtof(strVolumne.c_str(), NULL);
				sumVolume[volumeTimeIndex] += vloumne;

				// cout << dateNow << "  " << timeNow << "  " << type  << "     "  << strVolumne << "      " << volumeTimeIndex << "       " << sumVolume[volumeTimeIndex] << endl;

			}
			
		}
		else
		{ 
			 if (!t.vaildPriceTime(timeNow))
			 {
				 continue;
			 }
			 else
			 {
				 
				 strPrice = row[5];
				 Price[count] = strtof(strPrice.c_str(), NULL);
				 Time[count] = timeNow;
				 count++; 
			 }	

		}
						
	}

	ofstream myfile("output.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i <= MAX_INDEX; i++) {
			myfile << Time[i] << "      " << avgVolumne[i] << "      " << Price[i] << endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";


	cout << "Done." << endl;

	//cout << "===================================" << endl;
	//for (int i = 0; i <= MAX_INDEX; i++)
	//{
	//	avgVolumne[i] = int(sumVolume[i] / 14.0);
	//	cout << avgVolumne[i] <<endl;
	//}

	//cout << "===================================" << endl;
	//for (int i = 0; i <= MAX_INDEX; i++)
	//{
	//	avgPrice[i] = int(sumPrice[i] / 14.0);
	//	cout << avgPrice[i] << endl;
	//}
	


	//Timer t;
	//std::cout << t.timeIntervalIndex("8:01:01.1") << std::endl;


	system("Pause");
	return 0;
}