#include "Bullet.h"
#include "Drawer.h"
#include "define.h"
#include "Board.h"
#include "Hexagon.h"

const double SIZE_RATIO = 0.002;
const int ERASE_TIME = 1000;

Bullet::Bullet( BoardPtr board, int idx, int time ) :
_board( board ),
_target_time( time ),
_idx( idx ),
_finished( false ) {
	bool right = idx % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) < HEXAGON_RIGHT_WIDTH_NUM;
	int line = ( idx % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) ) - ( right ? 0 : HEXAGON_RIGHT_WIDTH_NUM );
	int row = idx / ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM );
	Vector adjust = Vector( line * PANEL_SPRITE_SIZE, row * PANEL_SPRITE_SIZE );
	if ( right ) {
		adjust += Vector( PANEL_SPRITE_SIZE / 2, 0 );
	} else {
		adjust += Vector( 0, PANEL_SPRITE_SIZE / 2 );
	}
	_pos = BOARD_DRAW_POS + adjust;
	_hex = _board->getHexagon( _idx );
}


Bullet::~Bullet( ) {
}


void Bullet::update( int time ) {
	_size = calcSize( time );
	if ( _size > 1 ) {
		_size = 1;
		if ( !_hex->isActive( ) ) {
			setActiveHex( true );
		}
	}
}

void Bullet::draw( ) const {
	if ( _size < 0 ) {
		return;
	}
	_board->drawBullet( _idx, _size );
}

bool Bullet::isFinished( ) const {
	return _finished;
}

int Bullet::getIdx( ) const {
	return _idx;
}

bool Bullet::canTouch( int time ) const {
	return _size >= 1;
}


double Bullet::calcSize( int time ) const {
	//target_time‚æ‚è‘O‚ÌŽž¬‚³‚­‚·‚é
	return 1 - ( _target_time - time ) * SIZE_RATIO;
}

void Bullet::setFinished( ) {
	_finished = true;
}

void Bullet::setActiveHex( bool value ) {
	_hex->setActive( value );
}

