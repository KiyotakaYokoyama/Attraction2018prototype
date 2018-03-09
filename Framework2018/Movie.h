#pragma once
#include <string>
#include "smart_ptr.h"

PTR( Movie );

class Movie {
public:
	Movie( );
	virtual ~Movie( );
public:
	enum BLEND {
		BLEND_NONE,
		BLEND_ALPHA,
		BLEND_ADD,
		MAX_BLEND,
	};
public:
	bool load( std::string path, bool alpha = false );
	void draw( int sx = 0, int sy = 0, int sw = 0, int sh = 0 ) const;
	void stop( );
	void play( bool is_loop = true );
	int getSeek( );
	void setSeek( int time );
	bool isPlaying( ) const;
	void setBlend( BLEND blend, double ratio );
	void setSoundVolume( int volume );
private:
	int _handle;
	BLEND _blend;
	double _ratio;
};

