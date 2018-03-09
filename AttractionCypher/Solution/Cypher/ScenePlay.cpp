#include "ScenePlay.h"
#include "Area.h"
#include "Board.h"
#include "Drawer.h"
#include "Image.h"
#include "ImageTarget.h"
#include "define.h"
#include "MusicSync.h"
#include "Panel.h"
#include "TouchPanel.h"
#include "Mouse.h"
#include "Debug.h"

const int MAX_RESULT_COUNT = 100;//ゲームオーバー画面へ遷移するまでの時間
const int MAX_MISS = 30;//ミスの許容数
const int TUTORIAL_BULLET = 30; // チュートリアルでたたく最低限の譜面数
const int TUTORIAL_WAIT_COUNT = 40; //テキストが出てからタッチが出来るまでのカウント
const int TUTORIAL_BUTTON = 114;


ScenePlay::ScenePlay( BoardPtr board, MusicSyncPtr sync, bool continue_play ) :
Scene( board ),
_state( STATE_INTERVAL ),
_result( RESULT_NONE ),
_count( 0 ),
_tutorial( TUTORIAL_TEXT1 ),
_sync( sync ) {
	board->clearButton( );
	board->addButton( TUTORIAL_BUTTON );
	if ( continue_play ) {
		_tutorial = TUTORIAL_END;
		board->clearButton( );
	}
	if ( Debug::getTask( )->isDebug( ) ) {
		_tutorial = TUTORIAL_END;
		board->clearButton( );
	}
	_interval_message = Drawer::getTask( )->createImage( "play/nextgame.png" );
	_meter = Drawer::getTask( )->createImage( "play/gauge.png" );
	_tutorial_text = Drawer::getTask( )->createImage( "play/TutorialText.png" );
	_number = Drawer::getTask( )->createImage( "number.png" );
	initIllustScreen( );

	_area = AreaPtr( new Area( board, sync, _number ) );
	createImages( );
	_sync->initInterval( isTutorial( ) );
}

ScenePlay::~ScenePlay( ) {
}

Scene::SCENE ScenePlay::update( ) {
	//-----update-----//
	bool wait = updateTutotial( );

	STATE before = _state; //シーン遷移判断で使用

	getBoard( )->update( );
	updateState( );
	if ( !wait ) {
		_sync->update( );
		_area->update( _state );
	}
	getBoard( )->updateTouchEffect( );
	checkTap( );
	_count++;
	//---------------//

	draw( );


	if ( !Debug::getTask( )->isDebug( ) ) {
		//----シーン遷移----//
			//チュートリアルモードではゲームオーバーにしない
		if ( _state == STATE_INTERVAL && !isTutorial( ) ) {
			//対戦での敗北でゲームオーバー
			if ( _result == RESULT_LOSE &&
				 _count > MAX_RESULT_COUNT ) {
				return SCENE_GAMEOVER;
			}
			//ソロモードでのミス多でゲームオーバー
			if ( _sync->getRoom( ).score_type == SCORETYPE_OFFLINE &&
				 _area->getSuccess( ) < MAX_MISS &&
				 before == STATE_BUTTEL ) {
				return SCENE_GAMEOVER;
			}
		}
	}

	if ( isReturnTitle( ) ) {
		return SCENE_WAIT_SCREEN;
	}

	return SCENE_CONTINUE;
}

void ScenePlay::updateState( ) {
	switch ( _state ) {
	case STATE_INTERVAL:
		if ( _sync->getPlayingTime( ) > 0 ) {
			if ( isTutorial( ) ) {
				setState( STATE_TUTORIAL );
			} else {
				setState( STATE_BUTTEL );
			}
			_result = RESULT_NONE;
		}
		break;
	case STATE_TUTORIAL:
		if ( _area->isFinished( ) ) {
			setState( STATE_INTERVAL );
		}
		break;
	case STATE_BUTTEL:
		//曲が終わり、インターバルにはいる前の時間
	{
		if ( _sync->getPlayingTime( ) > MUSIC_INFO[ _sync->getNowMusic( ) ].total_time ) {
			setWinnerIllust( );
			setResult( );
			_count = 0;
		}

		if ( _area->isFinished( ) ) {
			setState( STATE_INTERVAL );
		}
		break;
	}
	default:
		break;
	}
}

