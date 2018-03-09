#pragma once
#include "mathmatics.h"
#include <vector>

class PentaVertex {
public:
	struct Point {
		Vector pos;
		std::vector< int > point_idx;
	};
public:
	PentaVertex( );
	virtual ~PentaVertex( );
public:
	Vector getAdjustPos( int penta_type, int vertex_type ) const;
	std::vector< Vector > getCombinePos( int penta_type, int vertex_type ) const;
	Point getPoint( int idx ) const;
	int getPointIdx( Vector pos ) const;
	int getPointNum( ) const;
private:
	enum ROT {
		ROT_0,
		ROT_90,
		ROT_180,
		ROT_270,
		MAX_ROT,
	};
private:
	void checkVertex( );
	void rot( int& x, int& y, ROT rot ) const;
private:
	std::vector< Point > _points;
};

