#pragma once
#include "Scene.h"
#include <vector>
#include "smart_ptr.h"
#include "mathmatics.h"

PTR( Image );

class SceneGameOver : public Scene {
public:
	SceneGameOver( BoardPtr board );
	virtual ~SceneGameOver( );
public:
	SCENE update( );
private:
	void draw( ) const;
	bool isTouchContinue( ) const;
	void checkClearPanels( );
	void checkContinuePos( );
private:
	ImagePtr _image;
	std::vector< int > _clear_panels;
	Vector _continue_pos;
	int _count;
};

