#pragma once
#include <string>
#include "smart_ptr.h"
#include "Task.h"
#include "mathmatics.h"

PTR( Mouse );

enum MOUSE {
	MOUSE_LEFT_BUTTON,
	MOUSE_RIGHT_BUTTON,
	MAX_MOUSE,
};

class Mouse : public Task {
public:
	static MousePtr getTask( );
	static std::string getTag( ) { return "MOUSE"; }
public:
	Mouse( );
	virtual ~Mouse( );
public:
	void update( );
	Vector getPos( ) const;
	int getWheelRotValue( ) const;
	bool isPushRightButton( ) const;
	bool isHoldRightButton( ) const;
	bool isPushLeftButton( ) const;
	bool isHoldLeftButton( ) const;
	void changeMouseImage( int image_handle );
	void resetMouseImage( );
private:
	int _wheel_value;
	Vector _pos;
	int _image;
	int _key[ MAX_MOUSE ];
	bool _icon_disp_flag; 
};
