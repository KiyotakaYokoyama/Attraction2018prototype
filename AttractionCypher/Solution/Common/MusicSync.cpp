#include "MusicSync.h"
#include "Drawer.h"
#include "DataBase.h"
#include "Ntp.h"
#include "Server.h"
#include "Board.h"
#include "Panel.h"

const int OLD_TIME = 400 * 1000;//DB�X�V�m�F�p
const int CALC_DILAY = 1000;//�x��
const int TRANS_INTERVAL   = 30 * 1;//�ʐM�Ԋu


MusicSync::MusicSync( BoardPtr board, MATCHING_TYPE matching_type, int illust ) :
_board( board ),
_next_room_num( 0 ),
_matching_type( matching_type ),
_count( 0 ) {
	_check_matching = true;
	_online = false;
	_ntp = NtpPtr( new Ntp( ) );
	_db = DataBasePtr( new DataBase( ) );

	_battle_data.own_illust = illust;
	_battle_data.room_next = Room( );
	_battle_data.room_next.illust[ PLAYER_1 ] = illust;

	_now_music = MUSIC_0;
	_start_time = GetTickCount( ) + INTERVAL_INFO.total_time;
	_room_now  = Room( );
	_room_now.illust[ PLAYER_1 ] = illust;
	matchingInsert( );
}


MusicSync::~MusicSync( ) {
}

void MusicSync::update( ) {
	if ( _count % TRANS_INTERVAL == 0 ) {
		checkNextRoom( );
	}
	if ( _count % TRANS_INTERVAL == 1 ) {
		syncArea( );
	}
	if ( _count % TRANS_INTERVAL == 2 ) {
		refleshOnline( );
	}
	_count++;
}

void MusicSync::sync( bool maching ) {
	/*�I�t���C�����[�h�̏���
	  �ENTP�T�[�o���玞�Ԃ��擾�ł��Ȃ�
	  �EDB�T�[�o�̍X�V���Â�
	*/

	//�Đ�����
	char* end = NULL;
	std::string time_str = _db->selectField( TABLE_MUSIC, COLUMN_TIME, 1 );
	unsigned long long server_start_time = strtoull( time_str.c_str( ), &end, 0 );

	//�Đ����Ԃ̎��̃J�E���g
	//int count = std::atoi( _db->selectField( TABLE, COLUMN_COUNT ).c_str( ) );
	//�Ƃ肠����count==0�Ɖ���


	_online = true;
	unsigned long long now = _ntp->getTime( );
	//now��0�̏ꍇ�I�t���C��
	if ( now == 0 ) {
		_online = false;
	}
	//�ȊJ�n���Ԃ܂ł̍�
	int diff = ( int )( server_start_time - now );
	//���ԍ����傫��������X���[
	if ( abs( diff ) > OLD_TIME ) {
		_online = false;
	}
	//�ȊJ�n���Ԃ��v�Z����
	if ( _online ) {
		_start_time = GetTickCount( ) + diff;
	}

	//���̋ȃ`�F�b�N
	if ( !_online ) {
		OutputDebugString( "NTP�ڑ��Ɏ��sor�T�[�o�[�̍X�V���Â��ׁA�s�����Ŏ��s\n" );
		_now_music = ( MUSIC )( ( _now_music + 1 ) % MAX_MUSIC );
	} else {
		_now_music = ( MUSIC )std::atoi( _db->selectField( TABLE_MUSIC, COLUMN_MUSIC, 1 ).c_str( ) );
	}


	//�`���[�g���A�����̓}�b�`���O���Ȃ�
	if ( !maching ) {
		_online = false;
	}
}


void MusicSync::matchingInsert( ) {
	if ( !_online ) {
		return;
	}
	_db->add( TABLE_MACHING_NEXT, COLUMN_MACHING, std::to_string( _battle_data.own_illust ) );
}

int MusicSync::getPlayingTime( ) const {
	return GetTickCount( ) - _start_time;
}

MUSIC MusicSync::getNowMusic( ) const {
	return _now_music;
}

