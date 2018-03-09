#include "Status.h"



Status::Status( ) :
_level( 1 ) {
}


Status::~Status( ) {
}

int Status::getLevel( ) const {
	return _level;
}
