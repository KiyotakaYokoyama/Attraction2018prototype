#pragma once
#include "smart_ptr.h"

PTR( Image );
PTR( ImageTarget );
PTR( Board );
PTR( MusicSync );

class TitleViewer {
public:
	TitleViewer( BoardPtr board, MusicSyncPtr sync );
	virtual ~TitleViewer( );
public:
	void init( );
	void update( );
	void draw( ) const;
private:
	void initScreen( );
	void updateBeatEffect( );
private:
	int _before_playing_time;
	BoardPtr _board;
	ImagePtr _title;
	MusicSyncPtr _sync;
	ImageTargetPtr _screen;
};
