#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( Image );
PTR( Particle );

class EffectDrawer {
public:
	EffectDrawer( );
	virtual ~EffectDrawer( );
public:
	void draw( );
	void addEffect( Vector start, Vector end );
private:
	static const int SPRITE_NUM = 10;
	static const int MAX_PARTICLE_NUM = 5000;
private:
	std::array< ParticlePtr, MAX_PARTICLE_NUM > _particles;
	ImagePtr _images[ SPRITE_NUM ];
	int _particle_idx;
	int _count;
};

class Particle {
public:
	Particle( Vector start, Vector end ) :
	start_pos( start ),
	end_pos( end ),
	count( 0 ) {
	}
	~Particle( ) { }
public:
	Vector start_pos;
	Vector end_pos;
	int count;
};
