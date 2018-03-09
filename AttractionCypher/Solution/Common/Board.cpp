#include "Board.h"
#include "Drawer.h"
#include "Hexagon.h"
#include "TouchEffecter.h"
#include "Debug.h"

const double FADE_SPEED = 0.01;
const double MAX_FADE = 1.0;
const double MIN_FADE = 0.1;
const int RAND_RESET = 5;

Board::Board( ) {
	DrawerPtr drawer = Drawer::getTask( );
	_image_penta         = drawer->createImage( "penta.png" );
	_image_penta_base    = drawer->createImage( "penta_base.png" );
	_image_penta_default = drawer->createImage( "penta_default.png" );
	_image_pentaframe    = drawer->createImage( "pentaframe.png" );
	_image_hex           = drawer->createImage( "hex.png" );
}

Board::~Board( ) {
}

void Board::init( PentaVertexPtr penta_vertex ) {
	_touch_effecter = TouchEffecterPtr( new TouchEffecter( penta_vertex, shared_from_this( ) ) );
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_panels[ i ] = PanelPtr( new Panel( i, penta_vertex ) );
	}
	for ( int i = 0; i < HEXAGON_BUTTON_NUM; i++ ) {
		_hexagons[ i ] = HexagonPtr( new Hexagon( i, shared_from_this( ), _touch_effecter ) );
	}
	initFading( );
}

void Board::update( ) {
	updatePanels( );
	updateHexagons( );
	fading( );
	//updateTouchedPanelToHexagons( );
}

void Board::updateTouchEffect( ) {
	_touch_effecter->update( );
}

void Board::drawElectric( ) const {
	_touch_effecter->draw( );
}

void Board::drawFrame( ) const {
	//pentaframe全て描画
	srand( RAND_RESET );
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_panels[ i ]->drawFrame( _image_pentaframe, rand( ) % 4 );
	}
}

void Board::drawButton( ) const {
	//Hex描画
	int size = ( int )_button_idx.size( );
	for ( int i = 0; i < size; i++ ) {
		_hexagons[ _button_idx[ i ] ]->drawButton( _image_hex );
	}
}

void Board::drawBullet( int draw_hex_idx, double size ) const {
	//Hex描画
	_hexagons[ draw_hex_idx ]->drawBullet( _image_hex, size );
}

void Board::drawBlackPanel( std::vector< int > draw_penta_idxs ) const {
	//黒penta描画
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		if ( isMacthPanel( draw_penta_idxs, i ) ) {
			_panels[ i ]->drawBlackPanel( _image_penta );
		}
	}
}

void Board::drawBlur( std::vector< int > draw_penta_idxs ) const {
	//ぼかし描画
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		if ( isMacthPanel( draw_penta_idxs, i ) ) {
			_panels[ i ]->drawBlur( _image_penta, _fade );
		}
	}
}

void Board::drawPanelForIllust( PLAYER player ) const {
	//黒penta描画
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		//イラストを表示する場所以外は黒？
		switch ( _panels[ i ]->getState( ) ) {
		case Panel::STATE_PLAYER1:
		case Panel::STATE_PLAYER2:
			 if ( _panels[ i ]->getState( ) != Panel::STATE_PLAYER1 + player ) {
				_panels[ i ]->drawBlackPanel( _image_penta );
			 }
			break;
		default:
			_panels[ i ]->drawBlackPanel( _image_penta );
		}
	}
}

void Board::drawBase( ) const {
	//五角形ベース表示
	srand( RAND_RESET );
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_panels[ i ]->drawBase( _image_penta_base, rand( ) % 4 );
	}
}

void Board::drawDefault( ) const {
	//五角形標準表示
	srand( RAND_RESET );
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		int randam = rand( ) % 4;
		if ( _panels[ i ]->getState( ) == Panel::STATE_DEFAULT ) {
			_panels[ i ]->drawDefault( _image_penta_default, randam );
		}
	}
}

void Board::drawIdx( ) const {
	if ( Debug::getTask( )->isDebug( ) ) {
		for ( int i = 0; i < PENTA_NUM; i++ ) {
			_panels[ i ]->drawPanelIdx( );
		}
		for ( int i = 0; i < HEXAGON_BUTTON_NUM; i++ ) {
			_hexagons[ i ]->drawHexagonIdx( );
		}
	}
}

void Board::updatePanels( ) {
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_panels[ i ]->update( );
	}
}

