#pragma once

// ポート番号
static const int TCP_PORT_NUM = 9850;
static const int UDP_PORT_NUM = 9900;

/*
// TCP送信データ
const unsigned char COMMAND_DUMMY		   = 0xF0; // ダミー
const unsigned char COMMAND_CONDITION	   = 0xE0; // 接続確認
const unsigned char COMMAND_STATUS_DAMAGE  = 0x10; // ダメージ
const unsigned char COMMAND_BOSS_DEAD	   = 0x20; // ボスを倒したことの送信

const unsigned int BOSS_1 = 0x01;
const unsigned int BOSS_2 = 0x02;

const unsigned char SCENE_TITLE		= 0x00;
const unsigned char SCENE_PLAY		= 0x01;
const unsigned char SCENE_CLEAR		= 0x02;
const unsigned char SCENE_GAMEOVER	= 0x03;

const int PLAYER_NUM = 4;

const unsigned char PLAYER_0 = 0x00;
const unsigned char PLAYER_1 = 0x01;
const unsigned char PLAYER_2 = 0x02;
const unsigned char PLAYER_3 = 0x03;
const unsigned char PLAYER_NONE        = 0xff;

const unsigned char BUTTON_NONE = 0x00; // INPUT_NONE ( 0b00000000 )
const unsigned char BUTTON_A    = 0x01; // INPUT_A ( 0b00000001 )
const unsigned char BUTTON_B    = 0x02; // INPUT_B ( 0b00000010 )
const unsigned char BUTTON_C    = 0x04; // INPUT_C ( 0b00000100 )
const unsigned char BUTTON_D    = 0x08; // INPUT_D ( 0b00001000 )
const unsigned char BUTTON_E    = 0x10; // INPUT_E ( 0b00010000 )
const unsigned char BUTTON_F    = 0x20; // INPUT_F ( 0b00100000 )

const unsigned char NOT_EXIST = 0x00;
const unsigned char EXIST = 0x01;

const int MAX_MACHINE = 10;

#pragma pack( 1 )
	struct SERVERDATA {
		unsigned int command;
		unsigned int value[ 3 ];
	};
#pragma pack( )
	
*/