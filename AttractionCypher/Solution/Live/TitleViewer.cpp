#include "TitleViewer.h"
#include "Image.h"
#include "Drawer.h"
#include "Board.h"
#include "Application.h"
#include "ImageTarget.h"
#include "MusicSync.h"

const int TITLE_SPRITE_WIDTH = 400;
const int TITLE_SPRITE_HEIGHT = 224;
const int EFFECT_HEX_IDX = 80;

PTR( Music );

TitleViewer::TitleViewer( BoardPtr board, MusicSyncPtr sync ) :
_board( board ),
_sync( sync ),
_before_playing_time( 0 ) {
	_title = Drawer::getTask( )->createImage( "title/Cypher-logo.png" );
	_title->setPos( SCREEN_WIDTH / 2 - TITLE_SPRITE_WIDTH / 2, SCREEN_WIDTH / 2 - TITLE_SPRITE_HEIGHT / 2 );
	initScreen( );
	sync->initInterval( false );
}

TitleViewer::~TitleViewer( ) {
}

void TitleViewer::initScreen( ) {
	_screen = ImageTargetPtr( new ImageTarget );
	_screen->create( SCREEN_WIDTH, SCREEN_WIDTH );// ゲーム画面の縦長に対応するため正方形
	_screen->setPos( 0, -( SCREEN_WIDTH / 2 - SCREEN_HIGHT / 2 ) );
}

void TitleViewer::init( ) {
	_board->initPanelState( );
	std::vector< int > ary;

	ary.push_back( 159 );
	ary.push_back( 162 );
	ary.push_back( 163 );
	ary.push_back( 166 );
	ary.push_back( 193 );
	ary.push_back( 196 );
	ary.push_back( 197 );
	ary.push_back( 201 );
	_board->setPanelState( Panel::STATE_NONE, ary );

	ary.clear( );
	ary.push_back( 156 );
	ary.push_back( 157 );
	ary.push_back( 158 );
	ary.push_back( 160 );
	ary.push_back( 161 );
	ary.push_back( 164 );
	ary.push_back( 165 );
	ary.push_back( 192 );
	ary.push_back( 194 );
	ary.push_back( 195 );
	ary.push_back( 199 );
	ary.push_back( 200 );
	ary.push_back( 201 );
	_board->setPanelState( Panel::STATE_FADE, ary );
}

void TitleViewer::update( ) {
	if ( _sync->getPlayingTime( ) > MUSIC_INFO[ _sync->getNowMusic( ) ].total_time ) {
		_sync->initInterval( false );
	}
	updateBeatEffect( );
	_board->updateTouchEffect( );
}


void TitleViewer::updateBeatEffect( ) {
	int now = _sync->getPlayingTime( );
	if ( _before_playing_time > now ) {
		//曲が切り替わった
	}
	int beat_length = 0;
	if ( now < 0 ) {
		//インターバルの時
		beat_length = MusicPtr( )->getBeatLength( INTERVAL_INFO.bpm );
	} else {
		//バトルの時
		beat_length = MusicPtr( )->getBeatLength( MUSIC_INFO[ _sync->getNowMusic( ) ].bpm );
	}
	int beafore_beat_count = _before_playing_time / beat_length / 2;
	int now_beat_count     = now                  / beat_length / 2;
	if ( beafore_beat_count != now_beat_count ) {
		_board->addEffectHex( EFFECT_HEX_IDX );
	}
	_before_playing_time = now;
}

void TitleViewer::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setImageTarget( _screen );
	_board->drawBase( );
	_title->draw( );
	_board->drawDefault( );
	_board->drawFrame( );
	_board->drawIdx( );
	_board->drawElectric( );
	drawer->setImageTarget( );
	_screen->draw( );
}
