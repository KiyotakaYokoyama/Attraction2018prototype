#include "Application.h"
#include "Drawer.h"
#include "ModelMDL.h"
#include "ModelMV1.h"
#include "Device.h"
#include "Image.h"
#include "Gesture.h"
#include "FileSearchState.h"
#include <vector>
#include "Keyboard.h"
#include "Movie.h"
/*
class Test : public Task {
public:
	static std::string getTag( ) { return "TEST"; }
	Test( ) {
	}

	virtual ~Test( ) {
	}
	
	void initialize( ) {
		DrawerPtr drawer = Drawer::getTask( );

		_select = 0;

		// bg
		for ( auto& file : findFile( "Resource/bg/*.*" ) ) {
			Data data;
			data.name = file.Name;
			data.enabled = true;
			data.layer = false;
			if ( data.name == "." ) continue;
			if ( data.name == ".." ) continue;
			_file.push_back( data );
		}

		// layer
		for ( auto& file : findFile( "Resource/layer/*.*" ) ) {
			Data data;
			data.name = file.Name;
			data.enabled = true;
			data.layer = true;
			if ( data.name == "." ) continue;
			if ( data.name == ".." ) continue;
			_file.push_back( data );
		}

		// ‰æ–ÊƒTƒCƒY
		ApplicationPtr app = Application::getInstance( );
		_width = app->getWindowWidth( );
		_height = app->getWindowHeight( );
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->waitForSync( );
		drawer->flip( );

		if ( _movie_bg ) _movie_bg->draw( );
		if ( _movie_layer ) _movie_layer->draw( ( _width - 500 ) / 2, ( _height - 500 ) / 2 );

		int line = 0;
		for ( Data data : _file ) {
			drawer->drawString( 20, line, "%s: %s %s", data.layer ? "Layer" : "   BG", data.name.c_str( ), data.enabled ? "" : "disabled" );
			line += 20;
		}
		drawer->drawString( 0, _select * 20, ">" );

		KeyboardPtr keyboard = Keyboard::getTask( );
		int count = ( int )_file.size( );
		if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
			_select = ( _select + 1 ) % count;
		}
		if ( keyboard->isPushKey( "ARROW_UP" ) ) {
			_select = ( _select + count - 1 ) % count;
		}
		if ( keyboard->isPushKey( "ENTER" ) ) {
			if ( _file[ _select ].layer ) {
				if ( _movie_layer ) {
					_movie_layer->stop( );
				}
				std::string str = "Layer/";
				str += _file[ _select ].name;
				_movie_layer = drawer->createMovie( str.c_str( ), true );
				if ( _movie_layer ) {
					_movie_layer->play( false );
				} else {
					_file[ _select ].enabled = false;
				}
			} else {
				if ( _movie_bg ) {
					_movie_bg->stop( );
				}
				std::string str = "bg/";
				str += _file[ _select ].name;
				_movie_bg = drawer->createMovie( str.c_str( ) );
				if ( _movie_bg ) {
					_movie_bg->play( );
				} else {
					_file[ _select ].enabled = false;
				}
			}
		}

	}
private:
	struct Data {
		std::string name;
		bool enabled;
		bool layer;
	};
	std::vector< Data > _file;
	int _select;
	MoviePtr _movie_layer;
	MoviePtr _movie_bg;
	int _width;
	int _height;
};

void main( ) {
	TaskPtr drawer = TaskPtr( new Drawer( "Resource" ) );
	TaskPtr test = TaskPtr( new Test( ) );
	TaskPtr keyboard = TaskPtr( new Keyboard );

	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Test::getTag( ), test );
	app->addTask( Keyboard::getTag( ), keyboard );
}
*/