#pragma once
#include <string>
#include <array>
#include <Music.h>
#include "smart_ptr.h"

PTR( Ntp );
PTR( DataBase );

class Speaker {
public:
	Speaker( );
	virtual ~Speaker( );
public:
	void update( );
	bool isBGM( ) const;
private:
	void sendInfo( );
	void playNextMusic( );
	void replaceNowMatching( );
	void clearRoom( );
	MUSIC getNowMusic( ) const;
private:
	MUSIC _next_music;
	NtpPtr _ntp;
	DataBasePtr _db;
	unsigned int _send_time;
	bool _replace;
	bool _bgm;
};