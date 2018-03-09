#include "SceneWaitScreen.h"
#include "Board.h"
#include "Drawer.h"
#include "ImageTarget.h"
#include "MusicSync.h"
#include "Music.h"
#include "Debug.h"
#include <vector>

PTR( Music );

const int BUTTON_HEX_IDX = 114;
const int TITLE_SPRITE_WIDTH = 400;
const int TITLE_SPRITE_HEIGHT = 224;
const int BEET_EFFECT_HEX_IDX = 80;

SceneWaitScreen::SceneWaitScreen( BoardPtr board, MusicSyncPtr sync ) :
Scene( board ),
_sync( sync ),
_before_playing_time( 0 ) {
	board->clearButton( );
	board->addButton( BUTTON_HEX_IDX );
	_sync->initInterval( false );
	_title = Drawer::getTask( )->createImage( "title/Cypher-logo.png" );
	_title->setPos( SCREEN_WIDTH / 2 - TITLE_SPRITE_WIDTH / 2, SCREEN_WIDTH / 2 - TITLE_SPRITE_HEIGHT / 2 );
	
	std::vector< int > ary;
	ary.push_back( 157 );
	ary.push_back( 160 );
	ary.push_back( 161 );
	ary.push_back( 164 );
	ary.push_back( 159 );
	ary.push_back( 162 );
	ary.push_back( 163 );
	ary.push_back( 166 );
	ary.push_back( 193 );
	ary.push_back( 197 );
	ary.push_back( 196 );
	ary.push_back( 158 );
	board->setPanelState( Panel::STATE_NONE, ary );
}

SceneWaitScreen::~SceneWaitScreen( ) {
}

Scene::SCENE SceneWaitScreen::update( ) {
	if ( _sync->getPlayingTime( ) > MUSIC_INFO[ _sync->getNowMusic( ) ].total_time ) {
		_sync->initInterval( false );
	}
	updateBeatEffect( );
	BoardPtr board = getBoard( );
	board->update( );
	board->updateTouchEffect( );

	ImageTargetPtr screen = getScreen( );
	screen->clear( );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setImageTarget( screen );
	board->drawBase( );      //ペンタベース
	drawTitle( );
	board->drawDefault( );
	board->drawFrame( );
	board->drawIdx( );      //デバック表示
	board->drawElectric( ); //エフェクト
	board->drawButton( );
	drawer->setImageTarget( );
	screen->drawRotation( );
	if ( isTouchButton( ) ) {
		return SCENE_SELECT;
	}
	if ( Debug::getTask( )->isDebug( ) ) {
		int now = _sync->getPlayingTime( );
		drawer->drawString( 0, 0, "Now:%d(%d秒)", now, now / 1000 );
	}
	return SCENE_CONTINUE;
}

bool SceneWaitScreen::isTouchButton( ) {
	bool result = false;
	if ( getBoard( )->isTouchHexagon( BUTTON_HEX_IDX ) ) {
		result = true;
	}
	return result;
}

void SceneWaitScreen::updateBeatEffect( ) {
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
		getBoard( )->addEffectHex( BEET_EFFECT_HEX_IDX );
	}
	_before_playing_time = now;
}

void SceneWaitScreen::drawTitle( ) const {
	_title->draw( );
}
