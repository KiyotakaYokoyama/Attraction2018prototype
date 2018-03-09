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
			//�C���^�[�o���J�n��
			sendInfo( );
			replaceNowMatching( );
			_replace = false;
		} else {
			//�o�g���J�n��
			clearRoom( );
		}
	}
	int time = GetTickCount( ) - _send_time - INTERVAL_INFO.total_time;
	if ( MUSIC_INFO[ getNowMusic( ) ].total_time < time ) {
		//���݂̍Đ����ԁ����̍Đ�����( now_time - total_time - interval_time )
		time += - INTERVAL_INFO.total_time - MUSIC_INFO[ getNowMusic( ) ].total_time;
	}
	Drawer::getTask( )->drawString( 0, 0, "Time:%06d(�b:%d)", time, time / 1000 );
	Drawer::getTask( )->drawString( 0, 20, "Title:%s", MUSIC_INFO[ getNowMusic( ) ].filename.c_str( ) );
}

bool Speaker::isBGM( ) const {
	return _bgm;
}

void Speaker::sendInfo( ) {
	//���ԑ��M
#if ONLINE
	unsigned long long now = 0;

	while ( now == 0 ) {
		now = _ntp->getTime( );
	}
	_send_time = GetTickCount( );
	//�o�g���J�n����=�C���^�[�o���J�n����+�C���^�[�o������
	unsigned long long battle_start_time = now + INTERVAL_INFO.total_time;
	_db->set( TABLE_MUSIC, COLUMN_TIME, std::to_string( battle_start_time ) );
	_db->set( TABLE_MUSIC, COLUMN_MUSIC, std::to_string( _next_music ) );
	OutputDebugString( "�ȏ�񑗐M\n" );
#endif
}

void Speaker::playNextMusic( ) {
	_bgm = !_bgm;
	if ( _bgm ) {
		//�o�g����
		Sound::getTask( )->playBGM( MUSIC_INFO[ _next_music ].filename, false );
		_next_music = ( MUSIC )( ( _next_music + 1 ) % MAX_MUSIC );
	} else {
		//�C���^�[�o����
		Sound::getTask( )->playBGM( INTERVAL_INFO.filename, false );
	}
}

void Speaker::replaceNowMatching( ) {
#if ONLINE
	_replace = true;
	//Now���폜
	_db->erase( TABLE_MACHING_NOW, COLUMN_MACHING, COLUMN_MACHING + ">=0" );
	//Now��Next������
	_db->copy( TABLE_MACHING_NOW, TABLE_MACHING_NEXT, COLUMN_MACHING, COLUMN_MACHING );
	//Next���폜
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
