#include "command.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>

command::command(std::string fileName) : fileName(fileName)
{
	std::ifstream in(fileName);
	if (!in.is_open())
		throw "can't open file";
	std::string data;
	std::getline(in, data);
	std::stringstream sColumnsName(data);
	if (sColumnsName.get() != ',')
		throw "there is no comma at the beggining";
	while (sColumnsName.rdbuf()->in_avail()) //get column names from first line of CSV file
	{
		std::string buf;
		std::getline(sColumnsName, buf, ',');
		columns.push_back(buf);
	}
	while (std::getline(in, data)) // read line by line
	{
		std::stringstream line(data);
		std::string buf;
		std::getline(line, buf, ','); // get row name, which is lockated before the first comma
		int rowName = std::stoi(buf);
		rows.push_back(rowName);
		for (std::vector<std::string>::iterator it = columns.begin(); it < columns.end(); ++it)
		{
			std::getline(line, buf, ',');
			CSVmap.emplace(std::make_pair(rowName, *it), buf); // fill the map by current key
		}
	}
	in.close();
}

void command::normalize()
{
	for (auto &el : CSVmap)
		el.second = std::to_string(pars(el.first.first, el.first.second)); //normalize data
	for (const auto &el : columns)
		std::cout << ',' << el;
	std::cout << std::endl;
	for (const auto &r : rows)
	{
		std::cout << r;
		for (const auto &c : columns)
			std::cout << ',' << CSVmap[std::make_pair(r, c)];
		std::cout << std::endl;
	}
}

int command::pars(const int &row, const std::string &column) // parsing a formula
{
	std::string cell = CSVmap[std::make_pair(row, column)];
	if (std::all_of(cell.begin(), cell.end(), std::isdigit))
		return std::stoi(cell);
	auto firstRowIndex = cell.find_first_of("0123456789");
	std::string fColumn = cell.substr(1, firstRowIndex - 1);
	auto operationIndex = cell.find_first_of("+-*/");
	int fRow = std::stoi(cell.substr(firstRowIndex, operationIndex - firstRowIndex));
	auto secondRowIndex = cell.substr(operationIndex).find_first_of("0123456789");
	std::string sColumn = cell.substr(operationIndex + 1, secondRowIndex - 1);
	int sRow = std::stoi(cell.substr(operationIndex + secondRowIndex, cell.size() - (operationIndex + secondRowIndex)));
	switch (cell[operationIndex])
	{
	case '+':
		return pars(fRow, fColumn) + pars(sRow, sColumn);// get the values from other cells, whitch can be formulas too
		break;
	case '-':
		return pars(fRow, fColumn) - pars(sRow, sColumn);
		break;
	case '*':
		return pars(fRow, fColumn) * pars(sRow, sColumn);
		break;
	case '/':
		return pars(fRow, fColumn) / pars(sRow, sColumn);
		break;
	}
}