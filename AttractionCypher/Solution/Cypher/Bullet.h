#pragma once
#include "Image.h"

PTR( Board );
PTR( Hexagon );

class Bullet {
public:
	Bullet( BoardPtr board, int idx, int time );
	virtual ~Bullet( );
public:
	void update( int time );
	void draw( ) const;
public://set
	void setActiveHex( bool value );
	void setFinished( );
public://get is
	bool isFinished( ) const;
	bool canTouch( int time ) const;
	int getIdx( ) const;
	double calcSize( int time ) const;
private:
	long long _target_time;
	int _idx;
	double _size;
	bool _finished;
	Vector _pos;
	BoardPtr _board;
	HexagonPtr _hex;
};