#include "Image.h"
#include "DxLib.h"

Image::Image( ) :
_sx1( 0 ),
_sy1( 0 ),
_tx( -1 ),
_ty( -1 ),
_tw( -1 ),
_th( -1 ),
_sx2( -1 ),
_sy2( -1 ),
_blend( BLEND_NONE ),
_ratio( 1.0 ),
_center_sx( -1 ),
_center_sy( -1 ),
_center_tx( -1 ),
_center_ty( -1 ),
_angle( 0.0 ),
_expansion( Vector( 1, 1 ) ) {
}


Image::~Image( ) {
	if ( _handle >= 0 ) {
		DeleteGraph( _handle );
	}
}

bool Image::load( std::string path ) {
	_handle = LoadGraph( path.c_str( ) );
	if ( _handle < 0 ) {
		return false;
	}
	return true;
}

void Image::setBlend( BLEND blend, double ratio ) {
	_ratio = ratio;
	_blend = blend;
}

void Image::draw( ) const {
	switch ( _blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * _ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * _ratio ) );
		break;
	case BLEND_SUB:
		SetDrawBlendMode( DX_BLENDMODE_SUB  , ( int )( 255 * _ratio ) );
		break;
	}

	if ( _tw < 0 ) {
		DrawGraph( _sx1, _sy1, _handle, TRUE );
	} else if ( _sy2 < 0 ) {
		DrawRectGraph( _sx1, _sy1, _tx, _ty, _tw, _th, _handle, TRUE, FALSE );
	} else {
		DrawRectExtendGraph(
			_sx1, _sy1,
			_sx2, _sy2,
			_tx , _ty ,
			_tw , _th , _handle, TRUE );
	}

	if ( _blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}

void Image::drawRotation( ) const {
	switch ( _blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * _ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * _ratio ) );
		break;
	case BLEND_SUB:
		SetDrawBlendMode( DX_BLENDMODE_SUB  , ( int )( 255 * _ratio ) );
		break;
	}
		
	DrawRotaGraph3( _center_sx, _center_sy, _center_tx, _center_ty, _expansion.x, _expansion.y, _angle, _handle, TRUE, FALSE );
	
	if ( _blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}

void Image::setRect( int tx, int ty, int tw, int th ) {
	_tx = tx;
	_ty = ty;
	_tw = tw;
	_th = th;
}

void Image::setPos( const int sx1, const int sy1, const int sx2, const int sy2 ) {
	_sx1 = sx1;
	_sy1 = sy1;
	_sx2 = sx2;
	_sy2 = sy2;
}

void Image::setImageRotation( int c_sx, int c_sy, int c_tx, int c_ty, double radian, Vector expansion ) {
	_center_sx = c_sx;
	_center_sy = c_sy;
	_center_tx = c_tx;
	_center_ty = c_ty;
	_angle = radian;
	_expansion = expansion;
}

void Image::getImageSize( int& width, int& height ) const {
	GetGraphSize( _handle, &width, &height );
}

int Image::getHandle( ) const {
	return _handle;
}

void Image::setHandle( int handle ) {
	_handle = handle;
}

