#include "ModelMDL.h"
#include "dxlib.h"

class ModelImpl {
public:
	VERTEX3D *_vertex;
	unsigned int _polygon_num;
};

ModelMDL::ModelMDL( ) {
	_impl = ModelImplPtr( new ModelImpl );

	_impl->_vertex = NULL;
	_impl->_polygon_num = 0;
	_texture_handle = -1;
}

ModelMDL::~ModelMDL( ) {
	if ( _impl->_vertex ) {
		delete [] _impl->_vertex;
	}
}

void ModelMDL::alloc( int polygon_num ) {
	_impl->_polygon_num = polygon_num;
	if ( _impl->_vertex != NULL ) {
		delete [] _impl->_vertex;
	}
	_impl->_vertex = new VERTEX3D[ polygon_num * 3 ];
}

void ModelMDL::setPolygonNum( int num ) {
	_impl->_polygon_num = num;
}

void ModelMDL::draw( ) const {
	int check = DrawPolygon3D( _impl->_vertex, _impl->_polygon_num, _texture_handle, _trans ? TRUE : FALSE );
}

void ModelMDL::draw( int texture ) const {
	int check = DrawPolygon3D( _impl->_vertex, _impl->_polygon_num, texture, _trans ? TRUE : FALSE );
}

void ModelMDL::set( int n, VERTEX vertex ) {

	VERTEX3D vtx;
	vtx.pos = VGet( ( float )vertex.pos.x, ( float )vertex.pos.y, ( float )vertex.pos.z );
	vtx.norm = VGet( 0, 0, 0 );
	vtx.dif  = GetColorU8( 255, 255, 255, 255 );
	vtx.spc  = GetColorU8( 0, 0, 0, 0 );
	vtx.u    = ( float )vertex.u;
	vtx.v    = ( float )vertex.v;
	vtx.su   = 0.0f;
	vtx.sv   = 0.0f;

	_impl->_vertex[ n ] = vtx;
}

ModelMDL::VERTEX ModelMDL::getVERTEX( int idx ){
	VERTEX vertex;
	vertex.pos = Vector( _impl->_vertex[ idx ].pos.x, _impl->_vertex[ idx ].pos.y, _impl->_vertex[ idx ].pos.z );
	vertex.u = _impl->_vertex[ idx ].u;
	vertex.v = _impl->_vertex[ idx ].v;
	return vertex;
}


bool ModelMDL::load( std::string filename ) {
	int fh = FileRead_open( filename.c_str( ) );
	if ( fh <= 0 ) {
		return false;
	}

	unsigned int polygon_num = 0;
	FileRead_read( &polygon_num, sizeof( unsigned int ), fh );
	
	alloc( polygon_num );

	FileRead_read( _impl->_vertex, sizeof( DxLib::VERTEX3D ) * ( polygon_num * 3 ), fh );

	FileRead_close( fh );
	return true;
}

void ModelMDL::save( std::string filename ) {
	FILE *fp;
	errno_t err = fopen_s( &fp, filename.c_str( ), "wb" );
	if ( err != 0 ) {
		return;
	}
	
	fwrite( &_impl->_polygon_num, sizeof( unsigned int ), 1, fp );
	fwrite( _impl->_vertex, sizeof( DxLib::VERTEX3D ) * ( _impl->_polygon_num * 3 ), 1, fp );

	fclose( fp );
}

void ModelMDL::multiply( const Matrix& matrix ) {
	int count = ( int )_impl->_polygon_num * 3;
	for ( int i = 0; i < count; i++ ) {
		Vector pos(
			_impl->_vertex[ i ].pos.x,
			_impl->_vertex[ i ].pos.y,
			_impl->_vertex[ i ].pos.z );
		pos = matrix.multiply( pos );
		_impl->_vertex[ i ].pos = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );
	}
}

void ModelMDL::mergeModel( ModelMDLConstPtr model ) {
	if ( model == NULL ) {
		return;
	}

	ModelImplConstPtr merge_impl = model->getModelImpl( );
	int polygon_num = _impl->_polygon_num + merge_impl->_polygon_num;
	VERTEX3D* vertex = new VERTEX3D[ sizeof( VERTEX3D ) * polygon_num * 3 ];

	// this のモデルをコピー
	int base_count = _impl->_polygon_num * 3;
	for ( int i = 0; i < base_count; i++ ) {
		vertex[ i ] = _impl->_vertex[ i ];
	}
	// modelのモデルをコピー
	int marge_count = merge_impl->_polygon_num * 3;
	for ( int i = 0; i < marge_count; i++ ) {
		int idx = _impl->_polygon_num * 3 + i;
		vertex[ idx ] = merge_impl->_vertex[ i ];
	}

	if ( _impl->_vertex ) {
		delete [] _impl->_vertex;
	}
	_impl->_vertex = vertex;
	_impl->_polygon_num = polygon_num;
}

ModelImplConstPtr ModelMDL::getModelImpl( ) const {
	return _impl;
}

Vector ModelMDL::getPoint( int idx ) const {
	return Vector(
		_impl->_vertex[ idx ].pos.x,
		_impl->_vertex[ idx ].pos.y,
		_impl->_vertex[ idx ].pos.z );
}

int ModelMDL::getPolygonNum( ) const {
	return _impl->_polygon_num;
}

int ModelMDL::getTextureHandle( const char* filename ) {
	return LoadGraph( filename );
}

void ModelMDL::reset( ) {
	_impl = ModelImplPtr( new ModelImpl );

	_impl->_vertex = NULL;
	_impl->_polygon_num = 0;
}

void ModelMDL::setTexture( const char* filename ) {
	_texture_handle = getTextureHandle( filename );
}

void ModelMDL::setTrans( bool trans ) {
	_trans = trans;
}