void ScenePlay::initIllustScreen( ) {
	for ( int i = 0; i < MAX_PLAYER; i++ ) {
		_illust_for_screen[ i ] = ImageTargetPtr( new ImageTarget );
		_illust_for_screen[ i ]->create( SCREEN_WIDTH, SCREEN_WIDTH );
		_illust_for_screen[ i ]->setBlend( Image::BLEND_ADD, 1.0 );
	}
	_illust_draw_screen = ImageTargetPtr( new ImageTarget );
	_illust_draw_screen->create( SCREEN_WIDTH, SCREEN_WIDTH );
}


void ScenePlay::setState( STATE state ) {
	_state = state;
	switch ( state ) {
		case STATE_INTERVAL:
			_area->reset( );
			_sync->initInterval( isTutorial( ) );
			getBoard( )->initPanelState( );
			break;
		case STATE_TUTORIAL:
			break;
		case STATE_BUTTEL:
			_sync->initBattle( isTutorial( ) );
			getBoard( )->initPanelState( );
			break;
		default:
			break;
	}

}

void ScenePlay::draw( ) const {
	createPanelIllusts( ); //LAYER_BACKの代わり

	BoardPtr board = getBoard( );
	DrawerPtr drawer = Drawer::getTask( );
	ImageTargetPtr screen = getScreen( );
	screen->clear( );
	drawer->setImageTarget( screen );
	//---screenのtmpに描画---//
	board->drawBase( );            //ペンタベース
	_illust_draw_screen->draw( ); // 加工されたIllust画像
	board->drawDefault( );  //標準
	drawMessage( );         //メッセージ
	board->drawFrame( );    //フレーム
	board->drawIdx( );      //デバック表示
	board->drawElectric( ); //びりびり
	board->drawButton( );   //ボタン
	_area->draw( );         //譜面
	_area->drawJudge( );    //判定
	
	//---screenへの描画ここまで---//
	drawer->setImageTarget( );
	//---実際のscreenに描画---//
	screen->drawRotation( );
	
	if ( Debug::getTask( )->isDebug( ) ) {
		int now = _sync->getPlayingTime( );
		std::string state = "STATE:";
		if ( _state == STATE_TUTORIAL ) {
			state += "TUTORIAL";
		}
		if ( _state == STATE_INTERVAL ) {
			state += "INTERVAL";
		}
		if ( _state == STATE_BUTTEL ) {
			state += "BUTTEL";
		}
		DrawerPtr debug_drawer = Drawer::getTask( );
		Room room = _sync->getRoom( );
		Room room_next = _sync->getNextRoom( );
		debug_drawer->drawString( 0,   0, state.c_str( ) );
		debug_drawer->drawString( 0,  20, "Now:%08d ( %d秒 )", now, now / 1000 );
		debug_drawer->drawString( 0,  40, "Room:%d", room.idx );
		debug_drawer->drawString( 0,  60, "Illust-Player1:%d", room.illust[ PLAYER_1 ] );
		debug_drawer->drawString( 0,  80, "Illust-Player2:%d", room.illust[ PLAYER_2 ] );
		debug_drawer->drawString( 0, 100, "Next-Room:%d", room_next.idx );
		debug_drawer->drawString( 0, 120, "Next-Illust-Player1:%d", room_next.illust[ PLAYER_1 ] );
		debug_drawer->drawString( 0, 140, "Next-Illust-Player2:%d", room_next.illust[ PLAYER_2 ] );
	}
}


void ScenePlay::createPanelIllusts( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Room room = _sync->getRoom( );
	_illust_draw_screen->clear( );
	for ( int i = 0; i < MAX_PLAYER; i++ ) {
		_illust_for_screen[ i ]->clear( );
		drawer->setImageTarget( _illust_for_screen[ i ] );
		//---描画用イラストを作成---//
		if ( room.illust[ i ] < 0 ) {
			continue;
		}
		_player_illust[ room.illust[ i ] ]->draw( );
		getBoard( )->drawPanelForIllust( ( PLAYER )i );
		//---描画用イラストを作成ここまで---/
		//---スクリーン描画用画像作成---//
		drawer->setImageTarget( _illust_draw_screen );
		_illust_for_screen[ i ]->draw( );
	}
}

