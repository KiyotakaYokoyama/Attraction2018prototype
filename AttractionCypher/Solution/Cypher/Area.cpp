#include "Area.h"
#include "Drawer.h"
#include "Bullet.h"
#include "MusicSync.h"
#include "Board.h"
#include "DataBase.h"
#include "Ntp.h"
#include "Image.h"
#include "Panel.h"
#include "Hexagon.h"
#include "Debug.h"
#include "Sound.h"

PTR( Music );

const int FINISH_WAIT_TIME = 1000 * 1;//1秒待機
const int COUNT_DOWN_TIME = 3000;
const int CHANGE_MEASURE = 2;

const int SE_COUNT_DOWN_NUM = 3;
const char* SE_COUNT_DOWN_NAME[ SE_COUNT_DOWN_NUM ] = {
	"CountDown/nc43181.wav",
	"CountDown/nc43182.wav",
	"CountDown/nc43183.wav"
};

const std::array< int, MAX_AREA_IDX > AREA_IDX = {
	//左向き
	53, 54, 55,
	70, 71, 72,
	87, 88, 89,
	//右向き
	62, 63, 64,
	79, 80, 81,
	96, 97, 98
};

Area::Area( BoardPtr board, MusicSyncPtr sync, ImagePtr number ) :
_board( board ),
_sync( sync ),
_finished( false ),
_init( false ),
_before_load_time( 0 ),
_miss( 0 ),
_success( 0 ),
_number( number ) {
	for ( int i = 0; i < MAX_AREA_IDX; i ++ ) {
		_buttele_area[ i ] = -1;//-1のとき灰色
	}
	//initActiveHex( );
}

Area::~Area( ) {
}

void Area::update( STATE state ) {
	init( state );
	loadBullet( );
	switch ( state ) {
	case STATE_INTERVAL:
		break;
	case STATE_TUTORIAL:
	case STATE_BUTTEL:
		updateBullets( );
		judgmentTouchedHexagon( );
		break;
	}
	playCountDown( state );
	clearBulletByTurn( state );
}

void Area::draw( ) const {
	drawBullets( );
}

void Area::updateBullets( ) {
	int time = _sync->getPlayingTime( );
	std::list< BulletPtr >::iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		BulletPtr bullet = *ite;
		bullet->update( time );
		if ( bullet->isFinished( ) ) {
			bullet->setActiveHex( false );
			ite = _bullets.erase( ite );
			_miss++;
			continue;
		}
		ite++;
	}
}

void Area::drawBullets( ) const {
	for ( BulletPtr bullet : _bullets ) {
		bullet->draw( );
	}
}

void Area::drawCountDown( STATE state ) const {
	if ( isOwnTurn( state ) ) {
		return;
	}
	int now = _sync->getPlayingTime( );
	int measure_length = MusicPtr( )->getMeasureLength( MUSIC_INFO[ _sync->getNowMusic( ) ].bpm );
	int turn_time = measure_length * CHANGE_MEASURE;
	//カウントダウンできる長さかどうか
	if ( turn_time < COUNT_DOWN_TIME ) {
		return;
	}

	//残り時間
	int remaing_time = turn_time - ( now % turn_time );

	if ( remaing_time < COUNT_DOWN_TIME ) {
		//数字を描画
		int num = ( remaing_time + 999 ) / 1000;
		int size = 200;
		int sx1 = SCREEN_WIDTH / 2 - size / 2;
		int sy1 = SCREEN_WIDTH / 2 - size / 2;
		int sx2 = sx1 + size;
		int sy2 = sy1 + size;
		_number->setPos( sx1, sy1, sx2, sy2 );
		_number->setRect( 128 * num, 0,128, 128 );
		_number->draw( );
	}
}


