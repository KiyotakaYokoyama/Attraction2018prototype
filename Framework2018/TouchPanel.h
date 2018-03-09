#pragma once
#include <string>
#include <vector>
#include "Task.h"
#include "mathmatics.h"

PTR( TouchPanel );

class TouchPanel : public Task {
public:
	static std::string getTag( ) { return "TOUCHPANEL"; };
	static TouchPanelPtr getTask( );
public:
	struct Touch {
		Vector pos;
		int id;
		Touch( );
		~Touch( );
		Touch( Vector pos_, int id_ );
	};
public:
	TouchPanel( );
	virtual ~TouchPanel( );
public:
	std::vector< Touch > getTouchs( ) const;
private:
	void update( );
private:
	std::vector< Touch > _touchs;
};

