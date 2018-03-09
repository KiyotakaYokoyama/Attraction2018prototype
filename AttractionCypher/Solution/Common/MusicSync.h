#pragma once
#include "smart_ptr.h"
#include "Music.h"
#include <array>
#include "define.h"

PTR( Board );
PTR( DataBase );
PTR( Ntp );

class MusicSync {
public:
	enum MATCHING_TYPE {
		MATCHING_TYPE_LIVE,
		MATCHING_TYPE_BATTLE,
	};
public:
	MusicSync( BoardPtr board, MATCHING_TYPE matching_type, int illust = -1 );
	virtual ~MusicSync( );
public:
	void update( );
public:
	void initInterval( bool tutorial );
	void initBattle( bool tutorial );
	void observerRoom( int room_idx );
public:
	int getPlayingTime( ) const;//サーバから取得した再生時間
	int getPlayerIllust( ) const;
	int getNowRoomNum( ) const;
	int getNextRoomNum( ) const;
	Room getRoom( ) const;
	Room getNextRoom( ) const;
	MUSIC getNowMusic( ) const;
public:
	void setDBPanel( int panel_idx );
	void setPlayerIllust( int illust );
private:
	static const int ROOM_NUM = 4;
private:
	struct BattleData {
		Room room_next;
		int own_illust;
	};
private://共通
	void sync( bool maching );
	void checkNextRoom( );      //対戦相手リストを取得
	void syncArea( );           //領域同期
	void createRoom( int room );//ルーム
private://Live
private://Battle
	void replaceIllustNext( );//NextをNowへ
	void matchingInsert( );   //自分を登録
	void refleshOnline( );    //Online情報をＤＢへ
private:
	NtpPtr _ntp;
	BoardPtr _board;
	DataBasePtr _db;
	MUSIC _now_music;
	MATCHING_TYPE _matching_type;
	BattleData _battle_data;
	bool _online;
	int _count;
	int _next_room_num;
	bool _check_matching;//相手決定
	unsigned long _start_time;
	Room _room_now;
	std::array< Room, ROOM_NUM > _room_nexts;
};