void MusicSync::createRoom( int room ) {
	if ( room < 0 ) {
		return;
	}
	std::string room_str = "room" + std::to_string( room );
	//Room���݃`�F�b�N
	std::vector< std::string > panels = _db->selectColumn( TABLE_AREA, room_str );
	if ( ( int )panels.size( ) == 0 ) {
		//Room����
		_db->addColumn( TABLE_AREA, room_str, "int" );
	}
	//Room���|��
	_db->set( TABLE_AREA, room_str, "0", "id>=0" );
}

void MusicSync::syncArea( ) {
	if ( _room_now.idx < 0 ) {
		return;
	}
	std::string room = "room" + std::to_string( _room_now.idx );
	//�p�l��
	std::vector< std::string > panels = _db->selectColumn( TABLE_AREA, room );
	if ( ( int )panels.size( ) == 0 ) {
		return;
	}
	//0�Ԗڂ̓w�b�_�[�Ȃ̂ō폜
	panels.erase( panels.begin( ) );

	//�擾�ł����������Ȃ��ꍇ������
	if ( ( int )panels.size( ) < PENTA_NUM ) {
		return;
	}

	//�p�l����ύX������
	for ( int i = 0; i < PENTA_NUM; i++ ) {
		SCORETYPE type = ( SCORETYPE )std::atoi( panels[ i ].c_str( ) );
		if ( type == SCORETYPE_OFFLINE ) {
			continue;
		}
		Panel::STATE state = ( Panel::STATE )( Panel::STATE::STATE_PLAYER1 + ( type - SCORETYPE_A ) );
		//���C�u���j�^�[�Ŋϐ킷�鎞�p�̃Z�b�g
		if ( _room_now.score_type == SCORETYPE_LIVE ) {
			_board->getPanel( i )->setState( state );
			continue;
		}
		if ( _matching_type == MATCHING_TYPE_BATTLE &&
			 _room_now.score_type == SCORETYPE_B ) {
			if ( state == Panel::STATE::STATE_PLAYER1 ) {
				state = Panel::STATE::STATE_PLAYER2;
			} else {
				state = Panel::STATE::STATE_PLAYER1;
			}
		}
		if ( type != _room_now.score_type ) {
			state = Panel::STATE::STATE_PLAYER2;
		}
		_board->getPanel( i )->setState( state );
	}
}

void MusicSync::setDBPanel( int panel_idx ) {
	if ( _room_now.idx < 0 ) {
		return;
	}
	std::string room = "room" + std::to_string( _room_now.idx );
	_db->set( TABLE_AREA, room, std::to_string( _room_now.score_type ), "id=" + std::to_string( panel_idx ) );
}

Room MusicSync::getRoom( ) const {
	return _room_now;
}

Room MusicSync::getNextRoom( ) const {
	return _battle_data.room_next;
}

void MusicSync::replaceIllustNext( ) {
	//�o�g���ɓ������^�C�~���O
	switch ( _matching_type ) {
	case MATCHING_TYPE_BATTLE:
		_room_now = _battle_data.room_next;
		_battle_data.room_next = Room( );
		_battle_data.room_next.illust[ PLAYER_1 ] = _battle_data.own_illust;
		break;
	case MATCHING_TYPE_LIVE:
		if ( _next_room_num == 0 ) {
			_room_now = Room( );
		} else {
			_room_now = _room_nexts[ GetTickCount( ) % _next_room_num ];
		}
		break;
	}

	for ( int i = 0; i < ROOM_NUM; i++ ) {
		_room_nexts[ i ] = Room( );
	}
	_next_room_num = 0;
}

void MusicSync::initInterval( bool tutorial ) {
	sync( !tutorial );
	if ( getPlayingTime( ) > MUSIC_INFO[ _now_music ].total_time ) {
		_start_time += MUSIC_INFO[ _now_music ].total_time + INTERVAL_INFO.total_time;
		_now_music = ( MUSIC )( _now_music + 1 % MAX_MUSIC );
	}
	_check_matching = false;
}

void MusicSync::initBattle( bool tutorial ) {
	//sync( !tutorial );
	switch ( _matching_type ) {
	case MATCHING_TYPE_BATTLE:
		matchingInsert( );
		//createRoom( _room_now.idx );
		break;
	case MATCHING_TYPE_LIVE:
		break;
	}
	if ( !tutorial ) {
		replaceIllustNext( );
	}
	_check_matching = true;
	_count = 0;
}

