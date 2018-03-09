#include "SceneSelect.h"
#include "Drawer.h"
#include "Image.h"
#include "ImageTarget.h"
#include "Board.h"
#include "Game.h"
#include "FadePanelPos.h"
#include "DataBase.h"
#include "MusicSync.h"
#include "Ntp.h"
#include <string>
#include <vector>

const int CLEAR_PANEL_WIDTH_NUM = 12;
const int CLEAR_PANEL_HEIGHT_NUM = 4;

const int NEXT_BUTTON_HEX_IDX = 81;
const int PREV_BUTTON_HEX_IDX = 113;
const int DECISION_BUTTON_HEX_IDX = 106;

const int ONLINE_TIME = 1000 * 20;

const int ILLUST_SIZE = 1024;
const int ILLUST_DRAW_X = ( SCREEN_WIDTH - ILLUST_SIZE ) / 2;
const int ILLUST_DRAW_Y = ( SCREEN_WIDTH - ILLUST_SIZE ) / 2 - 240;


const std::vector< int > ILLUST_HEX_IDX = {
	//左向き
	HEXAGON_BOTH_WIDTH_NUM * 2 + 2, HEXAGON_BOTH_WIDTH_NUM * 2 + 3, HEXAGON_BOTH_WIDTH_NUM * 2 + 4,
	HEXAGON_BOTH_WIDTH_NUM * 3 + 2, HEXAGON_BOTH_WIDTH_NUM * 3 + 3, HEXAGON_BOTH_WIDTH_NUM * 3 + 4,
	HEXAGON_BOTH_WIDTH_NUM * 4 + 2, HEXAGON_BOTH_WIDTH_NUM * 4 + 3, HEXAGON_BOTH_WIDTH_NUM * 4 + 4,
	//右向き
	HEXAGON_BOTH_WIDTH_NUM * 2 + 11, HEXAGON_BOTH_WIDTH_NUM * 2 + 12, HEXAGON_BOTH_WIDTH_NUM * 2 + 13,
	HEXAGON_BOTH_WIDTH_NUM * 3 + 11, HEXAGON_BOTH_WIDTH_NUM * 3 + 12, HEXAGON_BOTH_WIDTH_NUM * 3 + 13,
	HEXAGON_BOTH_WIDTH_NUM * 4 + 11, HEXAGON_BOTH_WIDTH_NUM * 4 + 12, HEXAGON_BOTH_WIDTH_NUM * 4 + 13,
};

SceneSelect::SceneSelect( BoardPtr board, MusicSyncPtr sync ) :
Scene( board ),
_select_illust( 0 ),
_touch( 0 ),
_check_count( 0 ),
_sync( sync ) {
	setButton( );
	createIllusts( );
	checkClearPanel( );
	board->setPanelState( Panel::STATE_FADE, FadePneles::FADE_PANEL );
	_db = DataBasePtr( new DataBase );
	_ntp = NtpPtr( new Ntp ); 
	checkIllustEnableSelect( );
	initSelectIllust( );
}

SceneSelect::~SceneSelect( ) {
}

Scene::SCENE SceneSelect::update( ) {
	SCENE result = SCENE_CONTINUE;
	BoardPtr board = getBoard( );
	board->update( );
	board->updateTouchEffect( );
	checkIllustEnableSelect( );
	changeIllusts( );
	checkTap( );
	draw( );
	if ( isReturnTitle( ) ){
		result = SCENE_WAIT_SCREEN;
	}
	if ( decisionIllust( ) ) {
		int p1 = ( _select_illust ) % ( MAX_ILLUST_NUM );
		_sync->setPlayerIllust( p1 );
		result = SCENE_PLAY;
	}
	return result;
}

void SceneSelect::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	BoardPtr board = getBoard( );
	ImageTargetPtr screen = getScreen( );
	screen->clear( ); //前回の描画をクリア
	//---ImageTargetに描画開始--//
	drawer->setImageTarget( screen );
	board->drawBase( );                        //ペンタベース
	_illusts[ _select_illust ]->draw( );       //イラスト
											//文字
	board->drawBlur( FadePneles::FADE_PANEL ); //ぼかし
	board->drawDefault( );	                   //標準
	board->drawFrame( );	                   //フレーム
	board->drawIdx( );                         //デバック表示
	board->drawElectric( );	                   //びりびり
	board->drawButton( );	                   //ボタン
	drawer->setImageTarget( );
	//---ImageTargetに描画ここまで--//
	screen->drawRotation( ); //回転描画
}

