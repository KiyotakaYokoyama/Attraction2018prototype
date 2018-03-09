#include "Binary.h"
#include <assert.h>

Binary::Binary( ) :
_data( NULL ),
_pos( 0 ),
_size( 0 ) {
}


Binary::~Binary( ) {
	ensure( 0 );
}

int Binary::getSize( ) const {
	return _size;
}

void * Binary::getPtr( ) {
	return _data;
}

void Binary::ensure( int size ) {
	_pos = 0;

	if ( _data ) {
		delete [ ] _data;
		_data = NULL;
	}

	_size = 0;
	if ( size > 0 ) {
		_size = size;
		_data = new unsigned char[ size ];
	}
}

void Binary::seek( ) {
	_pos = 0;
}

void Binary::read( void* data, int size ) {
	memcpy( data, &_data[ _pos ], size );
	_pos += size;
	assert( _pos <= _size );
}

void Binary::write( void* data, int size ) {
	memcpy( &_data[ _pos ], data, size );
	_pos += size;
	assert( _pos <= _size );
}

