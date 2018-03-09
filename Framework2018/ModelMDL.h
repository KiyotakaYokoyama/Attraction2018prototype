#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( ModelMDL );
PTR( ModelImpl );

class ModelMDL {
public:
	ModelMDL( );
	virtual ~ModelMDL( );
public:
	struct VERTEX {
		Vector pos;
		double u;
		double v;
		VERTEX( ) { }
		VERTEX( 
			Vector pos_,
			double u_,
			double v_ ) {
				pos = pos_;
				u = u_;
				v = v_;
		}
	};
public:
	void draw( ) const;
	void draw( int texture ) const;
	void multiply( const Matrix& matrix );
	void alloc( int polygon_num );
	void setPolygonNum( int num );
	int  getPolygonNum( ) const;
	void set( int n, VERTEX vertex ); // n * 3
	bool load( std::string filename );
	void save( std::string filename );
	void mergeModel( ModelMDLConstPtr model );
	void setTrans( bool trans );
	void reset( );
	void setTexture( const char* texture_path );
	VERTEX getVERTEX( int idx );
	Vector getPoint( int idx ) const;
	ModelImplConstPtr getModelImpl( ) const;
	int getTextureHandle( const char* filename );
private:
	ModelImplPtr _impl;
	int _texture_handle;
	bool _trans;
};

