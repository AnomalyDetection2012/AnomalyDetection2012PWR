#ifndef INFOTABLE_H
#define INFOTABLE_H

#include <vector>

class InfoTable
{
public:
	InfoTable(std::string tableName, std::vector<std::string> &valueNames);
	~InfoTable(void);
	std::string tableName;
	std::vector<std::string> valueNames;
	std::vector<std::vector<std::string>> values;
	int add(std::vector<std::string> &infoRecord); // zwraca id nowego rekordu
	std::vector<std::string> get(int id);
};

#endif // INFOTABLE_H