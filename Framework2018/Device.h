#pragma once

#include "Task.h"
#include "Network.h"
#include <array>

const int MAX_JOYPAD_USE_NUM = 16;

const unsigned char BUTTON_NONE = 0x00; // INPUT_NONE ( 0b00000000 )
const unsigned char BUTTON_A    = 0x01; // INPUT_A ( 0b00000001 )
const unsigned char BUTTON_B    = 0x02; // INPUT_B ( 0b00000010 )
const unsigned char BUTTON_C    = 0x04; // INPUT_C ( 0b00000100 )
const unsigned char BUTTON_D    = 0x08; // INPUT_D ( 0b00001000 )
const unsigned char BUTTON_E    = 0x10; // INPUT_E ( 0b00010000 )
const unsigned char BUTTON_F    = 0x20; // INPUT_F ( 0b00100000 )

PTR( Device );

class Device : public Task {
public:
	static std::string getTag( ) { return "DEVICE"; }
	static DevicePtr getTask( );
public:
	Device( );
	virtual ~Device( );
public:
	void initialize( );
	virtual void update( );
public:
	int getDeviceNum( ) const;
	char getDirX( int idx = 0 ) const;
	char getDirY( int idx = 0 ) const;
	char getRightDirX( int idx = 0 ) const;
	char getRightDirY( int idx = 0 ) const;
	
	unsigned char getButton( int idx = 0 ) const;
	unsigned char getPush( int idx = 0 ) const;
public:
	void resetup( );
private:
	struct DATA {
		char x;
		char y;
		char rx;
		char ry;
		unsigned char button;
		unsigned char push;
	};
private:
	void updateNoJoypad();
	void updateJoypad();
private:
	std::array< DATA, MAX_JOYPAD_USE_NUM > _data;
	int _num;
};