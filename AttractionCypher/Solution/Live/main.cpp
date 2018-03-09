#include "Application.h"
#include "define.h"
#include "Drawer.h"
#include "Mouse.h"
#include "TouchPanel.h"
#include "Live.h"
#include "Debug.h"
#include "Keyboard.h"

void main( ) {
	ApplicationPtr app = Application::getInstance( );
	app->setGraphSize( SCREEN_WIDTH, SCREEN_HIGHT );
	app->addTask( Drawer::getTag( ), TaskPtr( new Drawer( "Resources" ) ) );
	app->addTask( TouchPanel::getTag( ), TaskPtr( new TouchPanel ) );
	app->addTask( Mouse::getTag( ), TaskPtr( new Mouse ) );
	app->addTask( Live::getTag( ), TaskPtr( new Live ) );
	app->addTask( Debug::getTag( ), TaskPtr( new Debug ) );
	app->addTask( Keyboard::getTag( ), TaskPtr( new Keyboard ) );
}
