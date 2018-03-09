#include "Scene.h"
#include "Board.h"
#include "ImageTarget.h"
#include "define.h"
#include "TouchPanel.h"

const int CHENGE_TIME = 60;

Scene::Scene( BoardPtr board ) :
_board( board ) {
	board->initPanelState( );
	_screen = ImageTargetPtr( new ImageTarget );
	_screen->create( SCREEN_WIDTH, SCREEN_WIDTH );// ゲーム画面の縦長に対応するため正方形
	_screen->setImageRotation( SCREEN_WIDTH / 2, SCREEN_HIGHT / 2, SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, PI * 3 / 2 );
}

Scene::~Scene( ) {
}

void Scene::checkTap( ) {
	int size = ( int )TouchPanel::getTask( )->getTouchs( ).size( );
	if ( size >= 10 ) { //10本の指でタッチされいなければ表示はしない
		_change_count++;
	} else {
		_change_count = 0;
	}
}

bool Scene::isReturnTitle( ) const {
	if ( _change_count == CHENGE_TIME ) {
		return true;
	}
	return false;
}

BoardPtr Scene::getBoard( ) const {
	return _board;
}

ImageTargetPtr Scene::getScreen( ) const {
	return _screen;
}
