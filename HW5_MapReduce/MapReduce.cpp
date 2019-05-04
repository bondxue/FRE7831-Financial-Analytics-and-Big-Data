#include <iostream>
#include <thread>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>    // std::transform
#include <vector>       // std::vector
#include <functional>   // std::plus


using namespace std;

//string DataCollection = "web,wed,green,sun,moon,land,part,web,green";

string getData()
{
	string stringfile, tmp;

	ifstream input("4727.txt");

	while (!input.eof()) {
		getline(input, tmp);
		stringfile += tmp;
		//stringfile += "\n";
	}
	// To lower
	transform(stringfile.begin(), stringfile.end(), stringfile.begin(), ::tolower);
	cout << stringfile << endl;
	return stringfile;
}


//string DataCollection = "senior us economic disagree on the likelihood the government can";
string DataCollection = getData();
vector<string> WordList;
map<string, int> ResultTable;

// no lock needed 

const int input_size = 735;
const int no_of_threads = 3;

class Parser {
public:
	void operator()(char delimiter)
	{
		string word;
		istringstream wordStream(DataCollection);
		while (getline(wordStream, word, delimiter))
			WordList.push_back(word);
	}
};

class Counter
{
public:
	void operator()(map<string, int> *pResultTable, int start)
	{
		while (WordList.size() != input_size);
		for (int i = start; i < start + input_size/no_of_threads; i++)
		{
			int count = (*pResultTable)[string(WordList[i])];
			count = count + 1;
			(*pResultTable)[string(WordList[i])] = count;
			// cout << "Key = " << string(WordList[i]) << " count = " << (*pResultTable)[string(WordList[i])] << endl;
		}
	}
};

class WordCounter
{
private:
	map<string, int> ResultTables[no_of_threads];
public:
	void Map()
	{
		thread parser(Parser(), ' ');
		thread counter1(Counter(), &ResultTables[0], 0);
		thread counter2(Counter(), &ResultTables[1], 3);
		thread counter3(Counter(), &ResultTables[2], 6);

		parser.join();
		counter1.join();
		counter2.join();
		counter3.join();
	}
	void Reduce()
	{
		// combine 3 tables into the final 1 table 
		for (int i = 0; i < no_of_threads; i++)
		{
			for (map<string, int>::iterator itr = ResultTables[i].begin();
				itr != ResultTables[i].end(); itr++)
				ResultTable[itr->first] += itr->second;
		}
	}
};

int main()
{
	

	WordCounter wordcounter;
	ofstream myfile;
	

	wordcounter.Map();
	wordcounter.Reduce();

	myfile.open("ouput.txt");
	for (map<string, int>::iterator itr = ResultTable.begin(); itr != ResultTable.end(); itr++)
		myfile << itr->first << "	" << itr->second << endl;
	myfile.close();

	system("Pause");
	return 0;
}
/*
green   2
land    1
moon    1
part    1
sun     1
web     2
wed     1
Press any key to continue . . .
*/
