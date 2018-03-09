#include "Live.h"
#include "Application.h"
#include "BoardViewer.h"
#include "Drawer.h"
#include "TitleViewer.h"
#include "Board.h"
#include "MusicSync.h"
#include "PentaVertex.h"
#include "Music.h"
#include "Debug.h"

PTR( Board );

LivePtr Live::getTask( ) {
	return std::dynamic_pointer_cast< Live >( Application::getInstance( )->getTask( getTag( ) ) );
}


Live::Live( )  {
}

Live::~Live( ) {
}

void Live::initialize( ) {
	BoardPtr board( new Board );
	board->init( PentaVertexPtr( new PentaVertex ) );
	_sync  = MusicSyncPtr( new MusicSync( board, MusicSync::MATCHING_TYPE_LIVE ) );
	_title_viewer = TitleViewerPtr( new TitleViewer( board, _sync ) );
	_board_viewer = BoardViewerPtr( new BoardViewer( board, _sync ) );
	
	_scene = SCENE_TITLE;
	_title_viewer->init( );
}

void Live::update( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->flip( );
	drawer->waitForSync( );

	switch ( _scene ) {
	case SCENE_TITLE:
		_title_viewer->update( );
		_title_viewer->draw( );
		break;
	case SCENE_BOARD:
		_board_viewer->update( );
		_board_viewer->draw( );
		break;
	}
	updateScene( );
	
	if ( Debug::getTask( )->isDebug( ) ) {
		int now = _sync->getPlayingTime( );
		drawer->drawString( 0, 0, "Now:%08d ( %d•b )", now, now / 1000 );
		Room now_room = _sync->getRoom( );
		drawer->drawString( 0, 20, "Room:%d", now_room.idx );
		drawer->drawString( 0, 40, "Illust0:%d", now_room.illust[ 0 ] );
		drawer->drawString( 0, 60, "Illust1:%d", now_room.illust[ 1 ] );
		drawer->drawString( 0, 80, "room_num:%d", _sync->getNowRoomNum( ) );
		drawer->drawString( 0, 100, "Next_room_num:%d", _sync->getNextRoomNum( ) );
	}
}

void Live::updateScene( ) {
	int play_time = _sync->getPlayingTime( );

	if ( play_time > MUSIC_INFO[ _sync->getNowMusic( ) ].total_time ) {
		_sync->initInterval( false );
		play_time = _sync->getPlayerIllust( );
	}

	if ( play_time < 0 && _scene != SCENE_TITLE ) {
		_scene = SCENE_TITLE;
		_title_viewer->init( );
	}

	int room_num = _sync->getNowRoomNum( );
	
	if ( play_time >= 0 &&
		 _scene != SCENE_BOARD &&
		room_num > 0 ) {
		_scene = SCENE_BOARD;
		_board_viewer->init( );
		_sync->observerRoom( rand( ) % room_num );
	}
}
