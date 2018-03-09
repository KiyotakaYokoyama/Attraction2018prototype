#include "Panel.h"
#include "Mouse.h"
#include "define.h"
#include "Drawer.h"
#include "TouchPanel.h"
#include "PentaVertex.h"

Panel::Panel( int idx, PentaVertexPtr penta_vertex ) :
_idx( idx ),
_touch( false ),
_active( false ),
_state( STATE_DEFAULT ),
_penta_vertex( penta_vertex ) {
	int penta_type = idx % 4;
	int hex_idx = idx / 4;
	Vector hex = Vector( HEX_WIDTH * ( hex_idx % HEX_WIDTH_NUM ), HEX_HEIGHT * ( hex_idx / HEX_WIDTH_NUM ) );
	_pos = BOARD_DRAW_POS + hex + PENTA_OFFSET[ _idx % 4 ];
	if ( penta_vertex == PentaVertexPtr( ) ) {
		return;
	}
	for ( int i = 0; i < 5; i++ ) {
		Vector vertex_pos = BOARD_DRAW_POS + hex + penta_vertex->getAdjustPos( penta_type, i );
		_vertex_idx[ i ] = penta_vertex->getPointIdx( vertex_pos );
	}
	//checkInScreen( );
	_in_screen = true;
}


Panel::~Panel( ) {
}

void Panel::update( ) {
	if ( !_in_screen ) {
		return;
	}
	checkTouch( );
}

void Panel::checkTouch( ) {
	MousePtr mouse = Mouse::getTask( );
	_touch = false;

	//クリック判定
	//if ( mouse->isHoldLeftButton( ) ) {
	if ( mouse->isPushLeftButton( ) ) {
		if ( isExistence( getRotationPos( mouse->getPos( ) ) ) ) {
			_touch = true;
		}
	}

	//タップ判定
	TouchPanelPtr panel = TouchPanel::getTask( );
	std::vector< TouchPanel::Touch > tap_pos = panel->getTouchs( );
	size_t size = tap_pos.size( );
	
	for ( int i = 0; i < size; i++ ) {
		if ( isExistence( getRotationPos( tap_pos[ i ].pos ) ) ) {
			_touch = true;
		}
	}
}

void Panel::checkInScreen( ) {
	int blank = ( SCREEN_WIDTH - SCREEN_HIGHT ) / 2;
	int left  = -PANEL_SPRITE_SIZE;
	int right = SCREEN_WIDTH;
	int up    = blank - PANEL_SPRITE_SIZE;
	int down  = blank + SCREEN_HIGHT;

	_in_screen = true;

	if ( _pos.y < up    ) {
		_in_screen = false;
		return;
	}
	if ( _pos.y > down  ) {
		_in_screen = false;
		return;
	}
	if ( _pos.x < left  ) {
		_in_screen = false;
		return;
	}
	if ( _pos.x > right ) {
		_in_screen = false;
		return;
	}
}

void Panel::drawBase( ImagePtr image, int image_idx ) const {
	int x1 = ( int )( _pos.x + 0.5 );
	int y1 = ( int )( _pos.y + 0.5 );
	int x2 = x1 + PANEL_SPRITE_SIZE;
	int y2 = y1 + PANEL_SPRITE_SIZE;
	int tx = _idx % 2 * PANEL_SPRITE_SIZE + ( image_idx % 2 ) * ( PANEL_SPRITE_SIZE * 2 );
	int ty = _idx % 4 / 2 * PANEL_SPRITE_SIZE + ( image_idx / 2 ) * ( PANEL_SPRITE_SIZE * 2 );

	if ( x2 < 0 || y2 < 0 ) {
		return;
	}
	
	image->setRect( tx, ty, PANEL_SPRITE_SIZE, PANEL_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );
}

void Panel::drawDefault( ImagePtr image, int image_idx ) const {
	int x1 = ( int )( _pos.x + 0.5 );
	int y1 = ( int )( _pos.y + 0.5 );
	int x2 = x1 + PANEL_SPRITE_SIZE;
	int y2 = y1 + PANEL_SPRITE_SIZE;
	int tx = _idx % 2 * PANEL_SPRITE_SIZE + ( image_idx % 2 ) * ( PANEL_SPRITE_SIZE * 2 );
	int ty = _idx % 4 / 2 * PANEL_SPRITE_SIZE + ( image_idx / 2 ) * ( PANEL_SPRITE_SIZE * 2 );

	if ( x2 < 0 || y2 < 0 ) {
		return;
	}
	
	image->setRect( tx, ty, PANEL_SPRITE_SIZE, PANEL_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );
}

