#include "InfoTable.h"

using namespace std;

InfoTable::InfoTable(string tableName, vector<string> &valueNames)
{
	this->tableName = tableName;
	this->valueNames = valueNames;
}


InfoTable::~InfoTable(void)
{
}

int InfoTable::add(std::vector<std::string> &infoRecord){
	values.push_back(infoRecord);
	return values.size()-1;
}

vector<string> InfoTable::get(int id){
	return values.at(id);
}