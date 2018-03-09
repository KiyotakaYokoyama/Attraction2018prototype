#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include "define.h"
#include "Music.h"

PTR( Area );
PTR( Image );
PTR( Status );
PTR( MusicSync );

class ScenePlay : public Scene {
public:
	ScenePlay( BoardPtr board, MusicSyncPtr sync, bool continue_play = false );
	virtual ~ScenePlay( );
private:
	enum RESULT {
		RESULT_WIN,
		RESULT_LOSE,
		RESULT_DRAW,
		RESULT_NONE
	};
	enum TUTORIAL {
		TUTORIAL_TEXT1,
		TUTORIAL_PLAY,
		TUTORIAL_TEXT2,
		TUTORIAL_END
	};
private:
	SCENE update( );
	void draw( ) const;
private://draw
	void drawResult( ) const;
	void drawNextPlayMode( ) const;
	void drawSoloGauge( ) const;
	void drawTutorialGauge( ) const;
	void drawTutorialText1( ) const;
	void drawTutorialText2( ) const;
	void drawMessage( ) const ;
private:
	bool updateTutotial( );
	void updateState( );
	void initIllustScreen( );
	void createPanelIllusts( ) const;
	void createImages( );
	void setResult( );
	void setWinnerIllust( );
	bool isTutorial( ) const;
	bool isTouch( ) const;
	void setState( STATE state );
private:
	ImagePtr _interval_message;
	ImagePtr _meter;
	ImagePtr _tutorial_text;
	ImagePtr _number;
	ImagePtr _player_illust[ MAX_ILLUST_NUM ];
	ImageTargetPtr _illust_draw_screen;
	ImageTargetPtr _illust_for_screen[ MAX_PLAYER ];
	AreaPtr _area;
	StatusPtr _status;
	STATE _state;
	MusicSyncPtr _sync;
	RESULT _result;
	int _count;
	TUTORIAL _tutorial;
};