void Panel::drawBlackPanel( ImagePtr image ) const {
	int x1 = ( int )( _pos.x + 0.5 );
	int y1 = ( int )( _pos.y + 0.5 );
	int x2 = x1 + PANEL_SPRITE_SIZE;
	int y2 = y1 + PANEL_SPRITE_SIZE;
	int tx = _idx % 2 * PANEL_SPRITE_SIZE;
	int ty = _idx % 4 / 2 * PANEL_SPRITE_SIZE;
	ty += PANEL_SPRITE_SIZE * 2;

	image->setRect( tx, ty, PANEL_SPRITE_SIZE, PANEL_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );	
}

void Panel::drawFrame( ImagePtr image, int image_idx ) const {
	int x1 = ( int )( _pos.x + 0.5 );
	int y1 = ( int )( _pos.y + 0.5 );
	int x2 = x1 + PANEL_SPRITE_SIZE;
	int y2 = y1 + PANEL_SPRITE_SIZE;
	int tx = _idx % 2 * PANEL_SPRITE_SIZE + ( image_idx % 2 ) * ( PANEL_SPRITE_SIZE * 2 );
	int ty = _idx % 4 / 2 * PANEL_SPRITE_SIZE + ( image_idx / 2 ) * ( PANEL_SPRITE_SIZE * 2 );

	image->setRect( tx, ty, PANEL_SPRITE_SIZE, PANEL_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );	
}

void Panel::drawBlur( ImagePtr image, double fade ) const {
	image->setBlend( Image::BLEND_ALPHA, fade );
	int x1 = ( int )( _pos.x + 0.5 );
	int y1 = ( int )( _pos.y + 0.5 );
	int x2 = x1 + PANEL_SPRITE_SIZE;
	int y2 = y1 + PANEL_SPRITE_SIZE;
	int tx = _idx % 2 * PANEL_SPRITE_SIZE;
	int ty = _idx % 4 / 2 * PANEL_SPRITE_SIZE;
	tx += PANEL_SPRITE_SIZE * 2;

	image->setRect( tx, ty, PANEL_SPRITE_SIZE, PANEL_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );	
	image->setBlend( Image::BLEND_NONE, 1.0 );
}

void Panel::drawPanelIdx( ) const {
	int x1 = ( int )( _pos.x + 0.5 );
	int y1 = ( int )( _pos.y + 0.5 );
	Drawer::getTask( )->drawString( x1 + 128, y1 + 128, "%d", _idx );
}

bool Panel::isExistence( const Vector& pos ) const {
	if ( pos.x < _pos.x ||
		 pos.y < _pos.y ) {
		return false;
	}
	if ( pos.x > _pos.x + PANEL_SPRITE_SIZE ||
		 pos.y > _pos.y + PANEL_SPRITE_SIZE ) {
		return false;
	}
	for ( int i = 0; i < 5; i++ ) {
		Vector vertex1 = _penta_vertex->getPoint( _vertex_idx[ i ] ).pos;
		Vector vertex2 = _penta_vertex->getPoint( _vertex_idx[ ( i + 1 ) % 5 ] ).pos;
		Vector vec1 = vertex2 - vertex1;
		Vector vec2 = pos - vertex1;
		//内側に無かったら抜ける
		if ( vec1.cross( vec2 ).z < 0 ) {
			return false;
		}
	}
	return true;
}

bool Panel::isTouch( ) const {
	return _touch;
}

int Panel::getIdx( ) const {
	return _idx;
}

void Panel::setTouch( ) {
	_touch = true;
}

Panel::STATE Panel::getState( ) const {
	return _state;
}

void Panel::setState( STATE state ) {
	_state = state;
}

void Panel::initState( ) {
	_state = STATE_DEFAULT;
}

Vector Panel::getPos( ) {
	return _pos;
}

std::array< int, 5 > Panel::getVertexIdx( ) const {
	return _vertex_idx;
}

Vector Panel::getRotationPos( const Vector& pos ) const {
	//画面中心点
	Vector centor( SCREEN_WIDTH / 2, SCREEN_HIGHT / 2 );
	//画面中心からの距離
	Vector rot_pos = pos - centor;
	//画面中心からの距離を回転
	Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI / 2 );
	rot_pos = rot.multiply( rot_pos );
	rot_pos.y += ( SCREEN_WIDTH - SCREEN_HIGHT ) / 2;
	return rot_pos + centor;
}

void Panel::setActive( bool active ) {
	_active = active;
}

bool Panel::isActive( ) const {
	return _active;
}
