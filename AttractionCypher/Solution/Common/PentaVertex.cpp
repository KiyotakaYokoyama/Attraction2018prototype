#include "PentaVertex.h"
#include "Panel.h"
#include "define.h"
#include <array>

//距離
const int VERTEX_BASE[ 5 ][ 2 ] {
	{ 0, 0 },
	{ -103, 25   },
	{ -153, -26  },
	{ -128, -128 },
	{ -26 , -103 }
};

const int PENTA_LINE[ 5 ][ 2 ] {
	{ VERTEX_BASE[ 1 ][ 0 ] - VERTEX_BASE[ 0 ][ 0 ], VERTEX_BASE[ 1 ][ 1 ] - VERTEX_BASE[ 0 ][ 1 ] },
	{ VERTEX_BASE[ 2 ][ 0 ] - VERTEX_BASE[ 1 ][ 0 ], VERTEX_BASE[ 2 ][ 1 ] - VERTEX_BASE[ 1 ][ 1 ] },
	{ VERTEX_BASE[ 3 ][ 0 ] - VERTEX_BASE[ 2 ][ 0 ], VERTEX_BASE[ 3 ][ 1 ] - VERTEX_BASE[ 2 ][ 1 ] },
	{ VERTEX_BASE[ 4 ][ 0 ] - VERTEX_BASE[ 3 ][ 0 ], VERTEX_BASE[ 4 ][ 1 ] - VERTEX_BASE[ 3 ][ 1 ] },
	{ VERTEX_BASE[ 0 ][ 0 ] - VERTEX_BASE[ 4 ][ 0 ], VERTEX_BASE[ 0 ][ 1 ] - VERTEX_BASE[ 4 ][ 1 ] },
};

PentaVertex::PentaVertex( ) {
	checkVertex( );
}

PentaVertex::~PentaVertex( ) {
}

PentaVertex::Point PentaVertex::getPoint( int idx ) const {
	return _points[ idx ];
}

void PentaVertex::checkVertex( ) {
	int check1 = 0;
	//全ての5角形の頂点の場所を計算する

	std::vector< int > tmp_idx;

	for ( int i = 0; i < PENTA_NUM; i++ ) {
		int hex_idx = i / 4;
		int type = i % 4;
		Vector hex_pos = Vector( HEX_WIDTH * ( hex_idx % HEX_WIDTH_NUM ), HEX_HEIGHT * ( hex_idx / HEX_WIDTH_NUM ) );
		for ( int j = 0; j < 5; j++ ) {

			//頂点座標を割り出す
			Point point = Point( );
			point.pos = getAdjustPos( type, j );
			point.pos += BOARD_DRAW_POS + hex_pos;

			//すでに在る頂点の場合は抜ける
			if ( getPointIdx( point.pos ) >= 0 ) {
				continue;
			}
			//追加
			tmp_idx.push_back( i * 5 + j );
			_points.push_back( point );
		}
	}

	//連結している頂点を探す

	int size = ( int )_points.size( );
	for ( int i = 0; i < size; i++ ) {
		int penta_type = ( tmp_idx[ i ] / 5 ) % 4;
		int vertex_type =  tmp_idx[ i ] % 5;
		std::vector< Vector > pos = getCombinePos( penta_type, vertex_type );
		int pos_num = ( int )pos.size( );
		for ( int j = 0; j < pos_num; j++ ) {
			int idx = getPointIdx( _points[ i ].pos + pos[ j ] );
			if ( idx < 0 ) {
				if ( _points[ i ].pos.x > 0 && _points[ i ].pos.y > 0 ) {
					int check = 0;
				}
				continue;
			}
			_points[ i ].point_idx.push_back( idx );
		}
	}
}

int PentaVertex::getPointIdx( Vector pos ) const {
	int size = ( int )_points.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( ( pos - _points[ i ].pos ).getLength2( ) < 13 ) {
			return i;
		}
	}
	return -1;
}

Vector PentaVertex::getAdjustPos( int penta_type, int vertex_type ) const {
	int dir_x = ( penta_type % 2 == 0 ) * 2 - 1;
	int dir_y = ( penta_type / 2 == 0 ) * 2 - 1;
	int xy_reverse = ( penta_type % 3 != 0 );
	return Vector( VERTEX_BASE[ vertex_type ][ xy_reverse ] * dir_x, VERTEX_BASE[ vertex_type ][ ( xy_reverse + 1 ) % 2 ] * dir_y );
}

