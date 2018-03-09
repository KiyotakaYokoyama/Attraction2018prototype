#include "Mouse.h"
#include "Application.h"
#include "DxLib.h"

MousePtr Mouse::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Mouse >( fw->getTask( getTag( ) ) );
}

Mouse::Mouse( ) {
	_pos = Vector( );
	_image = 0;
	_key[ MOUSE_LEFT_BUTTON ] = 0;
	_key[ MOUSE_RIGHT_BUTTON ] = 0;
	_icon_disp_flag = true;
	_wheel_value = 0;
}

Mouse::~Mouse( ) {
}

void Mouse::update( ) {
	int x = 0;
	int y = 0;
	GetMousePoint( &x, &y );
	_pos = Vector( x, y );
	
	int mouse = GetMouseInput( );
	if ( mouse & MOUSE_INPUT_LEFT ) {
		_key[ MOUSE_LEFT_BUTTON ]++;
		if ( _key[ MOUSE_LEFT_BUTTON ] > 2 ) {
			_key[ MOUSE_LEFT_BUTTON ] = 2;
		}
	} else {
		_key[ MOUSE_LEFT_BUTTON ] = 0;
	}

	if ( mouse & MOUSE_INPUT_RIGHT ) {
		_key[ MOUSE_RIGHT_BUTTON ]++;
		if ( _key[ MOUSE_RIGHT_BUTTON ] > 2 ) {
			_key[ MOUSE_RIGHT_BUTTON ] = 2;
		}
	} else {
		_key[ MOUSE_RIGHT_BUTTON ] = 0;
	}

	_wheel_value = GetMouseWheelRotVol( );
	SetMouseDispFlag( _icon_disp_flag );
	if ( !_icon_disp_flag ) {
		DrawRotaGraph( ( int )_pos.x, ( int )_pos.y, 1.0, 0.0, _image, TRUE );
	}
}

Vector Mouse::getPos( ) const {

	return _pos;
}

int Mouse::getWheelRotValue( ) const {
	return _wheel_value;
}

bool Mouse::isPushRightButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_RIGHT_BUTTON ] == 1 ) {
		result = true;
	}
	return result;
}

bool Mouse::isHoldRightButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_RIGHT_BUTTON ] >= 1 ) {
		result = true;
	}
	return result;
}

bool Mouse::isPushLeftButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_LEFT_BUTTON ] == 1 ) {
		result = true;
	}
	return result;
}

bool Mouse::isHoldLeftButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_LEFT_BUTTON ] >= 1 ) {
		result = true;
	}
	return result;
}

void Mouse::changeMouseImage( int image_handle ) {
	_image = image_handle;
	_icon_disp_flag = false;
}

void Mouse::resetMouseImage( ) {
	_icon_disp_flag = true;
}