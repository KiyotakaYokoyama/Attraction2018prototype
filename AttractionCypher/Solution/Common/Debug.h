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
	void update( );
	void draw( ) const;
	bool isDebug( ) const;
private:
	void checkDoubleTop( );
private:
	bool _debug;
	bool _change;
	int _change_count;
};

