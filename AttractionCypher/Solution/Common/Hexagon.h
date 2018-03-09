#pragma once
#include <array>
#include "define.h"
#include "smart_ptr.h"

PTR( Image );
PTR( Panel );
PTR( Board );
PTR( TouchEffecter );

class Hexagon {
public:
	Hexagon( int idx, BoardPtr board, TouchEffecterPtr touch_effecter );
	virtual ~Hexagon( );
public:
	void update( );
	void drawButton( ImagePtr image ) const;
	void drawBullet( ImagePtr image, double size ) const;
	void drawHexagonIdx( ) const;
public:
	void setActive( bool active );
public:
	bool isTouch( ) const;
	bool isInPanel( int idx );
	bool isActive( ) const;
	int getIdx( ) const;
	PanelPtr getInPenta( int idx ) const;
	std::array< int, HEXAGON_IN_PANEL_NUM > getHexInPentaIdx( ) const;
private:
	enum FORM {
		FORM_RIGHT,
		FORM_LEFT,
	};
private:
	int _idx;
	bool _touch;
	bool _active;
	FORM _form;
	Vector _pos;
	BoardConstPtr _board;
	std::array< int, HEXAGON_IN_PANEL_NUM > _have_penta_idx;
	std::array< PanelPtr, HEXAGON_IN_PANEL_NUM > _have_penta;
	TouchEffecterPtr _touch_effecter;
};