void SceneSelect::changeIllusts( ) {
	int select = 0;
	bool touch = false;
	BoardPtr board = getBoard( );

	if ( board->isTouchHexagon( NEXT_BUTTON_HEX_IDX ) ) {
		select++;
		touch = true;
	}
	
	if ( board->isTouchHexagon( PREV_BUTTON_HEX_IDX ) ) {
		select--;
		touch = true;
	}
	//Hold防止
	if ( _touch == 0 || ( _touch > 30 && _touch % 10 == 0 ) ) {
		_select_illust = ( _select_illust + select + MAX_ILLUST_NUM ) % MAX_ILLUST_NUM;
		//選択できないイラストの場合使えるイラストまで選択を変える
		while ( !_enable_select[ _select_illust ] ) {
			_select_illust = ( _select_illust + select + MAX_ILLUST_NUM ) % MAX_ILLUST_NUM;
		}
	}
	if ( touch ) {
		_touch++;
	} else {
		_touch = 0;
	}
}

bool SceneSelect::decisionIllust( ) {
	if ( getBoard( )->isTouchHexagon( DECISION_BUTTON_HEX_IDX ) ) {
		return true;
	}
	return false;
}


void SceneSelect::setButton( ) {
	BoardPtr board = getBoard( );
	board->clearButton( );
	board->addButton( NEXT_BUTTON_HEX_IDX     );
	board->addButton( PREV_BUTTON_HEX_IDX     );
	board->addButton( DECISION_BUTTON_HEX_IDX );
}

void SceneSelect::createIllusts( ) {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < MAX_ILLUST_NUM; i++ ) {
		std::string filename = "player/player" + std::to_string( i + 1 ) + ".jpg";
		_illusts[ i ] = drawer->createImage( filename.c_str( ) );
		_illusts[ i ]->setPos( ILLUST_DRAW_X, ILLUST_DRAW_Y );
	}
}

void SceneSelect::checkClearPanel( ) {
	BoardPtr board = getBoard( );
	std::vector< int > clear_panel_idx;
	int hex_num = ( int )ILLUST_HEX_IDX.size( );
	for ( int i = 0; i < hex_num; i++ ) {
		std::array< int, HEXAGON_IN_PANEL_NUM > penta = board->getHexInPentaIdx( ILLUST_HEX_IDX[ i ] );
		for ( int j = 0; j < HEXAGON_IN_PANEL_NUM; j++ ) {
			bool existence = false;
			for ( int k = 0; k < ( int )clear_panel_idx.size( ); k++ ) {
				if ( penta[ j ] == clear_panel_idx[ k ] ) {
					existence = true;
					break;
				}
			}
			if ( !existence ) {
				clear_panel_idx.push_back( penta[ j ] );
			}
		}
	}
	//FadePneles::FADE_PANEL = clear_panel_idx;
	getBoard( )->setPanelState( Panel::STATE_NONE, clear_panel_idx );
}

void SceneSelect::checkIllustEnableSelect( ) {
	if ( _check_count % 100 != 0 ) {
		_check_count++;
		return;
	}
	_check_count++;
	for ( int i = 0; i < MAX_ILLUST_NUM; i++ ) {
		_enable_select[ i ] = true;
	}

	std::vector< std::string > times = _db->selectColumn( TABLE_ONLINE, COLUMN_TIME );
	if ( ( int )times.size( ) < MAX_ILLUST_NUM ) {
		return;
	}
	times.erase( times.begin( ) );
	unsigned long long now_time = _ntp->getTime( );
	for ( int i = 0; i < MAX_ILLUST_NUM; i++ ) {
		char* end = NULL;
		unsigned long long time = strtoull( times[ i ].c_str( ), &end, 0 );
		if ( time == 0 ) {
			continue;
		}
		int diff = ( int )( now_time - time );
		if ( diff < ONLINE_TIME ) {
			//最近の更新がある場合イラスト選択不可
			_enable_select[ i ] = false;
		}
	}
}

void SceneSelect::initSelectIllust( ) {
	_select_illust = 0;
	//選択できないイラストの場合使えるイラストまで選択を変える
	while ( !_enable_select[ _select_illust ] ) {
		_select_illust = ( _select_illust + 1 ) % MAX_ILLUST_NUM;
	}
}
