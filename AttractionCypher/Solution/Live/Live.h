#pragma once
#include "Task.h"
#include <string>

PTR( Live );
PTR( BoardViewer );
PTR( TitleViewer );
PTR( MusicSync );

class Live : public Task {
public:
	static std::string getTag( ) { return "LIVE"; }
	static LivePtr getTask( );
public:
	Live( );
	virtual ~Live( );
private:
	enum SCENE {
		SCENE_TITLE,
		SCENE_BOARD,
	};
private:
	void initialize( );
	void update( );
	void updateScene( );
private:
	SCENE _scene;
	MusicSyncPtr _sync;
	TitleViewerPtr _title_viewer;
	BoardViewerPtr _board_viewer;
};

