#include "Application.h"
#include "Binary.h"
#include "DxLib.h"

#pragma warning(disable:4099)

const int PARTICLE = 2000; //Effekseerの最大パーティクル数
const int COLOR_BIT = 32;
const int COLOR_BIT_DEPTH = 32;
const int FPS = 60;
const char* WINDOW_NAME = "Framework";

ApplicationPtr Application::_instance;

Application::Application( ) {
	int sx = GetSystemMetrics( SM_CXSCREEN );
	int sy = GetSystemMetrics( SM_CYSCREEN );
	SetGraphMode( sx, sy, COLOR_BIT_DEPTH, FPS );
	_screen_width = sx;
	_screen_height = sy;
	_window_width = sx;
	_window_height = sy;

	SetWindowText( WINDOW_NAME );

#	if FULLSCREEN
		ChangeWindowMode( FALSE );
#	else
		ChangeWindowMode( TRUE );
		sx = sx * 3 / 4;
		sy = sy * 3 / 4;
		_window_width = sx;
		_window_height = sy;
		SetWindowSize( sx, sy );
#	endif

#	if _DEBUG
		SetDoubleStartValidFlag( TRUE ); // 多重起動
#	endif

	if ( DxLib_Init( ) == -1 ) {
		return;
	}

	initSettings( );

	_terminating = false;
}

Application::~Application( ) {
	DxLib_End( );
}

void Application::initialize( ) {
	if ( !_instance ) {
		_instance = ApplicationPtr( new Application );
	}
}

void Application::finalize( ) {
	if ( _instance ) {
		_instance.reset( );
	}
}

ApplicationPtr Application::getInstance( ) {
	return _instance;
}

void Application::initSettings( ) {
	SetUseLighting( FALSE );
	SetLightEnable( FALSE );
	SetUseZBuffer3D( TRUE );
	SetWriteZBuffer3D( TRUE );
	SetDrawScreen( DX_SCREEN_BACK );
	SetupCamera_Perspective( 12.0f * DX_PI_F / 180.0f );
	SetCameraNearFar( 0.1f, 300.0f );
	SetAlwaysRunFlag( TRUE ) ;
	SetUseBackCulling( TRUE ) ;
    SetTextureAddressModeUV( DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP );
}

void Application::run( ) {
	// 初期化
	{
		std::map< std::string, TaskPtr >::iterator ite = _task_list.begin( );
		while ( ite != _task_list.end( ) ) {
			TaskPtr task = ite->second;
			task->initialize( );
			ite++;
		}
	}

	// メインループ
	while ( true ) {
		if ( _terminating ) {
			break;
		}
		if ( ProcessMessage( ) != 0 ) {
			break;
		}
		if ( CheckHitKey( KEY_INPUT_ESCAPE ) ) {
			break;
		}

		std::map< std::string, TaskPtr >::iterator ite = _task_list.begin( );
		while ( ite != _task_list.end( ) ) {
			TaskPtr task = ite->second;
			task->update( );
			ite++;
		}
	}

	// 終了処理
	{
		std::map< std::string, TaskPtr >::iterator ite = _task_list.begin( );
		while ( ite != _task_list.end( ) ) {
			TaskPtr task = ite->second;
			task->finalize( );
			ite++;
		}
	}
}

void Application::addTask( std::string tag, TaskPtr task ) {
	if ( task ) {
		_task_list[ tag ] = task;
	}
}

TaskPtr Application::getTask( std::string tag ) {
	if( _task_list.find( tag ) == _task_list.end( ) ) {
		return TaskPtr( );
	}
	return _task_list[ tag ];
}

int Application::getWindowWidth( ) const  {
	return _screen_width;
}

int Application::getWindowHeight( ) const  {
	return _screen_height;
}

bool Application::loadBinary( std::string  filename, BinaryPtr binary ) {
	int size = ( int )FileRead_size( filename.c_str( ) );
	if ( size == -1 ) {
		filename = "../" + filename;
		size = ( int )FileRead_size( filename.c_str( ) );
		if ( size == -1 ) {
			return false;
		}
	}

	int handle = FileRead_open( filename.c_str( ) );
	if ( handle == -1 ) {
		return false;
	}

	binary->ensure( size );

	FileRead_read( binary->getPtr( ), size, handle );

	FileRead_close( handle );

	return true;
}


void Application::saveBinary( std::string filename, BinaryPtr binary ) {

	int size = binary->getSize( );
	if ( size <= 0 ) {
		return;
	}
	
	FILE *fp;
	errno_t err = fopen_s( &fp, filename.c_str( ), "wb" );
	if ( err != 0 ) {
		return;
	}

	fwrite( binary->getPtr( ), size, 1, fp );
	fclose( fp );
}

std::string Application::inputString( int sx, int sy ) {
	char buf[ 2048 ];
	std::string str;
	if ( KeyInputString( sx, sy, 2048, buf, TRUE ) == TRUE ) {
		str = buf;
	}
	return str;
}

void Application::terminate( ) {
	_terminating = true;
}

void Application::changeWindow( bool windowed ) {
	ChangeWindowMode( windowed ? TRUE : FALSE );
	initSettings( );
}

void Application::setWindowSize( int width, int height ) {
	SetWindowSize( width, height );
}

void Application::setGraphSize( int width, int height ) {
	SetGraphMode( width, height, COLOR_BIT_DEPTH, FPS );
	_screen_width = width;
	_screen_height = height;
	initSettings( );
	setWindowSize( _window_width, _window_height );
}
