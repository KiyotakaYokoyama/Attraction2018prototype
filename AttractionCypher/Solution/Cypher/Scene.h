#pragma once
#include "smart_ptr.h"

PTR( ImageTarget );
PTR( Board );

class Scene {
public:
	Scene( BoardPtr board );
	virtual ~Scene( );
public:
	enum SCENE {
		SCENE_CONTINUE,//���V�[��
		SCENE_SELECT,
		SCENE_PLAY,
		SCENE_WAIT_SCREEN,
		SCENE_GAMEOVER,
		SCENE_CONTINUE_PLAY,//�Q�[���I�[�o�[�V�[���ŃQ�[�����R���e�j���[����
	};
public:
	virtual SCENE update( ) = 0;
protected:
	void checkTap( );
	bool isReturnTitle( ) const;
	BoardPtr getBoard( ) const;
	ImageTargetPtr getScreen( ) const;
private:
	int _change_count;
	BoardPtr _board;
	ImageTargetPtr _screen;
};

