#include "DxLib.h"
#include "Application.h"
#include "Mouse.h"

void main( );

int WINAPI WinMain( HINSTANCE hInstaece, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	Application::initialize( );
	main( );
	Application::getInstance( )->run( );
	Application::finalize( );
	return 0;
}