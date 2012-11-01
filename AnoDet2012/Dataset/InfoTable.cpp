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

int InfoTable::add(int id, std::vector<std::string> &infoRecord){
	ids.push_back(id);
	values.push_back(infoRecord);
	return values.size()-1;
}

vector<string> InfoTable::get(int id){
	for(int i=0;i<ids.size();++i){
		if(ids.at(i) == id)
			return values.at(id);
	}
    vector<string> empty;
    return empty;
}