void Area::loadBullet( ) {
	int now = _sync->getPlayingTime( );
	MusicInfo info = MUSIC_INFO[ _sync->getNowMusic( ) ];
	int measure_time = MusicPtr( )->getMeasureLength( info.bpm );//小節時間
	int load_range = measure_time * 2 / BEAT;
	if ( now < -load_range ) {
		return;
	}

	if ( now > info.total_time + FINISH_WAIT_TIME ) {
		//曲が終了
		_finished = true;
		return;
	}

	//ロードする範囲までの時間
	int load_time = now + load_range;

	//前回のロードした場所
	int beat_length = MusicPtr( )->getBeatLength( info.bpm );
	int before_score_idx = _before_load_time / beat_length;

	//初回のみ_before_load_time == 0になる
	bool load = false;
	if ( _before_load_time == 0 ) {
		//初回時ロード
		load = true;
	}
	//前回と違う位置に来たらロード
	int load_score_idx = load_time / beat_length;
	if ( before_score_idx != load_score_idx ) {
		load = true;
	}
	
	//ロードしない場合は抜ける
	if ( !load ) {
		return;
	}

	//ロードできるかどうか
	int load_measure = load_score_idx / BEAT;
	if ( load_measure >= ( int )_score.size( ) - 1 ) {
		return;
	}
	//ロード
	int load_num = load_score_idx - before_score_idx;
	for ( int i = 0; i < load_num; i++ ) {
		int load_idx = before_score_idx + i + 1;
		int beat    = load_idx % BEAT;
		int measure = load_idx / BEAT;
		int score_idx = _score[ measure ][ beat ];
		//譜面がアクティブかどうか
		if ( score_idx < 0 ) {
			continue;
		}
		int hex_idx = AREA_IDX[ score_idx ];
		if ( isOverlapedBullet( hex_idx ) ) {
			continue;
		}
		int time = measure * measure_time + beat * beat_length;
		if ( time < now ) {
			//現在時間より前の譜面はださない
			continue;
		}
		_bullets.push_back( BulletPtr( new Bullet( _board, hex_idx, time ) ) );
	}

	_before_load_time = load_time;
}

void Area::createScore( STATE state ) {
	_score = { };
	std::vector< std::array< char, BEAT > > score = MUSIC_SCORE[ _sync->getNowMusic( ) ];
	SCORETYPE type = SCORETYPE_OFFLINE;
	if ( state == STATE_INTERVAL ) {
		//インターバルの場合ネクスト
		type = _sync->getNextRoom( ).score_type;
	} else {
		type = _sync->getRoom( ).score_type;
	}
	int measure_num = ( int )score.size( );
	
	//譜面作成
	for ( int i = 0; i < measure_num; i++ ) {
		bool skip = false;
		switch ( type ) {
		case SCORETYPE_OFFLINE:
			break;
		case SCORETYPE_A:
			if ( i % ( CHANGE_MEASURE * 2 ) < CHANGE_MEASURE ) {
				skip = true;
			}
			break;
		case SCORETYPE_B:
			if ( i % ( CHANGE_MEASURE * 2 ) >= CHANGE_MEASURE ) {
				skip = true;
			}
			break;
		}
		if ( skip ) {
			_score.push_back( SKIP_MEASURE );
		} else {
			_score.push_back( score[ i ] );
		}
	}
}

void Area::judgmentTouchedHexagon( ) {
	for ( int i = 0; i < HEXAGON_BUTTON_NUM; i++ ) {
		_judge[ i ] = "";
		if ( _board->isTouchHexagon( i ) ) {
			setjudge( i );
			switchPanel( i );
		}
	}
}

void Area::setjudge( int hex_idx ) {
	int time = _sync->getPlayingTime( );
	for ( BulletPtr bullet : _bullets ) {
		if ( hex_idx == bullet->getIdx( ) && bullet->canTouch( time ) ) {
			//譜面の削除とGOODのセット
			bullet->setFinished( );
			_judge[ hex_idx ] = "GOOD";
			
			//タッチできた譜面のカウント
			_success++;
			return;
		}
	}
	//BADのセット
	_judge[ hex_idx ] = "BAD";
}

void Area::drawJudge( ) const {
	if ( Debug::getTask( )->isDebug( ) ) {
		DrawerPtr drawer = Drawer::getTask( );
		for ( int i = 0; i < HEXAGON_BUTTON_NUM; i++ ) {
			bool right = i % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) < HEXAGON_RIGHT_WIDTH_NUM;
			int line = ( i % ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM ) ) - ( right ? 0 : HEXAGON_RIGHT_WIDTH_NUM );
			int row = i / ( HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM );
			Vector adjust = Vector( line * PANEL_SPRITE_SIZE, row * PANEL_SPRITE_SIZE );
			if ( right ) {
				adjust += Vector( PANEL_SPRITE_SIZE / 2, 0 );
			} else {
				adjust += Vector( 0, PANEL_SPRITE_SIZE / 2 );
			}
			Vector pos = BOARD_DRAW_POS + adjust;
			int sx = ( int )( pos.x + 0.5 );
			int sy = ( int )( pos.y + 0.5 ) + 20;
			drawer->drawString( sx, sy + 20, "%s", _judge[ i ].c_str( ) );
		}
	}
}