void MusicSync::checkNextRoom( ) {
	//battle && �}�b�`���O�ς݂̏ꍇ������
	if ( !_check_matching ) {
		return;
	}
	//�擾
	std::vector< std::string > list = _db->selectColumn( TABLE_MACHING_NEXT, COLUMN_MACHING );
	list.erase( list.begin( ) );            //�w�b�_�[�폜
	int room_num = ( int )list.size( ) / 2;	//���[����
	
	//���[���������Ă��Ȃ��ꍇ������
	if ( _next_room_num >= room_num ) {
		return;
	}
	_next_room_num = room_num;
	//RoomNexts�֓����
	for ( int i = 0; i < room_num; i++ ) {
		_room_nexts[ i ].idx = i;
		_room_nexts[ i ].illust[ 0 ] = std::atoi( list[ i * 2 + 0 ].c_str( ) );
		_room_nexts[ i ].illust[ 1 ] = std::atoi( list[ i * 2 + 1 ].c_str( ) );
		_room_nexts[ i ].score_type = SCORETYPE_A;
	}

	//�}�b�`���O���������T��
	if ( _matching_type == MATCHING_TYPE_BATTLE ) {
		for ( int i = 0; i < room_num; i++ ) {
			if ( _room_nexts[ i ].illust[ 0 ] == _battle_data.own_illust ) {
				_battle_data.room_next = _room_nexts[ i ];
				_battle_data.room_next.score_type = SCORETYPE_A;
				_check_matching = false;
				break;
			}
			if ( _room_nexts[ i ].illust[ 1 ] == _battle_data.own_illust ) {
				_battle_data.room_next = _room_nexts[ i ];
				_battle_data.room_next.illust[ 0 ] = _room_nexts[ i ].illust[ 1 ];
				_battle_data.room_next.illust[ 1 ] = _room_nexts[ i ].illust[ 0 ];
				_battle_data.room_next.score_type = SCORETYPE_B;
				_check_matching = false;
				break;
			}
		}
	}
}

void MusicSync::refleshOnline( ) {
	if ( _battle_data.own_illust < 0 ) {
		return;
	}
	//DB(OlineList)�֎����̃C���X�g�ԍ��փ^�C��������
	unsigned long long now_time = _ntp->getTime( );
	std::string time = std::to_string( now_time );
	std::string condition = COLUMN_ILLUST + "=" + std::to_string( _battle_data.own_illust );
	_db->set( TABLE_ONLINE, COLUMN_TIME, time, condition );

}

int MusicSync::getPlayerIllust( ) const {
	return _battle_data.own_illust;
}

void MusicSync::setPlayerIllust( int illust ) {
	_battle_data.own_illust = illust;
	_battle_data.room_next.illust[ PLAYER_1 ] = illust;
	_room_now.illust[ PLAYER_1 ] = illust;
}

int MusicSync::getNowRoomNum( ) const {
	std::vector< std::string > list = _db->selectColumn( TABLE_MACHING_NOW, COLUMN_MACHING );
	list.erase( list.begin( ) );            //�w�b�_�[�폜
	int room_num = ( int )list.size( ) / 2;	//���[����
	return room_num;
}

int MusicSync::getNextRoomNum( ) const {
	return _next_room_num;
}

void MusicSync::observerRoom( int room_idx ) {
	//���C�u���j�^�[�Ŋϐ킷�镔�����w��
	std::vector< std::string > list = _db->selectColumn( TABLE_MACHING_NOW, COLUMN_MACHING );
	list.erase( list.begin( ) );            //�w�b�_�[�폜
	int room_num = ( int )list.size( ) / 2;	//���[����
	for ( int i = 0; i < room_num; i++ ) {
		if ( i != room_idx ) {
			continue;
		}
		_room_now.idx = room_idx;
		_room_now.illust[ 0 ] = std::atoi( list[ i * 2 + 0 ].c_str( ) );
		_room_now.illust[ 1 ] = std::atoi( list[ i * 2 + 1 ].c_str( ) );
		_room_now.score_type = SCORETYPE_LIVE;
	}
}

