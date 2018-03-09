#include "ModelMaker.h"
#include "Application.h"
#include "Keyboard.h"
#include "Parser.h"
#include "ModelMDL.h"
#include <dxlib.h>

const int INPUT_X = 100;
const int INPUT_Y = 100;
const int COORDINATE_LINE_RANGE = 20;

ModelMakerPtr ModelMaker::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< ModelMaker >( app->getTask( getTag( ) ) );
}

ModelMaker::ModelMaker( ) :
_state( STATE_VIEWER )
, _texture( DX_NONE_GRAPH ) {
}

ModelMaker::~ModelMaker( ) {
}

void ModelMaker::update( ) {
	switch ( _state ) {
	case STATE_LOAD:
		load( );
		break;
	case STATE_SAVE:
		save( );
		break;
	case STATE_TEXTURE:
		loadTexture( );
		break;
	case STATE_VIEWER:
		view( );
		break;
	}
}

void ModelMaker::save( ) {
	_state = STATE_VIEWER;

	if ( !_model ) {
		return;
	}

	ApplicationPtr app = Application::getInstance( );
	std::string filename = app->inputString( INPUT_X, INPUT_Y );
	if ( filename.empty( ) ) {
		return;
	}
	
	if ( filename.find( ".mdl" ) == std::string::npos ) {
		filename += ".mdl";
	}

	_model->save( filename );
}

void ModelMaker::load( ) {
	_state = STATE_VIEWER;

	ApplicationPtr app = Application::getInstance( );
	std::string filename = app->inputString( INPUT_X, INPUT_Y );
	if ( filename.empty( ) ) {
		return;
	}

	if ( filename.find( ".x" ) != std::string::npos ) {
		ParserPtr parser( new Parser );
		_model = parser->makeModel( filename );
	}
	
	if ( filename.find( ".mdl" ) != std::string::npos ) {
		_model = ModelMDLPtr( new ModelMDL );
		if ( !_model->load( filename ) ) {
			_model.reset( );
		}
	}
}

void ModelMaker::view( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );

	if ( keyboard->isPushKey( "F1" ) ) {
		_state = STATE_LOAD;
	}
	if ( keyboard->isPushKey( "F2" ) ) {
		_state = STATE_TEXTURE;
	}
	if ( keyboard->isPushKey( "F5" ) ) {
		_state = STATE_SAVE;
	}
	
	if ( keyboard->isPushKey( "X" ) ) {
		Matrix matrix = Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI2 / 4 ); // XŽ²‚É90“x‰ñ“]
		_model->multiply( matrix );
	}

	if ( keyboard->isPushKey( "Y" ) ) {
		Matrix matrix = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI2 / 4 ); // YŽ²‚É90“x‰ñ“]
		_model->multiply( matrix );
	}

	if ( keyboard->isPushKey( "Z" ) ) {
		Matrix matrix = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI2 / 4 ); // ZŽ²‚É90“x‰ñ“]
		_model->multiply( matrix );
	}

	if ( keyboard->isPushKey( "NUM8" ) ) {
		Matrix matrix = Matrix::makeTransformTranslation( Vector( 0, 0.5, 0 ) );
		_model->multiply( matrix );
	}
	if ( keyboard->isPushKey( "NUM2" ) ) {
		Matrix matrix = Matrix::makeTransformTranslation( Vector( 0, -0.5, 0 ) );
		_model->multiply( matrix );
	}
	if ( keyboard->isPushKey( "NUM4" ) ) {
		Matrix matrix = Matrix::makeTransformTranslation( Vector( 0.5, 0, 0 ) );
		_model->multiply( matrix );
	}
	if ( keyboard->isPushKey( "NUM6" ) ) {
		Matrix matrix = Matrix::makeTransformTranslation( Vector( -0.5, 0, 0 ) );
		_model->multiply( matrix );
	}
	if ( keyboard->isPushKey( "+" ) ) {
		Matrix matrix = Matrix::makeTransformTranslation( Vector( 0, 0, 0.5 ) );
		_model->multiply( matrix );
	}
	if ( keyboard->isPushKey( "-" ) ) {
		Matrix matrix = Matrix::makeTransformTranslation( Vector( 0, 0, -0.5 ) );
		_model->multiply( matrix );
	}
	
	if ( keyboard->isPushKey( "A" ) ) {
		int count = _model->getPolygonNum( ) * 3;
		Vector max;
		for ( int i = 0; i < count; i++ ) {
			Vector pos = _model->getPoint( i );
			if ( ( int )( pos.z * 1000 ) != 0 ) {
				continue;
			}

			if ( max.x < abs( pos.x ) ) {
				max.x = abs( pos.x );
			}
			if ( max.y < abs( pos.y ) ) {
				max.y = abs( pos.y );
			}
			if ( max.z < abs( pos.z ) ) {
				max.z = abs( pos.z );
			}
		}
		double length = max.x;
		if ( length < max.y ) {
			length = max.y;
		}
		double scale = 0.5 / length;
		Matrix matrix = Matrix::makeTransformScaling( Vector( scale * 7, scale * 7, scale * 7 ) );
		_model->multiply( matrix );
	}


	ClearDrawScreen( );
	if ( _model ) {
		_model->draw( _texture );
	}
	DrawString( 0, 0, "F1:Load   F2:Texture   F5:save   XYZ:rot", GetColor( 255, 255, 255 ) );
	drawCoordinateLine( );

	ScreenFlip( );
}

void ModelMaker::loadTexture( ) {
	_state = STATE_VIEWER;
	if ( _texture != DX_NONE_GRAPH ) {
		DeleteGraph( _texture );
		_texture = DX_NONE_GRAPH;
	}

	ApplicationPtr app = Application::getInstance( );
	std::string filename = app->inputString( INPUT_X, INPUT_Y );
	if ( filename.empty( ) ) {
		return;
	}
	_texture = LoadGraph( filename.c_str( ) );
	if ( _texture < 0 ) {
		_texture = DX_NONE_GRAPH;
	}
}

void ModelMaker::drawCoordinateLine( ) const {
	int color_gray = GetColor( 100, 100, 100 );
	for ( int i = 0; i < 2; i++ ) {
		VECTOR pos1 = VGet( 0, 0, 0 );
		VECTOR pos2 = VGet( 0, 0, 0 );
		if ( i == 0 ) {
			pos2.x = COORDINATE_LINE_RANGE;
		} else {
			pos2.y = COORDINATE_LINE_RANGE;
		}
		for ( int j = 0; j < COORDINATE_LINE_RANGE * 2; j++ ) {
			if ( i == 0 ) {
				pos1.y += 0.5;
				pos2.y += 0.5;
			} else {
				pos1.x += 0.5;
				pos2.x += 0.5;
			}
			DrawLine3D( pos1, pos2, color_gray );
		}
	}
	const unsigned int COLOR[ 3 ] = {
		GetColor( 255, 0, 0 ),
		GetColor( 0, 255, 0 ),
		GetColor( 0, 0, 255 )
	};
	for ( int i = 0; i < 3; i++ ) {
		VECTOR pos1 = VGet( 0, 0, 0 );
		VECTOR pos2 = VGet( 0, 0, 0 );
		if ( i == 0 ) {
			pos2.x = COORDINATE_LINE_RANGE;
		}
		if ( i == 1 ) {
			pos2.y = COORDINATE_LINE_RANGE;
		}
		if ( i == 2 ) {
			pos2.z = COORDINATE_LINE_RANGE;
		}
		DrawLine3D( pos1, pos2, COLOR[ i ] );
	}
}
