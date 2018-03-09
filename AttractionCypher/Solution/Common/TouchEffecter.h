#pragma once
#include "smart_ptr.h"
#include <list>
#include <array>
#include "define.h"

PTR( Board );
PTR( TouchEffect );
PTR( PentaVertex );
PTR( EffectDrawer );

class TouchEffecter {
public:
	TouchEffecter( PentaVertexPtr penta_vertex, BoardConstPtr board );
	~TouchEffecter( );
public:
	void update( );
	void draw( ) const;
public:
	void addEffectHex( int hex_idx );
	void addEffectPenta( int penta_idx );
private:
	bool isAlreadyPenta( int idx ) const;
	bool isAlreadyHex  ( int idx ) const;
private:
	PentaVertexPtr _penta_vertex;
	BoardConstPtr _board;
	std::list< TouchEffectPtr > _touch_effects;
	EffectDrawerPtr _effect_drawer;
};