void ScenePlay::createImages( ) {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < MAX_ILLUST_NUM; i++ ) {
		std::string player = "player/player" + std::to_string( i + 1 ) + ".jpg";
		_player_illust[ i ] = drawer->createImage( player.c_str( ) );
		int tw = 0;
		int th = 0;
		_player_illust[ i ]->getImageSize( tw, th );
		_player_illust[ i ]->setPos( SCREEN_WIDTH / 2 - tw / 2, SCREEN_WIDTH / 2 - th / 2 );
	}
}

void ScenePlay::setResult( ) {
	if ( _sync->getRoom( ).score_type == SCORETYPE_OFFLINE ) {
		return;
	}
	BoardPtr board = getBoard( );
	int player1_num = 0;
	int player2_num = 0;
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		if ( board->getPanel( i )->getState( ) == Panel::STATE_PLAYER1 ) {
			player1_num++;
			continue;
		}
		if ( board->getPanel( i )->getState( ) == Panel::STATE_PLAYER2 ) {
			player2_num++;
			continue;
		}
	}

	if ( player1_num > player2_num ) {
		_result = RESULT_WIN;
	} else if ( player2_num > player1_num ) {
		_result = RESULT_LOSE;
	} else {
		_result = RESULT_DRAW;
	}
}

void ScenePlay::drawResult( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	switch ( _result ) {
		case RESULT_WIN:
			drawer->drawString( SCREEN_WIDTH / 2, SCREEN_HIGHT / 2 - 10, "YOU WIN" );
			break;	
		case RESULT_LOSE:
			drawer->drawString( SCREEN_WIDTH / 2, SCREEN_HIGHT / 2 - 10, "YOU LOSE" );
			break;
		case RESULT_DRAW:
			drawer->drawString( SCREEN_WIDTH / 2, SCREEN_HIGHT / 2 - 10, "DRAW GAME" );
			break;
		case RESULT_NONE:
			break;
	}
}

void ScenePlay::setWinnerIllust( ) {
	if ( _sync->getRoom( ).score_type == SCORETYPE_OFFLINE ) {
		return;
	}

	//winnerを設定
	BoardPtr board = getBoard( );
	int player1_num = 0;
	int player2_num = 0;
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		if ( board->getPanel( i )->getState( ) == Panel::STATE_PLAYER1 ) {
			player1_num++;
			continue;
		}
		if ( board->getPanel( i )->getState( ) == Panel::STATE_PLAYER2 ) {
			player2_num++;
			continue;
		}
	}

	Panel::STATE winner = Panel::STATE_DEFAULT;

	if ( player1_num > player2_num ) {
		winner = Panel::STATE_PLAYER1;
	} 
	
	if ( player1_num < player2_num ) {
		winner = Panel::STATE_PLAYER2;
	}

	if ( winner != Panel::STATE_DEFAULT ) {
		board->setWinnerIllust( winner );
	}
}

void ScenePlay::drawNextPlayMode( ) const {
	if ( _state != STATE_INTERVAL ) {
		return;
	}

	const int TEX_SIZE = 512;

	//next game
	_interval_message->setRect( 0, 0, TEX_SIZE, TEX_SIZE / 4 );
	_interval_message->setPos( SCREEN_WIDTH / 2 - TEX_SIZE / 2, SCREEN_WIDTH / 2 - TEX_SIZE / 4 );
	_interval_message->draw( );

	//solo or battle
	int type = _sync->getNextRoom( ).idx;
	if ( type == -1 ) {
		_interval_message->setRect( 0, TEX_SIZE / 4, TEX_SIZE, TEX_SIZE / 4 );
	} else {
		_interval_message->setRect( 0, TEX_SIZE / 2, TEX_SIZE, TEX_SIZE * 3 / 4 );
	}
	_interval_message->setPos( SCREEN_WIDTH / 2 - TEX_SIZE / 2, SCREEN_WIDTH / 2 );
	_interval_message->draw( );
}

