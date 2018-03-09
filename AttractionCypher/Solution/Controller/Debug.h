#pragma once
#include "Task.h"
#include <string>

PTR( Debug );

class Debug : public Task {
public:
	static std::string getTag( ) { return "DEBUG"; };
	static DebugPtr getTask( );
public:
	Debug( );
	virtual ~Debug( );
public:
	void draw( ) const;
private:
	void update( );
};

