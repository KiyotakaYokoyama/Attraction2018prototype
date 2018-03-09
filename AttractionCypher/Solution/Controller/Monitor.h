#pragma once
#include "Task.h"
#include <string>

PTR( Monitor );
PTR( Speaker );
PTR( Matching );

class Monitor : public Task {
public:
	static std::string getTag( ) { return "MONITOR"; };
	static MonitorPtr getTask( );
public:
	Monitor( );
	virtual ~Monitor( );
private:
	void initialize( );
	void update( );
	void draw( ) const;
private:
	bool _setuped;
	SpeakerPtr _speaker;
	MatchingPtr _matching;
};

