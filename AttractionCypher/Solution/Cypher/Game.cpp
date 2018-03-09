#include "Game.h"
#include "Application.h"
#include "ScenePlay.h"
#include "SceneSelect.h"
#include "SceneWaitScreen.h"
#include "SceneGameOver.h"
#include "Drawer.h"
#include "Board.h"
#include "PentaVertex.h"
#include "Debug.h"
#include "MusicSync.h"

GamePtr Game::getTask( ) {
	return std::dynamic_pointer_cast< Game >( Application::getInstance( )->getTask( getTag( ) ) );
}

Game::Game( ) :
_next( Scene::SCENE_WAIT_SCREEN ) {
}


Game::~Game( ) {
}

void Game::initialize( ) {
	PentaVertexPtr penta_vertex( new PentaVertex );
	_board = BoardPtr( new Board( ) );
	_board->init( penta_vertex );
	_sync = MusicSyncPtr( new MusicSync( _board, MusicSync::MATCHING_TYPE_BATTLE ) );
	changeScene( );
}

void Game::update( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->flip( );
	drawer->waitForSync( );
	_next = _scene->update( );
	changeScene( );
	Debug::getTask( )->draw( );
}

void Game::changeScene( ) {
	if ( _next == Scene::SCENE_CONTINUE ) {
		return;
	}
	
	_scene.reset( );

	switch ( _next ) {
	case Scene::SCENE_WAIT_SCREEN:
		_scene = ScenePtr( new SceneWaitScreen( _board, _sync ) );
		break;
	case Scene::SCENE_SELECT:
		_scene = ScenePtr( new SceneSelect( _board, _sync ) );
		break;
	case Scene::SCENE_PLAY:
		_scene = ScenePtr( new ScenePlay( _board, _sync ) );
		break;
	case Scene::SCENE_GAMEOVER:
		_scene = ScenePtr( new SceneGameOver( _board ) );
		break;
	case Scene::SCENE_CONTINUE_PLAY:
		_scene = ScenePtr( new ScenePlay( _board, _sync, true ) );
		break;
	}
}