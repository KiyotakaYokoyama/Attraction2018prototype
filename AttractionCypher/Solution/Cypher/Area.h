#pragma once
#include "smart_ptr.h"
#include "define.h"
#include "ScenePlay.h"
#include <list>
#include <vector>
#include <array>

PTR( Bullet );
PTR( MusicSync );
PTR( Board );
PTR( DataBase );
PTR( Ntp );
PTR( Image );

const int MAX_AREA_IDX = 18;

class Area {
public:
	Area( BoardPtr board, MusicSyncPtr sync, ImagePtr number );
	virtual ~Area( );
public:
	void update( STATE state );
	void draw( ) const;
	void drawJudge( ) const;
	void drawCountDown( STATE state ) const;
	void init( STATE state );
	void setFinished( bool value );
	int getMiss( ) const;
	int getSuccess( ) const;
	void setSuccess( int success );
	bool isFinished( ) const;
	void reset( );
private:
	void updateBullets( );
	void drawBullets( ) const;
	void loadBullet( );
	void createScore( STATE state );
	void judgmentTouchedHexagon( );
	void setjudge( int hex_idx );
	void switchPanel( int hex_idx );
	void initActiveHex( );
	void playCountDown( STATE state );
	void clearBulletByTurn( STATE state );
private:
	bool isOwnTurn( STATE state ) const;
	bool isOverlapedBullet( int hex_idx ) const;
private:
	MusicSyncPtr _sync;
	DataBasePtr _db;
	BoardPtr _board;
	ImagePtr _number;
	NtpPtr _ntp;
	std::list< BulletPtr > _bullets;
	std::vector< std::array< char, BEAT > > _score;
	std::array< int, MAX_AREA_IDX > _buttele_area;
	std::array< std::string, HEXAGON_BUTTON_NUM > _judge;
	bool _init;
	bool _finished;
	bool _clear;
	bool _count_down;
	int _miss;
	int _success;
	int _before_load_time;
	int _before_count_down_time;
};

