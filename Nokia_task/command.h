#include <string>
#include <map>
#include <vector>

class command
{
public:
	command(std::string fileName);
	void normalize();
private:
	std::string fileName;
	std::vector<std::string> columns;
	std::vector<int> rows;
	std::map < std::pair<int, std::string>, std::string> CSVmap;
	int pars(const int &row, const std::string &column);
};