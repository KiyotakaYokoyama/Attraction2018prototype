#include "Debug.h"
#include "Application.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "define.h"
#include "TouchPanel.h"

const int WAIT_DOUBLE_TOP_TIME = 15;

DebugPtr Debug::getTask( ) {
	return std::dynamic_pointer_cast< Debug >( Application::getInstance( )->getTask( getTag( ) ) );
}

Debug::Debug( ) :
_debug( false ),
_change( false ),
_change_count( 0 ) {
}

Debug::~Debug( ) {
}

void Debug::update( ) {
	if ( Keyboard::getTask( )->isPushKey( "SPACE" ) ) {
		_debug = !_debug;
	}
	checkDoubleTop( );
}

void Debug::checkDoubleTop( ) {
	int size = ( int )TouchPanel::getTask( )->getTouchs( ).size( );
	if ( _change_count > WAIT_DOUBLE_TOP_TIME && _change ) {
		_change = false;
		_change_count = 0;
	}
	if ( _change_count > 0 && size > 0 ) {
		_debug = !_debug;
		_change = false;
		_change_count = 0;
	}
	if ( _change && size == 0 ) {
		_change_count++;
	}
	if ( size >= 10 ) { //10本の指でタッチされいなければ表示はしない
		_change = true;
	}
}

bool Debug::isDebug( ) const {
	return _debug;
}

void Debug::draw( ) const {
	if ( isDebug( ) ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( SCREEN_WIDTH - 60, 0, "Debug" );
		drawer->drawString( SCREEN_WIDTH - 60, 20, "FPS:%.2lf", drawer->getFps( ) );
	}
}