#include "Camera.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ModelMaker.h"
#include "Drawer.h"

const Vector START_TO_POS = Vector( 0, 50, 50 );
const double ROT_SPEED = 0.01;

CameraPtr Camera::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Camera >( app->getTask( getTag( ) ) );
}

Camera::Camera( ) {
	_to_pos = START_TO_POS;
	_before_mouse_pos = _to_pos;

}

Camera::~Camera( ) {
}

void Camera::initialize( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCameraUp( Vector( 0.0, 0.0, 1.0 ) );
	drawer->setCamera( _target + _to_pos, _target );
}

void Camera::update( ) {
	// マウスの左右でZ軸回転をするように
	DrawerPtr drawer = Drawer::getTask( );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );

	Vector mouse_pos = mouse->getPos( );
	if ( mouse->isPushLeftButton( ) ) {
		_before_mouse_pos = mouse_pos;
	}
	if ( mouse->isHoldLeftButton( ) ) {
		Vector diff = _before_mouse_pos - mouse_pos;
		if ( diff.x != 0 ) {
			Vector axis = Vector( 0, 0, 1 );
			Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), diff.x * ROT_SPEED );
			_to_pos = rot.multiply( _to_pos );
		}
		if ( diff.y != 0 ) {
			Vector axis = Vector( _to_pos.y, -_to_pos.x ).normalize( );
			Matrix rot = Matrix::makeTransformRotation( axis, diff.y * ROT_SPEED );
			_to_pos = rot.multiply( _to_pos );
		}
		_before_mouse_pos = mouse_pos;
	}

	int wheel = mouse->getWheelRotValue( );
	if ( wheel != 0 ) {
		double length = _to_pos.getLength( );
		length += wheel;
		if ( length < 10 ) {
			length = 10;
		}
		_to_pos = _to_pos.normalize( ) * length;
	}

	if ( keyboard->isPushKey( "ENTER" ) ) {
  		_to_pos = START_TO_POS;
	}

	Vector vec;
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		vec = _to_pos;
		vec.z = 0;
		vec *= -1;
	}

	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		vec = _to_pos;
		vec.z = 0;
	}

	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		vec = _to_pos;
		vec.z = 0;
		vec = _to_pos.cross( Vector( 0, 0, -1 ) );
	}
	
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		vec = _to_pos;
		vec.z = 0;
		vec = _to_pos.cross( Vector( 0, 0, 1 ) );
	}
	

	_target += vec.normalize( ) * 0.1;

	drawer->setCamera( _target + _to_pos, _target );
}