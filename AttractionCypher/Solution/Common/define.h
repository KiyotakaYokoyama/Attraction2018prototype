#pragma once
#include "mathmatics.h"

//��ʃT�C�Y
const int SCREEN_WIDTH = 1920;
const int SCREEN_HIGHT = 1080;

//-------------�p�l���n��-------------//
const int PANEL_SPRITE_SIZE = 256;
const int HEX_WIDTH  = 256;
const int HEX_HEIGHT = 256;
const int HEX_WIDTH_NUM  = SCREEN_WIDTH / HEX_WIDTH + 2;
const int HEX_HEIGHT_NUM = SCREEN_WIDTH / HEX_WIDTH + 2;
const int HEX_NUM = HEX_WIDTH_NUM * HEX_HEIGHT_NUM;

//�Z�p�`���̌܊p�`
const int PENTA_NUM = 4 * HEX_NUM;
const Vector PENTA_OFFSET[ 4 ] = {
	Vector( -PANEL_SPRITE_SIZE + 64, -PANEL_SPRITE_SIZE + 64 ),//�E���ɂ��炷
	Vector(                    - 64, -PANEL_SPRITE_SIZE + 64 ),//�����ɂ��炷
	Vector( -PANEL_SPRITE_SIZE + 64,                    - 64 ),//�E��ɂ��炷
	Vector(                    - 64,                    - 64 ),//����ɂ��炷
};

//-------------�w�L�T�O�����n��-------------//
const int HEXAGON_SPRITE_SIZE = 512;
const int HEXAGON_LEFT_WIDTH_NUM = HEX_WIDTH_NUM;
const int HEXAGON_RIGHT_WIDTH_NUM = HEX_WIDTH_NUM - 1;
const int HEXAGON_LEFT_HEIGHT_NUM = HEX_HEIGHT_NUM - 1;
const int HEXAGON_RIGHT_HEIGHT_NUM = HEX_HEIGHT_NUM;
const int HEXAGON_BOTH_WIDTH_NUM = HEXAGON_LEFT_WIDTH_NUM + HEXAGON_RIGHT_WIDTH_NUM;
//�{�^����
const int HEXAGON_BUTTON_NUM = HEXAGON_LEFT_WIDTH_NUM * HEXAGON_LEFT_HEIGHT_NUM + HEXAGON_RIGHT_WIDTH_NUM * HEXAGON_RIGHT_HEIGHT_NUM;
const int HEXAGON_IN_PANEL_NUM = 4;

//Board�̕`����W
const Vector BOARD_DRAW_POS = Vector( 0, -128 );

//�C���X�g�֌W
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