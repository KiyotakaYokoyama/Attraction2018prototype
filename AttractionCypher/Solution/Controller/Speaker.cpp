#include "Speaker.h"
#include "Sound.h"
#include "Ntp.h"
#include "DataBase.h"
#include "Drawer.h"
#include "define.h"

#define ONLINE 1

Speaker::Speaker( ) :
_next_music( MUSIC_0 ) {
	_bgm = true;
	_ntp = NtpPtr( new Ntp( ) );
	_db = DataBasePtr( new DataBase( ) );

#if ONLINE
	_db->erase( TABLE_AREA, "id", "id>=0" );
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		_db->add( TABLE_AREA, "id", std::to_string( i ) );
	}
#endif
}

Speaker::~Speaker( ) {
}

void Speaker::update( ) {
	SoundPtr sound = Sound::getTask( );
	if ( !sound->isPlayingBGM( ) ) {
		playNextMusic( );
		if ( !isBGM( ) ) {
			//インターバル開始時
			sendInfo( );
			replaceNowMatching( );
			_replace = false;
		} else {
			//バトル開始時
			clearRoom( );
		}
	}
	int time = GetTickCount( ) - _send_time - INTERVAL_INFO.total_time;
	if ( MUSIC_INFO[ getNowMusic( ) ].total_time < time ) {
		//現在の再生時間→次の再生時間( now_time - total_time - interval_time )
		time += - INTERVAL_INFO.total_time - MUSIC_INFO[ getNowMusic( ) ].total_time;
	}
	Drawer::getTask( )->drawString( 0, 0, "Time:%06d(秒:%d)", time, time / 1000 );
	Drawer::getTask( )->drawString( 0, 20, "Title:%s", MUSIC_INFO[ getNowMusic( ) ].filename.c_str( ) );
}

bool Speaker::isBGM( ) const {
	return _bgm;
}

void Speaker::sendInfo( ) {
	//時間送信
#if ONLINE
	unsigned long long now = 0;

	while ( now == 0 ) {
		now = _ntp->getTime( );
	}
	_send_time = GetTickCount( );
	//バトル開始時間=インターバル開始時間+インターバル時間
	unsigned long long battle_start_time = now + INTERVAL_INFO.total_time;
	_db->set( TABLE_MUSIC, COLUMN_TIME, std::to_string( battle_start_time ) );
	_db->set( TABLE_MUSIC, COLUMN_MUSIC, std::to_string( _next_music ) );
	OutputDebugString( "曲情報送信\n" );
#endif
}

void Speaker::playNextMusic( ) {
	_bgm = !_bgm;
	if ( _bgm ) {
		//バトル曲
		Sound::getTask( )->playBGM( MUSIC_INFO[ _next_music ].filename, false );
		_next_music = ( MUSIC )( ( _next_music + 1 ) % MAX_MUSIC );
	} else {
		//インターバル曲
		Sound::getTask( )->playBGM( INTERVAL_INFO.filename, false );
	}
}

void Speaker::replaceNowMatching( ) {
#if ONLINE
	_replace = true;
	//Nowを削除
	_db->erase( TABLE_MACHING_NOW, COLUMN_MACHING, COLUMN_MACHING + ">=0" );
	//NowにNextを入れる
	_db->copy( TABLE_MACHING_NOW, TABLE_MACHING_NEXT, COLUMN_MACHING, COLUMN_MACHING );
	//Nextを削除
	_db->erase( TABLE_MACHING_NEXT, COLUMN_MACHING, COLUMN_MACHING + ">=0" );
#endif
}

MUSIC Speaker::getNowMusic( ) const {
	return ( MUSIC )( ( _next_music - 1 + MAX_MUSIC ) % MAX_MUSIC );
}

void Speaker::clearRoom( ) {
#if ONLINE
	for ( int i = 0; i < 5; i++ ) {
		std::string room = "room" + std::to_string( i );
		_db->set( TABLE_AREA, room, "0", room + ">0" );
	}
#endif
}
