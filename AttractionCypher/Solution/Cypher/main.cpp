#include "Application.h"
#include "Game.h"
#include "define.h"
#include "Drawer.h"
#include "Mouse.h"
#include "TouchPanel.h"
#include "Debug.h"
#include "Keyboard.h"
#include "Sound.h"

void main( ) {
	ApplicationPtr app = Application::getInstance( );
	app->setGraphSize( SCREEN_WIDTH, SCREEN_HIGHT );
	app->addTask( Debug::getTag( ), TaskPtr( new Debug( ) ) );
	app->addTask( Keyboard::getTag( ), TaskPtr( new Keyboard( ) ) );
	app->addTask( Drawer::getTag( ), TaskPtr( new Drawer( "Resources" ) ) );
	app->addTask( TouchPanel::getTag( ), TaskPtr( new TouchPanel ) );
	app->addTask( Mouse::getTag( ), TaskPtr( new Mouse ) );
	app->addTask( Sound::getTag( ), TaskPtr( new Sound( "Resources/Sound" ) ) );
	app->addTask( Game::getTag( ), TaskPtr( new Game ) );
}
