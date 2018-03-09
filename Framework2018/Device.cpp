#include "Device.h"
#include "mathmatics.h"
#include "Application.h"
#include "DxLib.h"

const int JOYPADKEY[ MAX_JOYPAD_USE_NUM ] = {
	DX_INPUT_PAD1,
	DX_INPUT_PAD2,
	DX_INPUT_PAD3,
	DX_INPUT_PAD4,
	DX_INPUT_PAD5,
	DX_INPUT_PAD6,
	DX_INPUT_PAD7,
	DX_INPUT_PAD8,
	DX_INPUT_PAD9,
	DX_INPUT_PAD10,
	DX_INPUT_PAD11,
	DX_INPUT_PAD12,
	DX_INPUT_PAD13,
	DX_INPUT_PAD14,
	DX_INPUT_PAD15,
	DX_INPUT_PAD16,
};

DevicePtr Device::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Device >( fw->getTask( Device::getTag( ) ) );
}


Device::Device( ) :
_num( 0 ) {
}

void Device::initialize( ) {
	for ( int i = 0; i < MAX_JOYPAD_USE_NUM; i++ ) {
		_data[ i ].x = 0;
		_data[ i ].y = 0;
		_data[ i ].rx = 0;
		_data[ i ].ry = 0;
		_data[ i ].button = 0;
		_data[ i ].push = 0;
	}
	resetup( );
}

Device::~Device( ) {
}

void Device::resetup( ) {
	ReSetupJoypad( );
	_num = GetJoypadNum( );
}

char Device::getDirX( int idx ) const {
	if ( idx < 0 ) return 0;
	return _data[ idx ].x;
}

char Device::getDirY( int idx ) const {
	if ( idx < 0 ) return 0;
	return _data[ idx ].y;
}

char Device::getRightDirX( int idx ) const {
	if ( idx < 0 ) return 0;
	return _data[ idx ].rx;
}

char Device::getRightDirY( int idx ) const {
	if ( idx < 0 ) return 0;
	return _data[ idx ].ry;
}


unsigned char Device::getButton( int idx ) const {
	if ( idx < 0 ) return 0;
	return _data[ idx ].button;
}

unsigned char Device::getPush( int idx ) const {
	if ( idx < 0 ) return 0;
	return _data[ idx ].push;
}

int Device::getDeviceNum( ) const {
	return _num;
}

void Device::update() {
	if (_num < 1) {
		updateNoJoypad();
	} else {
		updateJoypad();
	}
}

void Device::updateNoJoypad() {
	{
		Vector vec;
		vec.x += +(CheckHitKey(KEY_INPUT_RIGHT) != 0);
		vec.x += -(CheckHitKey(KEY_INPUT_LEFT) != 0);
		vec.y += +(CheckHitKey(KEY_INPUT_DOWN) != 0);
		vec.y += -(CheckHitKey(KEY_INPUT_UP) != 0);
		vec = vec.normalize() * 100;

		_data[0].x = (char)vec.x;
		_data[0].y = (char)vec.y;
	}

	{
		Vector vec;
		vec.x += +(CheckHitKey(KEY_INPUT_NUMPAD6) != 0);
		vec.x += -(CheckHitKey(KEY_INPUT_NUMPAD4) != 0);
		vec.y += +(CheckHitKey(KEY_INPUT_NUMPAD2) != 0);
		vec.y += -(CheckHitKey(KEY_INPUT_NUMPAD8) != 0);
		vec = vec.normalize() * 100;

		_data[0].rx = (char)vec.x;
		_data[0].ry = (char)vec.y;
	}

	unsigned char button = _data[0].button;
	_data[0].button = 0;
	_data[0].push = 0;

	int input[6] = {
		KEY_INPUT_Z,
		KEY_INPUT_X,
		KEY_INPUT_C,
		KEY_INPUT_V,
		KEY_INPUT_A,
		KEY_INPUT_S,
	};
	unsigned char code[6] = {
		BUTTON_A,
		BUTTON_B,
		BUTTON_C,
		BUTTON_D,
		BUTTON_E,
		BUTTON_F,
	};
	for ( int i = 0; i < 6; i++ ) {
		if ( CheckHitKey( input[ i ] ) ) {
			_data[0].button |= code[ i ] ;
			if ( ( button & code[ i ] ) == 0 ) {
				_data[0].push |= code[ i ];
			}
		}
	}
}

void Device::updateJoypad( ) {
	for ( int i = 0; i < _num; i++ ) {
		int joypad_key = JOYPADKEY[ i ];
		int key = GetJoypadInputState( joypad_key );
		Vector vec;
		int x = 0, y = 0;
		GetJoypadAnalogInput( &x, &y, joypad_key );
		vec.x = x / 1000.0;
		vec.y = y / 1000.0;
		vec.x += +( ( key & PAD_INPUT_RIGHT ) != 0 );
		vec.x += -( ( key & PAD_INPUT_LEFT  ) != 0 );
		vec.y += +( ( key & PAD_INPUT_DOWN  ) != 0 );
		vec.y += -( ( key & PAD_INPUT_UP    ) != 0 );
		vec = vec.normalize( ) * 100;

		_data[ i ].x = ( char )vec.x;
		_data[ i ].y = ( char )vec.y;

		GetJoypadAnalogInputRight( &x, &y, joypad_key );
		vec.x = x / 1000.0;
		vec.y = y / 1000.0;
		vec.x += +( CheckHitKey( KEY_INPUT_NUMPAD6 ) != 0 );
		vec.x += -( CheckHitKey( KEY_INPUT_NUMPAD4 ) != 0 );
		vec.y += +( CheckHitKey( KEY_INPUT_NUMPAD2 ) != 0 );
		vec.y += -( CheckHitKey( KEY_INPUT_NUMPAD8 ) != 0 );
		vec = vec.normalize( ) * 100;

		_data[ i ].rx = ( char )vec.x;
		_data[ i ].ry = ( char )vec.y;

		unsigned char button = _data[ i ].button;
		_data[ i ].button = 0;
		_data[ i ].push = 0;
		if ( ( key & PAD_INPUT_1 ) != 0 ) { 
			_data[ i ].button |= BUTTON_A;
			if ( ( button & BUTTON_A ) == 0 ) {
				_data[ i ].push |= BUTTON_A;
			}
		}
		if ( ( key & PAD_INPUT_2 ) != 0 ) { 
			_data[ i ].button |= BUTTON_B;
			if ( ( button & BUTTON_B ) == 0 ) {
				_data[ i ].push |= BUTTON_B;
			}
		}
		if ( ( key & PAD_INPUT_3 ) != 0 ) { 
			_data[ i ].button |= BUTTON_C;
			if ( ( button & BUTTON_C ) == 0 ) {
				_data[ i ].push |= BUTTON_C;
			}
		}
		if ( ( key & PAD_INPUT_4 ) != 0 ) { 
			_data[ i ].button |= BUTTON_D;
			if ( ( button & BUTTON_D ) == 0 ) {
				_data[ i ].push |= BUTTON_D;
			}
		}
		if ( ( key & PAD_INPUT_5 ) != 0 ) { 
			_data[ i ].button |= BUTTON_E;
			if ( ( button & BUTTON_E ) == 0 ) {
				_data[ i ].push |= BUTTON_E;
			}
		}
		if ( ( key & PAD_INPUT_6 ) != 0 ) { 
			_data[ i ].button |= BUTTON_F;
			if ( ( button & BUTTON_F ) == 0 ) {
				_data[ i ].push |= BUTTON_F;
			}
		}
	}
}
