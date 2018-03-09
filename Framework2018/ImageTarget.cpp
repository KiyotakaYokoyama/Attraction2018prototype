#include "ImageTarget.h"
#include "DxLib.h"


ImageTarget::ImageTarget( ) {
}


ImageTarget::~ImageTarget( ) {
}

void ImageTarget::create( int width, int height ) {
	setHandle( MakeScreen( width, height, TRUE ) );
}

void ImageTarget::clear( ) {
	SetDrawScreen( getHandle( ) );
	ClearDrawScreen( );
	SetDrawScreen( DX_SCREEN_BACK );
}

void ImageTarget::save( std::string path ) {
	if ( path.size( ) == 0 ) {
		return;
	}
	if ( path.find( ".png" ) == std::string::npos ) {
		path += ".png";
	}
	int sx1 = 0;
	int sy1 = 0;
	int sx2 = 0;
	int sy2 = 0;
	getImageSize( sx2, sy2 );
	int draw_screen = GetDrawScreen( );
	SetDrawScreen( getHandle( ) );
	SaveDrawScreenToPNG( sx1, sy1, sx2, sy2, path.c_str( ) );
	SetDrawScreen( draw_screen );
}

