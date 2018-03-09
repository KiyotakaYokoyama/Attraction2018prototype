#pragma once
#include <string>
#include "Task.h"
#include "Scene.h"
#include "define.h"

PTR( Game );
PTR( Scene );
PTR( Board );
PTR( MusicSync );

class Game : public Task {
public:
	static std::string getTag( ) { return "GAME"; };
	static GamePtr getTask( );
public:
	Game( );
	virtual ~Game( );
public:
	void initialize( );
	void update( );
private:
	void changeScene( );
private:
	ScenePtr _scene;
	Scene::SCENE _next;
	BoardPtr _board;
	MusicSyncPtr _sync;
};
