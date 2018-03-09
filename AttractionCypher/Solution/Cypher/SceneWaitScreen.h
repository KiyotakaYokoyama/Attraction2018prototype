#pragma once
#include "Scene.h"

PTR( MusicSync );
PTR( Image );

class SceneWaitScreen : public Scene {
public:
	SceneWaitScreen( BoardPtr board, MusicSyncPtr sync );
	virtual ~SceneWaitScreen( );
public:
	void updateBeatEffect( );
private:
	SCENE update( );
	void drawTitle( ) const;
	bool isTouchButton( );
private:
	ImagePtr _title;
	MusicSyncPtr _sync;
	int _before_playing_time;
};

