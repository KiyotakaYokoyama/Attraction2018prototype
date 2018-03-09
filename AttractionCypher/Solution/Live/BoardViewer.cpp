#include "BoardViewer.h"
#include "ImageTarget.h"
#include "Board.h"
#include "Drawer.h"
#include "DataBase.h"
#include "MusicSync.h"

const int REFRESH_COUNT = 10;
const int OCCUPANCY_DRAW_INTERVAL_X = 600;
const int NUM_SIZE = 10;
const int NUM_SPRITE_SIZE = 128;
const int ILLUST_PANEL_NAM = 47;

BoardViewer::BoardViewer( BoardPtr board, MusicSyncPtr sync ) :
_count( 0 ),
_board( board ),
_sync( sync ),
_state( STATE_INTERVAL ) {
	_number = Drawer::getTask( )->createImage( "number.png" );
	loadIllusts( );
	initScreen( );
	initIllustScreen( );
}


BoardViewer::~BoardViewer( ) {
}

void BoardViewer::init( ) {
	_board->initPanelState( );
	std::vector< int > ary;
	ary.push_back( 186 );
	ary.push_back( 187 );
	ary.push_back( 220 );
	ary.push_back( 221 );
	ary.push_back( 190 );
	ary.push_back( 183 );
	ary.push_back( 224 );
	ary.push_back( 217 );

	ary.push_back( 206 );
	ary.push_back( 207 );
	ary.push_back( 240 );
	ary.push_back( 241 );
	ary.push_back( 210 );
	ary.push_back( 211 );
	ary.push_back( 244 );
	ary.push_back( 245 );
	_board->setPanelState( Panel::STATE_NONE, ary );
}

void BoardViewer::initScreen( ) {
	_screen = ImageTargetPtr( new ImageTarget );
	_screen->create( SCREEN_WIDTH, SCREEN_WIDTH );// ゲーム画面の縦長に対応するため正方形
	_screen->setPos( 0, -( SCREEN_WIDTH / 2 - SCREEN_HIGHT / 2 ) );
}

void BoardViewer::loadIllusts( ) {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < MAX_ILLUST_NUM; i++ ) {
		std::string player = "player/player" + std::to_string( i + 1 ) + ".jpg";
		_illust[ i ] = drawer->createImage( player.c_str( ) );
		int tw = 0;
		int th = 0;
		_illust[ i ]->getImageSize( tw, th );
		_illust[ i ]->setPos( SCREEN_WIDTH / 2 - tw / 2, SCREEN_WIDTH / 2 - th / 2 );
	}
}

void BoardViewer::initIllustScreen( ) {
	for ( int i = 0; i < MAX_PLAYER; i++ ) {
		_illust_for_screen[ i ] = ImageTargetPtr( new ImageTarget );
		_illust_for_screen[ i ]->create( SCREEN_WIDTH, SCREEN_WIDTH );
		_illust_for_screen[ i ]->setBlend( Image::BLEND_ADD, 1.0 );
	}
	_illust_draw_screen = ImageTargetPtr( new ImageTarget );
	_illust_draw_screen->create( SCREEN_WIDTH, SCREEN_WIDTH );
}

void BoardViewer::update( ) {
	//_count++;
	////通信削減のため何フレームかに一回のみ更新
	//if ( _count % REFRESH_COUNT != 0 ) {
	//	return;
	//}
	//領域取得
	_sync->update( );
}

void BoardViewer::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	_screen->clear( );
	drawer->setImageTarget( _screen );
	_board->drawBase( );      //バックパネル
	drawPanelIllusts( );      //イラスト
	_board->drawDefault( );   //標準
	drawOccupancy( );         //占領率
	_board->drawFrame( );     //フレーム
	_board->drawIdx( );       //デバック表示
	drawer->setImageTarget( );
	_screen->draw( );
}


void BoardViewer::drawPanelIllusts( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Room room = _sync->getRoom( );
	for ( int i = 0; i < MAX_PLAYER; i++ ) {
		_illust_for_screen[ i ]->clear( );
		drawer->setImageTarget( _illust_for_screen[ i ] );
		//---描画用イラストを作成---//
		_illust[ room.illust[ i ] ]->draw( );
		_board->drawPanelForIllust( ( PLAYER )i );
		//---描画用イラストを作成ここまで---/
	}
	_illust_draw_screen->clear( );
	drawer->setImageTarget( _illust_draw_screen );
	for ( int i = 0; i < MAX_PLAYER; i++ ) {
		_illust_for_screen[ i ]->draw( );
	}
	drawer->setImageTarget( _screen );
	_illust_draw_screen->draw( );
}

void BoardViewer::drawOccupancy( ) const {
	//占有率
	//パネルを数える
	int player_panels[ 2 ] = { 0 };
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		switch ( _board->getPanel( i )->getState( ) ) {
		case Panel::STATE_DEFAULT:
			break;
		case Panel::STATE_PLAYER1:
			player_panels[ 0 ]++;	
			break;
		case Panel::STATE_PLAYER2:
			player_panels[ 1 ]++;
			break;
		}
	}
	//各プレイヤーの占有率を表示
	int y = SCREEN_WIDTH - SCREEN_HIGHT / 2 - 128;
	double percent[ 2 ] = { 0 };
	percent[ 0 ] = ( double )player_panels[ 0 ] / ILLUST_PANEL_NAM * 100;
	percent[ 1 ] = ( double )player_panels[ 1 ] / ILLUST_PANEL_NAM * 100;
	for ( int i = 0; i < 2; i++ ) {
		const int DIGIT = ( int )log10( percent[ i ] ) + 1;
		int sx = i * 1480 + 320;
		//小数点以上
		for ( int j = 0; j < DIGIT + 3; j++ ) {
			int divided = ( int )pow( 10, j - 3 );
			if ( divided < 1 ) divided = 1;
			int tx = ( ( int )percent[ i ] / divided ) % NUM_SIZE * NUM_SPRITE_SIZE;
			int ty = 0;
			if ( j == 2 ) {//小数点
				tx = NUM_SPRITE_SIZE * 10;
			}
			if ( j == 1 ) {//小数点以下
				tx = NUM_SPRITE_SIZE * ( ( int )( percent[ i ] * 10 ) % 10 );
			}
			if ( j == 0 ) {//%
				tx = NUM_SPRITE_SIZE * 11;
			}
			_number->setRect( tx, ty, NUM_SPRITE_SIZE, NUM_SPRITE_SIZE );
			_number->setPos( sx, y );
			_number->draw( );
			sx -= ( int )( NUM_SPRITE_SIZE * 0.65 );
		}
	}
}
