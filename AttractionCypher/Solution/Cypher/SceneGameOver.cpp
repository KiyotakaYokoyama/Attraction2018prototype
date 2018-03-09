#include "SceneGameOver.h"
#include "Drawer.h"
#include "define.h"
#include "Board.h"
#include "Panel.h"
#include "Image.h"
#include "ImageTarget.h"

//遷移
const int MAX_WAIT_COUNT = 400;
//Panel
const int HEX_IDX_CONTINUE = 105;
const int CONTINUE_PANEL_NUM = 4;

SceneGameOver::SceneGameOver( BoardPtr board ) :
Scene( board ),
_count( 0 ) {
	_image = Drawer::getTask( )->createImage( "GameOver/GameOver.png" );
	board->clearButton( );
	board->addButton( HEX_IDX_CONTINUE );
	checkClearPanels( );
	checkContinuePos( );
}


SceneGameOver::~SceneGameOver( ) {
}

Scene::SCENE SceneGameOver::update( ) {
	getBoard( )->update( );
	getBoard( )->updateTouchEffect( );
	draw( );


	//シーン遷移
	_count++;
	if ( _count > MAX_WAIT_COUNT ) {
		return SCENE_WAIT_SCREEN;
	}
	if ( isTouchContinue( ) ) {
		return SCENE_CONTINUE_PLAY;
	}
	return SCENE_CONTINUE;
}


void SceneGameOver::draw( ) const {
	ImageTargetPtr screen = getScreen( );
	BoardPtr board = getBoard( );
	screen->clear( );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setImageTarget( screen );
	{//Panel
		board->drawBase( );       //ペンタベース
		board->drawDefault( );
		board->drawFrame( );
		board->drawIdx( );       //デバック表示
	}
	//タッチびりびり
	board->drawElectric( );
	board->drawButton( );
	{//GameOver
		int width = 1024;
		int height = 256;
		int x = SCREEN_WIDTH / 2 - width / 2;
		int y = SCREEN_HIGHT / 2 - height / 2;
		_image->setRect( 0, 0, width, height );
		_image->setPos( x, y );
		_image->draw( );
	}
	{//Continue
		int sprite_width = 1024;
		int sprite_height = 256;
		int draw_width = 256;
		int draw_height = 64;
		int x = ( int )_continue_pos.x - draw_width / 2;
		int y = ( int )_continue_pos.y;
		_image->setRect( 0, 256, sprite_width, sprite_height );
		_image->setPos( x, y, x + draw_width, y + draw_height );
		_image->draw( );
	}
	drawer->setImageTarget( );
	screen->drawRotation( );
}

void SceneGameOver::checkClearPanels( ) {
	//非表示パネル
	std::array< int, HEXAGON_IN_PANEL_NUM > panels = getBoard( )->getHexInPentaIdx( HEX_IDX_CONTINUE );
	_clear_panels.assign( &panels[ 0 ], &panels[ 0 ] + HEXAGON_IN_PANEL_NUM );
	getBoard( )->setPanelState( Panel::STATE_NONE, _clear_panels );
}

void SceneGameOver::checkContinuePos( ) {
	BoardPtr board = getBoard( );
	_continue_pos = Vector( );
	std::array< int, HEXAGON_IN_PANEL_NUM > panels = board->getHexInPentaIdx( HEX_IDX_CONTINUE );
	for ( int i = 0; i < HEXAGON_IN_PANEL_NUM; i++ ) {
		Vector pos = board->getPanel( panels[ i ] )->getPos( );
		_continue_pos += pos;
	}
	_continue_pos += Vector( PANEL_SPRITE_SIZE * 0.5, PANEL_SPRITE_SIZE * 0.5 ) * HEXAGON_IN_PANEL_NUM;
	_continue_pos *= ( 1.0 / HEXAGON_IN_PANEL_NUM );
}


bool SceneGameOver::isTouchContinue( ) const {
	if ( getBoard( )->isTouchHexagon( HEX_IDX_CONTINUE ) ) {
		return true;
	}
	return false;
}