void ScenePlay::drawSoloGauge( ) const {
	const int TEXTURE_SIZE = 128;

	//ゲージ
	double ratio = ( double )_area->getSuccess( ) / MAX_MISS;
	if ( ratio > 1 ) ratio = 1;
	_meter->setRect( 0, TEXTURE_SIZE / 2, TEXTURE_SIZE, TEXTURE_SIZE / 2 );
	int sx = SCREEN_WIDTH / 2;
	int sy = 150;
	_meter->setPos( sx, sy, sx + ( int )( TEXTURE_SIZE * ratio ), sy + TEXTURE_SIZE / 2 );
	_meter->draw( );

	//枠
	_meter->setRect( 0, 0, TEXTURE_SIZE, TEXTURE_SIZE / 2 );
	_meter->setPos( sx, sy );
	_meter->draw( );
}

void ScenePlay::drawTutorialGauge( ) const {
	const int TEXTURE_SIZE = 128;

	//ゲージ
	double ratio = ( double )_area->getSuccess( ) / TUTORIAL_BULLET;
	if ( ratio > 1 ) ratio = 1;
	_meter->setRect( 0, TEXTURE_SIZE / 2, TEXTURE_SIZE, TEXTURE_SIZE / 2 );
	int sx = SCREEN_WIDTH / 2;
	int sy = 150;
	_meter->setPos( sx, sy, sx + ( int )( TEXTURE_SIZE * ratio ), sy + TEXTURE_SIZE / 2 );
	_meter->draw( );

	//枠
	_meter->setRect( 0, 0, TEXTURE_SIZE, TEXTURE_SIZE / 2 );
	_meter->setPos( sx, sy );
	_meter->draw( );
}


void ScenePlay::drawTutorialText1( ) const {
	//説明テキスト
	_tutorial_text->setRect( 0, 0, 1024, 512 );
	int x = SCREEN_WIDTH / 2 - 512;
	int y = SCREEN_HIGHT / 2 - 256;
	_tutorial_text->setPos( x, y, x + 1024, y + 512 ); 
	_tutorial_text->draw( );
}

void ScenePlay::drawTutorialText2( ) const {
	//対戦モードについての説明
	_tutorial_text->setRect( 0, 512, 1024, 512 );
	int x = SCREEN_WIDTH / 2 - 512;
	int y = SCREEN_HIGHT / 2 - 256;
	_tutorial_text->setPos( x, y, x + 1024, y + 512 ); 
	_tutorial_text->draw( );
}

bool ScenePlay::isTutorial( ) const {
	return _tutorial != TUTORIAL_END;
}

bool ScenePlay::isTouch( ) const {
	//クリック判定
	BoardPtr board = getBoard( );
	if ( board->isTouchHexagon( TUTORIAL_BUTTON ) ) {
		return true;
	}
	return false;
}

bool ScenePlay::updateTutotial( ) {
	bool wait = false;
	switch ( _tutorial ) {
	case TUTORIAL_TEXT1:
		wait = true;
		if ( isTouch( ) && _count > TUTORIAL_WAIT_COUNT ) {
			_tutorial = TUTORIAL_PLAY;
			getBoard( )->clearButton( );
		}
		break;
	case TUTORIAL_PLAY:
		//死なないチュートリアルプレイ
		//一定数叩くとTEXT2へ
		if ( _area->getSuccess( ) > TUTORIAL_BULLET ) {
			getBoard( )->addButton( TUTORIAL_BUTTON );
			_area->reset( );
			_tutorial = TUTORIAL_TEXT2;
			_count = 0;
		}
		break;
	case TUTORIAL_TEXT2:
		wait = true;
		if ( isTouch( ) && _count > TUTORIAL_WAIT_COUNT ) {
			_tutorial = TUTORIAL_END;
			if ( _state != STATE_INTERVAL ) {
				getBoard( )->clearButton( );
				_area->setSuccess( MAX_MISS );
				_sync->initBattle( true );
			}
		}
		break;
	case TUTORIAL_END:
		//通常プレイ
		break;
	}
	return wait;
}

void ScenePlay::drawMessage( ) const {
	if ( _sync->getRoom( ).score_type == SCORETYPE_OFFLINE ) {
		drawSoloGauge( );
	}

	if ( isTutorial( ) ) {
		drawTutorialGauge( );
	} else {
		if ( _state == STATE_INTERVAL ) {
			drawNextPlayMode( );
		}
	}
	if ( _tutorial == TUTORIAL_TEXT1 ) {
		drawTutorialText1( );
	}
	if ( _tutorial == TUTORIAL_TEXT2 ) {
		drawTutorialText2( );
	}
	drawResult( );
	_area->drawCountDown( _state );
}