#pragma once

#include "smart_ptr.h"

PTR( Binary );

class Binary {
public:
	Binary();
	virtual ~Binary();
public:
	void read( void* data, int size );
	void write( void* data, int size );
	void seek( );
	void ensure( int size );
	int getSize( ) const;
	void * getPtr( );
private:
	unsigned char * _data;
	unsigned int _size;
	unsigned int _pos;
};

