#pragma once
#include "smart_ptr.h"
#include "define.h"
#include "Music.h"

PTR( Image );
PTR( ImageTarget );
PTR( Board );
PTR( MusicSync );

class BoardViewer {
public:
	BoardViewer( BoardPtr board, MusicSyncPtr sync );
	virtual ~BoardViewer( );
public:
	void init( );
	void update( );
	void draw( ) const;
private:
	void loadIllusts( );
	void initScreen( );
	void initIllustScreen( );
	void setPanelState( );
	void drawPanelIllusts( ) const;
	void drawOccupancy( ) const;
private:
	BoardPtr _board;
	MusicSyncPtr _sync;
	ImageTargetPtr _screen;
	ImageTargetPtr _illust_for_screen[ MAX_PLAYER ];
	ImageTargetPtr _illust_draw_screen;
	ImagePtr _illust[ MAX_ILLUST_NUM ];
	ImagePtr _number;
	int _count;
	STATE _state;
};