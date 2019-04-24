#ifndef CSVROW
#define CSVROW


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <stdio.h>
#include <iomanip>


using namespace std;

class CsvRow
{
public:
	std::string const& operator[](std::size_t index) const
	{
		return m_data[index];
	}

	std::size_t size() const
	{
		return m_data.size();
	}

	void readNextRow(std::istream& str)
	{
		std::string line;
		std::getline(str, line);
		std::stringstream lineStream(line);
		std::string cell;

		m_data.clear();
		while (std::getline(lineStream, cell, ','))
		{
			m_data.push_back(cell);
		}
		
		if (!lineStream && cell.empty())
		{
			m_data.push_back("");
		}
	}
private:
	std::vector<std::string> m_data;
};

std::istream& operator>>(std::istream& str, CsvRow& data)
{
	data.readNextRow(str);
	return str;
}
#endif 
