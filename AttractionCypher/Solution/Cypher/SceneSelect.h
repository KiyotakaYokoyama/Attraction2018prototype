#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include <array>
#include <vector>
#include "define.h"

PTR( Image );
PTR( DataBase );
PTR( Ntp );
PTR( MusicSync );

class SceneSelect : public Scene {
public:
	SceneSelect( BoardPtr board, MusicSyncPtr sync );
	virtual ~SceneSelect( );
private:
	SCENE update( );
	void draw( ) const;
	void changeIllusts( );
	void setButton( );
	void createIllusts( );
	void checkClearPanel( );
	bool decisionIllust( );
	void checkIllustEnableSelect( ); 
	void initSelectIllust( );
private:
	int _check_count;
	int _select_illust;
	int _touch;
	DataBasePtr _db;
	MusicSyncPtr _sync;
	NtpPtr _ntp;
	std::array< ImagePtr, MAX_ILLUST_NUM > _illusts;
	std::array< bool, MAX_ILLUST_NUM > _enable_select;
};
