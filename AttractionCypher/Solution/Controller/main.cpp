#include "Application.h"
#include "Sound.h"
#include "Drawer.h"
#include "Monitor.h"
#include "Debug.h"
#include "Keyboard.h"
#include "define.h"
#include "TouchPanel.h"

void main( ) {
	ApplicationPtr app = Application::getInstance( );

	app->addTask( Drawer::getTag( ), TaskPtr( new Drawer( "Resources" ) ) );
	app->addTask( Sound::getTag( ), TaskPtr( new Sound( "Resources/Sound" ) ) );
	app->addTask( Monitor::getTag( ), TaskPtr( new Monitor ) );
	app->addTask( Keyboard::getTag( ), TaskPtr( new Keyboard ) );
	app->addTask( TouchPanel::getTag( ), TaskPtr( new TouchPanel ) );
	app->addTask( Debug::getTag( ), TaskPtr( new Debug ) );
}
