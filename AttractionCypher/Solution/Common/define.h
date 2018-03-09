#pragma once
#include "mathmatics.h"

//画面サイズ
const int SCREEN_WIDTH = 1920;
const int SCREEN_HIGHT = 1080;

//-------------パネル系統-------------//
const int PANEL_SPRITE_SIZE = 256;
const int HEX_WIDTH  = 256;
const int HEX_HEIGHT = 256;
const int HEX_WIDTH_NUM  = SCREEN_WIDTH / HEX_WIDTH + 2;
const int HEX_HEIGHT_NUM = SCREEN_WIDTH / HEX_WIDTH + 2;
const int HEX_NUM = HEX_WIDTH_NUM * HEX_HEIGHT_NUM;

//六角形内の五角形
const int PENTA_NUM = 4 * HEX_NUM;
const Vector PENTA_OFFSET[ 4 ] = {
	Vector( -PANEL_SPRITE_SIZE + 64, -PANEL_SPRITE_SIZE + 64 ),//右下にずらす
	Vector(                    - 64, -PANEL_SPRITE_SIZE + 64 ),//左下にずらす
	Vector( -PANEL_SPRITE_SIZE + 64,                    - 64 ),//右上にずらす
	Vector(                    - 64,                    - 64 ),//左上にずらす
};

//-------------ヘキサグラム系統-------------//
const int HEXAGON_SPRITE_SIZE = 512;
const int HEXAGON_LEFT_WIDTH_NUM = HEX_WIDTH_NUM;
const int HEXAGON_RIGHT_WIDTH_NUM = HEX_WIDTH_NUM - 1;
const int HEXAGON_LEFT_HEIGHT_NUM = HEX_HEIGHT_NUM - 1;
const int HEXAGON_RIGHT_HEIGHT_NUM = HEX_HEIGHT_NUM;
const int HEXAGON_BOTH_WIDTH_NUM = HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM;
//ボタン数
const int HEXAGON_BUTTON_NUM = HEXAGON_LEFT_WIDTH_NUM * HEXAGON_LEFT_HEIGHT_NUM + HEXAGON_RIGHT_WIDTH_NUM * HEXAGON_RIGHT_HEIGHT_NUM;
const int HEXAGON_IN_PANEL_NUM = 4;

//Boardの描画座標
const Vector BOARD_DRAW_POS = Vector( 0, -128 );

//イラスト関係
const int MAX_ILLUST_NUM = 29;
const int ILLUST_WIDTH  = 1024;
const int ILLUST_HEIGHT = 1024;

enum PLAYER {
	PLAYER_1,
	PLAYER_2,
	MAX_PLAYER
};

enum SCORETYPE {
	SCORETYPE_OFFLINE,
	SCORETYPE_A,
	SCORETYPE_B,
	SCORETYPE_LIVE,
};

struct Room {
	int idx;
	int illust[ MAX_PLAYER ];
	SCORETYPE score_type;
	Room( ) {
		idx = -1;
		for ( int i = 0; i < MAX_PLAYER; i++ ) {
			illust[ i ] = -1;
		}
		score_type = SCORETYPE_OFFLINE;
	}
};