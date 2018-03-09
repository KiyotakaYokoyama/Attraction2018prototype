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
#include "ModelMV1.h"

class Test : public Task {
public:
	static std::string getTag( ) { return "TEST"; }
	Test( ) {
	}

	virtual ~Test( ) {
	}
	
	void initialize( ) {
		DrawerPtr drawer = Drawer::getTask( );

		double SCALE = 0.1;
		_model_background = ModelMV1Ptr( new ModelMV1 );
		_model_background->load( "BG/background.mv1" );
		_model_background->scaleTransform( Matrix::makeTransformScaling( Vector( SCALE, SCALE, SCALE  ) ) );

		_model_floor = ModelMV1Ptr( new ModelMV1 );
		_model_floor->load( "BG/floor.mv1" );
		_model_floor->scaleTransform( Matrix::makeTransformScaling( Vector( SCALE, SCALE, SCALE  ) ) );


		_pos = Vector( 0,0,0 );
		_angle = 0;
		drawer->setCameraUp( Vector( 0,1,0 ) );
	}

	void update( ) {

		double speed = 0.3;
		KeyboardPtr keyboard = Keyboard::getTask( );
		if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
			_pos.z -= speed;
		}
		if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
			_pos.z += speed;
		}
		if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
			_pos.x += speed;
		}
		if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
			_pos.x -= speed;
		}
		if ( keyboard->isHoldKey( "Z" ) ) {
			_pos.y -= speed;
		}
		if ( keyboard->isHoldKey( "A" ) ) {
			_pos.y += speed;
		}
		if ( keyboard->isHoldKey( "Q" ) ) {
			_angle += 0.01;
		}


		DrawerPtr drawer = Drawer::getTask( );
		drawer->waitForSync( );
		drawer->flip( );
		Vector vec = Vector( 0, 0, 1 );
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), _angle );
		Vector target = mat.multiply( vec ) + _pos;
		drawer->setCamera( _pos, target );

		_model_floor->draw();
		_model_background->draw();


		double  LINE_SIZE = 1000;
		drawer->drawLine( Vector( 0,-LINE_SIZE,0),Vector( 0,LINE_SIZE,0 ) );
		drawer->drawLine( Vector( -LINE_SIZE,0,0),Vector( LINE_SIZE,0,0 ) );
		drawer->drawLine( Vector( 0,0,-LINE_SIZE),Vector( 0,0,LINE_SIZE ) );

		double MEMORI = 0.1; // 0.1m = 100mm
		const int MAJOR_BIT_NUM = 100;

		for( int i = 0; i < MAJOR_BIT_NUM; i++ ) {
			drawer->drawLine( Vector( 0.01, MEMORI * MAJOR_BIT_NUM / 2 - ( i* MEMORI ) ,0 ),Vector( -0.01,MEMORI * MAJOR_BIT_NUM / 2 - ( i* MEMORI ), 0 ) );
		}

		drawer->drawString( 10,10,"pos( %.3f, %.3f, %.3f )",_pos.x, _pos.y, _pos.z );

	}



private:
	ModelMV1Ptr _model_background;
	ModelMV1Ptr _model_floor;

	Vector _pos;

	double _angle;
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