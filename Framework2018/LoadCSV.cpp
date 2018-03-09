#include "LoadCSV.h"
#include <cstdio>
#include <assert.h>

const int MAX_BUFFER = 100;

LoadCSV::LoadCSV( const char* filename, int max_num ) {
	std::string path = filename;
	path += ".csv";

	FILE* fp;
	errno_t file_load;
	file_load = fopen_s( &fp, path.c_str(), "r" );

	assert( !file_load );

	char buffer[ MAX_BUFFER ];
	while ( fgets( buffer, MAX_BUFFER, fp ) != NULL ) {
		std::string filename = buffer;
		int index = (int)filename.find(",");
		filename = filename.substr( 0, index );
		_csv_data.push_back( filename );
	}
	assert( ( int )_csv_data.size( ) <= max_num );
}

std::string LoadCSV::getData( int index ) {
	return _csv_data[index];
}

int LoadCSV::getSize( ) {
	return (int)_csv_data.size( );
}

LoadCSV::~LoadCSV( ) {
}
