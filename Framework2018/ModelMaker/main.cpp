#include "Application.h"
#include "ModelMaker.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Drawer.h"
#include "Camera.h"

int main( ) {
	ApplicationPtr app = Application::getInstance( );
	KeyboardPtr keyboard = KeyboardPtr( new Keyboard );
	MousePtr mouse = MousePtr( new Mouse );
	CameraPtr camera = CameraPtr( new Camera );
	ModelMakerPtr model_maker = ModelMakerPtr( new ModelMaker( ) );
	DrawerPtr drawer = DrawerPtr( new Drawer( "ModelMaker" ) );

	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Keyboard::getTag( ), keyboard );
	app->addTask( Mouse::getTag( ), mouse );
	app->addTask( Camera::getTag( ), camera );
	app->addTask( ModelMaker::getTag( ), model_maker );
}