void Board::updateHexagons( ) {
	for ( int i = 0; i < HEXAGON_BUTTON_NUM; i++ ) {
		_hexagons[ i ]->update( );
	}
}

void Board::updateTouchedPanelToHexagons( ) {
	for ( int panel_idx = 0; panel_idx < PENTA_NUM; panel_idx++ ) {
		if ( _panels[ panel_idx ]->isTouch( ) ) {
			checkTouchedHexagons( panel_idx );
			break;
		}
	}
}

void Board::checkTouchedHexagons( int touch_panel ) {	
	for ( int hex_idx = 0; hex_idx < HEXAGON_BUTTON_NUM; hex_idx++ ) {
		if ( _hexagons[ hex_idx ]->isInPanel( touch_panel ) && _hexagons[ hex_idx ]->isActive( ) ) {
			std::array< int, HEXAGON_IN_PANEL_NUM > idx = _hexagons[ hex_idx ]->getHexInPentaIdx( );
			//ここでタッチされたヘキサグラムの演出を描画

			//デバッグ用　タッチされたヘキサグラムの中のパネルを光らせる
			for ( int j = 0; j < HEXAGON_IN_PANEL_NUM; j++ ) {
				_panels[ idx[ j ] ]->setTouch( );
				_panels[ idx[ j ] ]->setActive( false );
			}
		}
	}
}

bool Board::isMacthPanel( std::vector< int > clear_panels, int idx ) const {
	int size = ( int )clear_panels.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( clear_panels[ i ] == idx ) {
			return true;
		}
	}
	return false;
}

int Board::getPanelIdx( const Vector& pos ) const {
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		if ( _panels[ i ]->isExistence( pos ) ) {
			return i;
		}
	}
	return -1;
}

bool Board::isTouchPanel( int idx ) const {
	return _panels[ idx ]->isTouch( );
}

bool Board::isTouchHexagon( int hex_idx ) const {
	if ( !_hexagons[ hex_idx ]->isActive( ) ) {
		return false;
	}
	return _hexagons[ hex_idx ]->isTouch( );
}

bool Board::isActiveHexagon( int hex_idx ) const {
	bool result = false;
	std::array< int, HEXAGON_IN_PANEL_NUM > penta_idx = _hexagons[ hex_idx ]->getHexInPentaIdx( );
	for ( int idx : penta_idx ) {
		if ( _panels[ idx ]->isActive( ) ) {
			result = true;
			break;
		}
	}
	return result;
}

void Board::initPanelState( ) {
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_panels[ i ]->initState( );
	}
}

void Board::setPanelState( Panel::STATE state, std::vector< int > panels ) {
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		if ( isMacthPanel( panels, i ) ) {
			_panels[ i ]->setState( state );
		}
	}
}

void Board::setActiveHexagon( int idx, bool active ) {
	_hexagons[ idx ]->setActive( active );
	std::array< int, HEXAGON_IN_PANEL_NUM > penta_idx = _hexagons[ idx ]->getHexInPentaIdx( );
	for ( int p_idx : penta_idx ) {
		_panels[ p_idx ]->setActive( active );
	}
}

std::array< int, HEXAGON_IN_PANEL_NUM > Board::getHexInPentaIdx( int hex_idx ) const {
	return _hexagons[ hex_idx ]->getHexInPentaIdx( );
}

PanelPtr Board::getPanel( int idx ) const {
	return _panels[ idx ];
}

HexagonPtr Board::getHexagon( int idx ) const {
	return _hexagons[ idx ];
}


void Board::setWinnerIllust( const Panel::STATE winner ) {
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_panels[ i ]->setState( winner );
	}
}


void Board::addButton( int hex_idx ) {
	_button_idx.push_back( hex_idx );
	_hexagons[ hex_idx ]->setActive( true );
}


void Board::addEffectHex( int hex_idx ) {
	_touch_effecter->addEffectHex( hex_idx );
}

void Board::addEffectPenta( int penta_idx ) {
	_touch_effecter->addEffectPenta( penta_idx );
}

void Board::clearButton( ) {
	int size = ( int )_button_idx.size( );
	for ( int i = 0; i < size; i++ ) {
		_hexagons[ _button_idx[ i ] ]->setActive( false );
	}
	_button_idx.clear( );
}

void Board::fading( ) {
	_fade += FADE_SPEED * ( _fade_in ? 1 : -1 );

	if ( _fade > MAX_FADE || _fade < MIN_FADE ) {
		_fade_in = !_fade_in;
	}

}

void Board::initFading( ) {
	_fade = 0;
}
