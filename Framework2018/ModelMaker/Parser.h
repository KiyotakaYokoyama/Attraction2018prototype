#pragma once

#include "smart_ptr.h"
#include <string>

PTR( Parser );
PTR( ModelMDL );

class Parser {
public:
	Parser();
	virtual ~Parser();
public:
	ModelMDLPtr makeModel( std::string filename );
};