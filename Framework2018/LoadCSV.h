#pragma once
#include <vector>
#include <string>

class LoadCSV {
public:
	LoadCSV( const char* filename, int max_num );
	virtual ~LoadCSV( );
public:
	std::string getData( int index );
	int getSize( );
private:
	std::vector<std::string> _csv_data;
};

