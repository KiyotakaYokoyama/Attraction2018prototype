#include "Hexagon.h"
#include "Board.h"
#include "define.h"
#include "TouchEffecter.h"
#include "Drawer.h"

#include <assert.h>

const int HEXA_BASE_IDX[ HEXAGON_IN_PANEL_NUM * 2 ] = {
	//RIGHT_PANELS
	1, 3, 4, 6,
	//LEFT_PANELS
	2, 3, HEX_WIDTH_NUM * HEXAGON_IN_PANEL_NUM, HEX_WIDTH_NUM * HEXAGON_IN_PANEL_NUM + 1
};

Hexagon::Hexagon( int idx, BoardPtr board, TouchEffecterPtr touch_effecter ) :
_idx( idx ),
_active( false ),
_touch( false ),
_board( board ),
_touch_effecter( touch_effecter ) {
	bool right = idx % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) < HEXAGON_RIGHT_WIDTH_NUM;
	_form = right ? FORM_RIGHT : FORM_LEFT;
	int line = ( idx % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) ) - ( right ? 0 : HEXAGON_RIGHT_WIDTH_NUM );
	int row = idx / ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM );
	Vector adjust = Vector( line * PANEL_SPRITE_SIZE, row * PANEL_SPRITE_SIZE );
	if ( right ) {
		adjust += Vector( PANEL_SPRITE_SIZE / 2, 0 );
	} else {
		adjust += Vector( 0, PANEL_SPRITE_SIZE / 2 );
	}
	_pos = BOARD_DRAW_POS + adjust;

	int check_idx = idx % ( HEXAGON_RIGHT_WIDTH_NUM + HEXAGON_LEFT_WIDTH_NUM );
	int plus_x = HEXAGON_IN_PANEL_NUM * ( right ? ( check_idx % HEXAGON_RIGHT_WIDTH_NUM ) : ( ( check_idx - HEXAGON_RIGHT_WIDTH_NUM ) % HEXAGON_LEFT_WIDTH_NUM ) );
	int plus_y = ( HEX_WIDTH_NUM * HEXAGON_IN_PANEL_NUM ) * ( idx / ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) );
	for ( int i = 0; i < HEXAGON_IN_PANEL_NUM; i++ ) {
		int idx = HEXA_BASE_IDX[ i + _form * HEXAGON_IN_PANEL_NUM ] + plus_x + plus_y;
		assert( idx >= 0 && idx < PENTA_NUM );
		_have_penta_idx[ i ] = idx;
		_have_penta[ i ] = board->getPanel( idx );
	}
}

Hexagon::~Hexagon( ) {
}

void Hexagon::update( ) {
	_touch = false;
	if ( !isActive( ) ) {
		return;
	}
	//Touch
	for ( int i = 0; i < HEXAGON_IN_PANEL_NUM; i++ ) {
		if ( _have_penta[ i ]->isTouch( ) ) {
			_touch = true;
			break;
		}
	}

	//Effect
	if ( _touch ) {
		_touch_effecter->addEffectHex( _idx );
	}
}


void Hexagon::drawButton( ImagePtr image ) const {
	int central_x = ( int )( _pos.x + 0.5 );
	int central_y = ( int )( _pos.y + 0.5 );
	int x1 = central_x - HEXAGON_SPRITE_SIZE / 2;
	int x2 = central_x + HEXAGON_SPRITE_SIZE / 2;
	int y1 = central_y - HEXAGON_SPRITE_SIZE / 2;
	int y2 = central_y + HEXAGON_SPRITE_SIZE / 2;
	int tx = 0;
	if ( _idx % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) < HEXAGON_RIGHT_WIDTH_NUM ) {
		tx = HEXAGON_SPRITE_SIZE;
	}
	int ty = 0;
	if ( x2 < 0 || y2 < 0 ) {
		return;
	}
	image->setRect( tx, ty, HEXAGON_SPRITE_SIZE, HEXAGON_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );
}

void Hexagon::drawBullet( ImagePtr image, double size ) const {
	//target_timeより前の時小さくする
	if ( size < 0 ) {
		return;
	}
	if ( size > 1 ) {
		size = 1;
	}

	int draw_size = ( int )( HEXAGON_SPRITE_SIZE * size );
	int central_x = ( int )( _pos.x + 0.5 );
	int central_y = ( int )( _pos.y + 0.5 );
	int x1 = central_x - draw_size / 2;
	int x2 = central_x + draw_size / 2;
	int y1 = central_y - draw_size / 2;
	int y2 = central_y + draw_size / 2;
	int tx = 0;
	if ( _idx % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) < HEXAGON_RIGHT_WIDTH_NUM ) {
		tx = HEXAGON_SPRITE_SIZE;
	}
	int ty = 0;
	if ( x2 < 0 || y2 < 0 ) {
		return;
	}
	image->setRect( tx, ty, HEXAGON_SPRITE_SIZE, HEXAGON_SPRITE_SIZE );
	image->setPos( x1, y1, x2, y2 );
	image->draw( );
}

void Hexagon::drawHexagonIdx( ) const {
	int sx = ( int )( _pos.x + 0.5 );
	int sy = ( int )( _pos.y + 0.5 ) + 20;
	Drawer::getTask( )->drawString( sx, sy, "idx:%d  %s", _idx, ( _form == FORM_RIGHT ? "right" : "left" ) );
}

bool Hexagon::isTouch( ) const {
	return _touch;
}

bool Hexagon::isInPanel( int idx ) {
	bool result = false;
	//ヘキサグラムの中のパネルがタッチされたidxと一致するか確認
	for ( int i = 0; i < HEXAGON_IN_PANEL_NUM; i++ ) {
		if ( idx == _have_penta_idx[ i ] ) {
			result = true;
			break;
		}
	}

	return result;
}

void Hexagon::setActive( bool active ) {
	_active = active;
}

bool Hexagon::isActive( ) const {
	return _active;
}

int Hexagon::getIdx( ) const {
	return _idx;
}

PanelPtr Hexagon::getInPenta( int idx ) const {
	return _have_penta[ idx ];
}

std::array< int, HEXAGON_IN_PANEL_NUM > Hexagon::getHexInPentaIdx( ) const {
	return _have_penta_idx;
}
