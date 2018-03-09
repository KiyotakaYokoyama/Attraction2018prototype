#pragma once
#include "mathmatics.h"
#include "smart_ptr.h"

PTR( EffectDrawer );

class ElectricShock {
public:
	ElectricShock( int target_vertex_idx, Vector start_pos, Vector target_pos, EffectDrawerPtr effect_drawer );
	virtual ~ElectricShock( );
public:
	void draw( ) const;
	int getTargetVertexIdx( ) const;
private:
	Vector _start_pos;
	Vector _target_pos;
	EffectDrawerPtr _effect_drawer;
	int _target_vertex_idx;
};

