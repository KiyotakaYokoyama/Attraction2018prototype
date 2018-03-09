#include "ModelMV1.h"
#include "DxLib.h"
#include <assert.h>

const int FRAME_INDEX = -1;

ModelMV1::ModelMV1( ) :
	_mesh( -1 ),
	_anim( -1 ),
	_time( 0 ) {
}


ModelMV1::~ModelMV1( ) {
	if ( _mesh >= 0 ) {
		MV1DeleteModel( _mesh );
	}
}

void ModelMV1::load( const char* filename ) {
	_mesh = MV1LoadModel( filename );
	
	assert( _mesh > 0 );
	int num = MV1GetMaterialNum( _mesh ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( _mesh, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}
	MV1SetupCollInfo( _mesh, FRAME_INDEX );
}

void ModelMV1::setTexture( const char* filename, bool semi_trans ) {
	int graph = MV1LoadTexture( filename );
	if ( semi_trans ) {
		MV1SetTextureGraphHandle( _mesh, 0, graph, TRUE );
	} else {
		MV1SetTextureGraphHandle( _mesh, 0, graph, FALSE );
	}
}


void ModelMV1::draw( ) const {
	Matrix mat = _rot_mat.multiply( _scale_mat ).multiply( _trans_mat );
	MATRIX matrix = MGetIdent( );
	for( int j = 0; j < 3 * 3; j++ ) {
		int n = j % 3;
		int m = j / 3;
		matrix.m[ m ][ n ] = ( float )mat.data[ n ][ m ];
	}
	for ( int j = 0; j < 4; j++ ) {
		matrix.m[ j ][ 3 ] = ( float )mat.data[ j ][ 3 ];
		matrix.m[ 3 ][ j ] = ( float )mat.data[ 3 ][ j ];
	}
	MV1SetMatrix( _mesh, matrix );
	//ƒAƒjƒ[ƒVƒ‡ƒ“Ý’è
	int idx = MV1AttachAnim( _mesh, 0, _anim, TRUE );
	MV1SetAttachAnimTime( _mesh, idx, ( float )_time );
	// ‚R‚cƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel( _mesh );
	MV1DetachAnim( _mesh, idx );
}

void ModelMV1::setTrans( const Matrix& matrix ) {
	_trans_mat = matrix;
}

void ModelMV1::setRot( const Matrix& matrix ) {
	_rot_mat = matrix;
}

void ModelMV1::setScale( const Matrix& matrix ) {
	_scale_mat = matrix;
}

void ModelMV1::transferTransform( const Matrix& trans_mat ) {
	_trans_mat = _trans_mat.multiply( trans_mat );
}

void ModelMV1::rotateTransform( const Matrix& rot_mat ) {
	_rot_mat = _rot_mat.multiply( rot_mat );
}

void ModelMV1::scaleTransform( const Matrix& scale_mat ) {
	_rot_mat = _rot_mat.multiply( scale_mat );
}

double ModelMV1::getEndAnimTime( ) const {
	return MV1GetAnimTotalTime( _mesh, 0 );
}

void ModelMV1::reset( ) {
	MV1DeleteModel( _mesh );
	_mesh = -1;
	_anim = -1;
	_time = 0;
}

Vector ModelMV1::getHitPos( ) const {
	return _hit_pos;
}


bool ModelMV1::isHitLine( const Vector& begin, const Vector& end ) {
	//‚¢‚Á‚½‚ñ‹K’è’l‚É‚ ‚í‚¹‚é
	MV1SetupCollInfo( _mesh, FRAME_INDEX );
	VECTOR start_pos = VGet( (float)begin.x, (float)begin.y,(float)begin.z );
	VECTOR end_pos = VGet( (float)end.x, (float)end.y, (float)end.z );
	MV1_COLL_RESULT_POLY hit_poly = MV1CollCheck_Line( _mesh, FRAME_INDEX, start_pos, end_pos );
	if ( hit_poly.HitFlag == 1 ) {
		_hit_pos = Vector( ( double )hit_poly.HitPosition.x, ( double )hit_poly.HitPosition.y, ( double )hit_poly.HitPosition.z );
	}
	return hit_poly.HitFlag == 1;
}

bool ModelMV1::isHitSphere( const Vector& pos, const float radius ) const {
	//‚¢‚Á‚½‚ñ‹K’è’l‚É‚ ‚í‚¹‚é
	MV1SetupCollInfo( _mesh, FRAME_INDEX );
	VECTOR center_pos = VGet( (float)pos.x, (float)pos.y,(float)pos.z );
	MV1_COLL_RESULT_POLY_DIM hit_poly = MV1CollCheck_Sphere( _mesh, FRAME_INDEX, center_pos, radius );
	bool result = hit_poly.HitNum > 0;
	MV1CollResultPolyDimTerminate( hit_poly );
	return result;
}

void ModelMV1::setAnimTime( double time ) {
	_time = time;
}

void ModelMV1::setAnim( int anim ) {
	_anim = anim;
}