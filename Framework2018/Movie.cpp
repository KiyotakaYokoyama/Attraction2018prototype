#include "Movie.h"
#include "DxLib.h"
#include "Application.h"

Movie::Movie( ) :
_handle( -1 ) {
	_blend = BLEND_NONE;
	_ratio = 1.0;
}

Movie::~Movie( ) {
	if ( _handle >= 0 ) {
		DeleteGraph( _handle );
	}
}

void Movie::setBlend( BLEND blend, double ratio ) {
	_blend = blend;
	_ratio = ratio;
}

bool Movie::load( std::string path, bool alpha ) {
	const char * str = path.c_str( );
	SetMovieColorA8R8G8B8Flag( alpha ? TRUE : FALSE );
	_handle = LoadGraph( path.c_str( ) );
	if ( _handle < 0 ) {
		return false;
	}
	return true;
}

void Movie::draw( int sx, int sy, int sw, int sh ) const {
	switch ( _blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * _ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * _ratio ) );
		break;
	}
	
	if ( sw == 0 ) {
		DrawGraph( sx, sy, _handle, TRUE );
	} else {
		DrawExtendGraph( sx, sy, sx + sw, sy + sh, _handle, TRUE );
	}

	if ( _blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}

void Movie::stop( ) {
	PauseMovieToGraph( _handle );
}

void Movie::play( bool is_loop ){
	if ( is_loop ) {
		PlayMovieToGraph( _handle, DX_PLAYTYPE_LOOP );
	} else {
		PlayMovieToGraph( _handle );
	}
}

bool Movie::isPlaying( ) const {
	return GetMovieStateToGraph( _handle ) != 0;
}

int Movie::getSeek( ) {
	return TellMovieToGraph( _handle );
}

void Movie::setSeek( int time ) {
	SeekMovieToGraph( _handle, time );
}

void Movie::setSoundVolume( int volume ) {
	SetMovieVolumeToGraph( volume, _handle );
}