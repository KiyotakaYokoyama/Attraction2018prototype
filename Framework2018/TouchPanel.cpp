#include "TouchPanel.h"
#include "Application.h"
#include "DxLib.h"

TouchPanelPtr TouchPanel::getTask( ) {
	return std::dynamic_pointer_cast<TouchPanel>( Application::getInstance( )->getTask( getTag( ) ) );
}

TouchPanel::Touch::Touch( ) :
pos( Vector( ) ),
id( 0 ) {
}

TouchPanel::Touch::~Touch( ) {
}

TouchPanel::Touch::Touch( Vector pos_, int id_ ) :
pos( pos_ ),
id( id_ ) {
}

TouchPanel::TouchPanel( ) {
}


TouchPanel::~TouchPanel( ) {
	_touchs.clear( );
}

void TouchPanel::update( ) {
	_touchs.clear( );
	int count = 0;
	while ( true ) {
		int x = 0;
		int y = 0;
		int id = 0;
		if ( GetTouchInput( count, &x, &y, &id, NULL ) == -1 ) {
			break;
		}
		_touchs.push_back( Touch( Vector( x, y ), id ) );
		count++;
	}
}

std::vector< TouchPanel::Touch > TouchPanel::getTouchs( ) const {
	return _touchs;
}