std::vector< Vector > PentaVertex::getCombinePos( int penta_type, int vertex_type ) const {
	std::vector< Vector > result;
	if ( penta_type == 2 ) {
		penta_type = 3;
	} else {
		if ( penta_type == 3 ) {
			penta_type = 2;
		}
	}

	switch ( vertex_type ) {
	case 0:
	{
		int x1 = PENTA_LINE[ 0 ][ 0 ];
		int y1 = PENTA_LINE[ 0 ][ 1 ];
		
		int x2 = PENTA_LINE[ 4 ][ 0 ];
		int y2 = PENTA_LINE[ 4 ][ 1 ];
		
		int x3 = PENTA_LINE[ 0 ][ 0 ];
		int y3 = PENTA_LINE[ 0 ][ 1 ];
		
		int x4 = PENTA_LINE[ 0 ][ 0 ];
		int y4 = PENTA_LINE[ 0 ][ 1 ];

		rot( x1, y1, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );
		rot( x2, y2, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x3, y3, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x4, y4, ( ROT )( ( ROT_270 + penta_type ) % MAX_ROT ) );

		result.push_back( Vector( x1, y1 ) );
		result.push_back( Vector( x2, y2 ) );
		result.push_back( Vector( x3, y3 ) );
		result.push_back( Vector( x4, y4 ) );
	}
		break;
	case 1:
	{
		int x1 = PENTA_LINE[ 1 ][ 0 ];
		int y1 = PENTA_LINE[ 1 ][ 1 ];
		
		int x2 = PENTA_LINE[ 0 ][ 0 ];
		int y2 = PENTA_LINE[ 0 ][ 1 ];
		
		int x3 = PENTA_LINE[ 2 ][ 0 ];
		int y3 = PENTA_LINE[ 2 ][ 1 ];

		rot( x1, y1, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );
		rot( x2, y2, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x3, y3, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );

		result.push_back( Vector( x1, y1 ) );
		result.push_back( Vector( x2, y2 ) );
		result.push_back( Vector( x3, y3 ) );
	}
		break;
	case 2:
	{
		int x1 = PENTA_LINE[ 2 ][ 0 ];
		int y1 = PENTA_LINE[ 2 ][ 1 ];
		
		int x2 = PENTA_LINE[ 1 ][ 0 ];
		int y2 = PENTA_LINE[ 1 ][ 1 ];
		
		int x3 = PENTA_LINE[ 0 ][ 0 ];
		int y3 = PENTA_LINE[ 0 ][ 1 ];

		rot( x1, y1, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );
		rot( x2, y2, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x3, y3, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );

		result.push_back( Vector( x1, y1 ) );
		result.push_back( Vector( x2, y2 ) );
		result.push_back( Vector( x3, y3 ) );
	}
		break;
	case 3:
	{
		int x1 = PENTA_LINE[ 3 ][ 0 ];
		int y1 = PENTA_LINE[ 3 ][ 1 ];
		
		int x2 = PENTA_LINE[ 2 ][ 0 ];
		int y2 = PENTA_LINE[ 2 ][ 1 ];
		
		int x3 = PENTA_LINE[ 3 ][ 0 ];
		int y3 = PENTA_LINE[ 3 ][ 1 ];
		
		int x4 = PENTA_LINE[ 2 ][ 0 ];
		int y4 = PENTA_LINE[ 2 ][ 1 ];

		rot( x1, y1, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );
		rot( x2, y2, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x3, y3, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x4, y4, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );

		result.push_back( Vector( x1, y1 ) );
		result.push_back( Vector( x2, y2 ) );
		result.push_back( Vector( x3, y3 ) );
		result.push_back( Vector( x4, y4 ) );
	}
		break;
	case 4:
	{
		int x1 = PENTA_LINE[ 4 ][ 0 ];
		int y1 = PENTA_LINE[ 4 ][ 1 ];
		
		int x2 = PENTA_LINE[ 3 ][ 0 ];
		int y2 = PENTA_LINE[ 3 ][ 1 ];
		
		int x3 = PENTA_LINE[ 1 ][ 0 ];
		int y3 = PENTA_LINE[ 1 ][ 1 ];
		
		rot( x1, y1, ( ROT )( ( ROT_0   + penta_type ) % MAX_ROT ) );
		rot( x2, y2, ( ROT )( ( ROT_180 + penta_type ) % MAX_ROT ) );
		rot( x3, y3, ( ROT )( ( ROT_90  + penta_type ) % MAX_ROT ) );

		result.push_back( Vector( x1, y1 ) );
		result.push_back( Vector( x2, y2 ) );
		result.push_back( Vector( x3, y3 ) );
	}
		break;
	}

	return result;
}

void PentaVertex::rot( int& x, int& y, ROT rot ) const {
	switch ( rot ) {
	case PentaVertex::ROT_0:
		break;
	case PentaVertex::ROT_90:
	{
		int tmp = x;
		x = -y;
		y = tmp;
	}
		break;
	case PentaVertex::ROT_180:
		x *= -1;
		y *= -1;
		break;
	case PentaVertex::ROT_270:
	{
		int tmp = x;
		x = y;
		y = -tmp;
	}
		break;
	default:
		break;
	}
}

int PentaVertex::getPointNum( ) const {
	return ( int )_points.size( );
}
