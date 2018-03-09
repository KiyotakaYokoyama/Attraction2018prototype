#include "Debug.h"
#include "Application.h"
#include "Drawer.h"

DebugPtr Debug::getTask( ) {
	return std::dynamic_pointer_cast< Debug >( Application::getInstance( )->getTask( getTag( ) ) );
}

Debug::Debug( ) {
}

Debug::~Debug( ) {
}

void Debug::update( ) {
}

void Debug::draw( ) const {
}