void Area::switchPanel( int hex_idx ) {
	if ( _judge[ hex_idx ] == "GOOD" ) {
		std::array< int, 4 > panels = _board->getHexInPentaIdx( hex_idx );
		for ( int i = 0; i < 4; i++ ) {
			_sync->setDBPanel( panels[ i ] );
			_board->getPanel( panels[ i ] )->setState( Panel::STATE::STATE_PLAYER1 );
		}
	}
}

bool Area::isFinished( ) const {
	return _finished;
}

void Area::setFinished( bool value ) {
	_finished = value;
}

void Area::reset( ) {
	_init = false;
	setFinished( false );
	_score = { };
	_bullets.clear( );
}

void Area::init( STATE state ) {
	if ( state == STATE_INTERVAL ) {
		_init = false;
		return;
	}
	if ( _init ) {
		return;
	}
	_init = true;
	createScore( state );
	_miss = 0;
	_success = 0;
}

int Area::getMiss( ) const {
	return _miss;
}

int Area::getSuccess( ) const {
	return _success;
}

void Area::setSuccess( int success ) {
	_success = success;
}

bool Area::isOverlapedBullet( int hex_idx ) const {
	//出したい６角形内の５角形
	std::array< int, HEXAGON_IN_PANEL_NUM > penta_idx1 = _board->getHexagon( hex_idx )->getHexInPentaIdx( );
	//現在出ている譜面からチェック
	int bullet_num = ( int )_bullets.size( );
	for ( BulletConstPtr bullet : _bullets ) {
		std::array< int, HEXAGON_IN_PANEL_NUM > penta_idx2 = _board->getHexagon( bullet->getIdx( ) )->getHexInPentaIdx( );
		for ( int i = 0; i < HEXAGON_IN_PANEL_NUM; i++ ) {
			for ( int j = 0; j < HEXAGON_IN_PANEL_NUM; j++ ) {
				if ( penta_idx1[ i ] == penta_idx2[ j ] ) {
					return true;
				}
			}
		}
	}
	return false;
}

void Area::initActiveHex( ) {
	for ( int i = 0; i < MAX_AREA_IDX; i++ ) {
		_board->setActiveHexagon( AREA_IDX[ i ], true );
	}
}

void Area::playCountDown( STATE state ) {
	if ( isOwnTurn( state ) ) {
		_count_down = false;
		return;
	}
	if ( _count_down ) {
		return;
	}
	int now = _sync->getPlayingTime( );
	int measure_length = MusicPtr( )->getMeasureLength( MUSIC_INFO[ _sync->getNowMusic( ) ].bpm );
	int turn_time = measure_length * CHANGE_MEASURE;
	//カウントダウンできる長さかどうか
	if ( turn_time < COUNT_DOWN_TIME ) {
		return;
	}

	//残り時間
	int remaing_time = turn_time - ( now % turn_time );

	if ( remaing_time < COUNT_DOWN_TIME ) {
		//再生
		srand( SE_COUNT_DOWN_NUM );
		int se = rand( ) % SE_COUNT_DOWN_NUM;
		Sound::getTask( )->playSE( SE_COUNT_DOWN_NAME[ se ] );
		_count_down = true;
	}
}


bool Area::isOwnTurn( STATE state ) const {
	//type
	SCORETYPE type = SCORETYPE_OFFLINE;
	if ( state == STATE_INTERVAL ) {
		type = _sync->getNextRoom( ).score_type;
	} else {
		type = _sync->getRoom( ).score_type;
	}
	//対戦相手がいない場合は自分のターン
	if ( type == SCORETYPE_OFFLINE ) {
		return true;
	}
	//インターバルの場合false
	int now = _sync->getPlayingTime( );
	if ( now < 0 ) {
		return false;
	}

	int measure_length = MusicPtr( )->getMeasureLength( MUSIC_INFO[ _sync->getNowMusic( ) ].bpm );
	int count = now / measure_length;
	int own = type - SCORETYPE_A;
	if ( type == SCORETYPE_A ) {
		if ( count % ( CHANGE_MEASURE * 2 ) < CHANGE_MEASURE ) {
			return false;
		}
	} else {
		if ( count % ( CHANGE_MEASURE * 2 ) >= CHANGE_MEASURE ) {
			return false;
		}
	}
	return true;
}

void Area::clearBulletByTurn( STATE state ) {
	//削除済みかどうか
	bool own_turn = isOwnTurn( state );
	if ( _clear ) {
		if ( own_turn ) {
			_clear = false;
		} else {
			return;
		}
	}
	//自分のターンでなければ削除
	if ( !own_turn ) {
		_bullets.clear( );
	}
}
