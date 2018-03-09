#include "Monitor.h"
#include "Application.h"
#include "Speaker.h"
#include "Debug.h"
#include "Drawer.h"
#include "Keyboard.h"

MonitorPtr Monitor::getTask( ) {
	return std::dynamic_pointer_cast< Monitor >( Application::getInstance( )->getTask( getTag( ) ) );
}

Monitor::Monitor( ) :
_setuped( false ) {
}

Monitor::~Monitor( ) {
}

void Monitor::initialize( ) {
	_speaker = SpeakerPtr( new Speaker );
}

void Monitor::update( ) {
	_speaker->update( );
	if ( _speaker->isBGM( ) ) {
		_setuped = false;
	}
	draw( );
}

void Monitor::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->flip( );
}
