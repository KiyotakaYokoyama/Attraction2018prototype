#include "Parser.h"
#include "ModelMDL.h"
#include "DxLib.h"
#include <vector>
#include <array>
#include <assert.h>

Parser::Parser( ) {
}


Parser::~Parser( ) {
}

ModelMDLPtr Parser::makeModel( std::string filename ) {
	enum MODE {
		MODE_SEECK_SEARCH,
		MODE_READ_MATRIX,
		MODE_READ_POINT,
		MODE_READ_INDEX,
		MODE_READ_NORMALS_POINT,
		MODE_READ_NORMALS_INDEX,
		MODE_READ_TEXTURE,
		MODE_READ_MATERIAL,
	};

	int fh = FileRead_open( filename.c_str( ) );
	if ( fh == 0 ) {
		return ModelMDLPtr( );
	}

	int texture_num = 0;
	int mat_num = 0;
	MODE mode = MODE_SEECK_SEARCH;
	Matrix matrix;

	int point_num = 0;
	std::vector< Vector > point_array;

	struct Index {
		std::array< unsigned int, 3 > idx;
	};

	struct UV {
		float u;
		float v;
	};

	int point_index_num = 0;
	std::vector< Index > point_index_array;

	int normal_num = 0;
	std::vector< Vector > normal_array;
	
	int normal_index_num;
	std::vector< Index > normal_index_array;

	std::vector< int > material_array;
	std::vector< UV > texture_array;

	while ( FileRead_eof( fh ) == 0 ) {
		char buf[ 1024 ];
		FileRead_gets( buf, 1024, fh );
		std::string str = buf;
		switch( mode ) {
			
			case MODE_SEECK_SEARCH:
				if ( str.find( "FrameTransformMatrix" ) != -1 ) {
					mode = MODE_READ_MATRIX;
				}
				break;
			case MODE_READ_MATRIX:
				if ( mat_num < 4 ) {
					double dat_1;
					double dat_2;
					double dat_3;
					double dat_4;
					sscanf_s( buf, "%lf,%lf,%lf,%lf,", &dat_1, &dat_2, &dat_3, &dat_4 );
					matrix.data[ mat_num ][0] = dat_1;
					matrix.data[ mat_num ][1] = dat_2;
					matrix.data[ mat_num ][2] = dat_3;
					matrix.data[ mat_num ][3] = dat_4;

					mat_num++;
				}
				if ( str.find( "Mesh" ) != -1 ) {
					FileRead_gets( buf, 1024, fh );
					sscanf_s( buf, "%d,", &point_num );
					mode = MODE_READ_POINT;
				}
				break;
			case MODE_READ_POINT: {
				double x;
				double y;
				double z;
				sscanf_s( buf, "%lf;%lf;%lf;,", &x, &y, &z );
				point_array.push_back( Vector( x, y, z ) );
				if ( point_array.size( ) == point_num ) {
					FileRead_gets( buf, 1024, fh );
					mode = MODE_READ_INDEX;
					sscanf_s( buf, "%d;", &point_index_num );
				}
				break;
			}
			case MODE_READ_INDEX: {
				int polygon;
				int point_1;
				int point_2;
				int point_3;
				sscanf_s( buf, "%d;%d,%d,%d;,", &polygon, &point_1, &point_2, &point_3 );
				assert( polygon == 3 );
				Index index;
				index.idx[ 0 ] = point_1;
				index.idx[ 1 ] = point_2;
				index.idx[ 2 ] = point_3;
				point_index_array.push_back( index );
				if ( point_index_num == point_index_array.size( ) ) {
					FileRead_gets( buf, 1024, fh );
					FileRead_gets( buf, 1024, fh );
					sscanf_s( buf, "%d,", &normal_num );
					mode = MODE_READ_NORMALS_POINT;
				}
				break;
			}
			case MODE_READ_NORMALS_POINT: {
				double x;
				double y;
				double z;
				sscanf_s( buf, "%lf;%lf;%lf;,", &x, &y, &z );
				normal_array.push_back( Vector( x, y, z ) );
				if ( normal_array.size( ) == normal_num ) {
					FileRead_gets( buf, 1024, fh );
					sscanf_s( buf, "%d,", &normal_index_num );
					mode = MODE_READ_NORMALS_INDEX;
				}
				break;
			}
			case MODE_READ_NORMALS_INDEX: {
				int polygon;
				int point_1;
				int point_2;
				int point_3;
				sscanf_s( buf, "%d;%d,%d,%d;,", &polygon, &point_1, &point_2, &point_3 );
				assert( polygon == 3 );
				Index index;
				index.idx[ 0 ] = point_1;
				index.idx[ 1 ] = point_2;
				index.idx[ 2 ] = point_3;
				normal_index_array.push_back( index );
				if (  normal_index_num == normal_index_array.size( ) ) {
					FileRead_gets( buf, 1024, fh );
					FileRead_gets( buf, 1024, fh );
					FileRead_gets( buf, 1024, fh );
					sscanf_s( buf, "%d,", &texture_num );
					mode = MODE_READ_TEXTURE;
				}
				}
				break;
			case MODE_READ_TEXTURE: {
				float u;
				float v;
				sscanf_s( buf, "%f;%f;,", &u, &v );
				UV uv;
				uv.u = u;
				uv.v = v;
				texture_array.push_back( uv );
				if (  texture_array.size( ) == texture_num ) {
					FileRead_gets( buf, 1024, fh );
					FileRead_gets( buf, 1024, fh );
					mode = MODE_READ_MATERIAL;
				}
				break;
			}
			case MODE_READ_MATERIAL: {
				int material;
				sscanf_s( buf, "%d;,", &material );
				material_array.push_back( material );
				if (  str.find( "}" ) != -1 ) {
					FileRead_gets( buf, 1024, fh );
					mode = MODE_READ_MATERIAL;
				}
				break;
			}
		}
	
	}

	FileRead_close( fh );

	//‚±‚±‚Åƒ‚ƒfƒ‹‚ðì‚éB
	ModelMDLPtr model( new ModelMDL );
	model->alloc( point_index_num );
	for ( int i = 0; i < point_index_num; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			int idx = point_index_array[ i ].idx[ j ];
			Vector point = point_array[ idx ];
			ModelMDL::VERTEX vertex;
			vertex.pos = matrix.multiply( point );
			vertex.u = texture_array[ idx ].u;
			vertex.v = texture_array[ idx ].v;
			model->set( i * 3 + j, vertex );
		}
	}
	return model;	
}

