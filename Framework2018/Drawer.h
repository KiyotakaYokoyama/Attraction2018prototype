#pragma once
#include "Task.h"
#include "mathmatics.h"
#include <string>
#include <array>

PTR( Drawer );
PTR( Model );
PTR( Image );
PTR( Movie );
PTR( ImageTarget );

class Drawer : public Task {
public:
	static DrawerPtr getTask( );
	static std::string getTag( ) { return "DRAWER"; }
public:
	Drawer( const char* directory );
	virtual ~Drawer( );
public:
	void initialize( );
	void update( );
	void drawCircle( const Vector& pos, const double radius ) const;
	void drawSphere( const Vector& pos, const double radius ) const;
	void drawSphere( const Vector& pos, const double radius, int polygon_num, unsigned int color, int fill_flag ) const;
	void drawEffect( int id_, const Vector& pos_, double size_ = 1.0, const Vector& rotate_ = Vector( ) ) const;
	void loadEffect( int id, const char* filename );
	ImagePtr createImage( const char* filename ) const;
	MoviePtr createMovie( const char* filename, bool alpha = false ) const;
	void drawString( int x, int y, const char* string, ... ) const;
	void drawLine( int x1, int y1, int x2, int y2 ) const;
	void drawLine( const Vector& pos1, const Vector& pos2 ) const;
	void setCameraUp( const Vector& up );
	void setCamera( const Vector& pos, const Vector& target );
	void setCameraCenter( double x, double y);
	void setCameraPerspective( double fov );
	void setImageTarget( ImageTargetPtr image = ImageTargetPtr( ) );
	void setFog( double start, double end, int red, int green, int blue );
	void resetFPS( );
	void waitForSync( );
	void flip( );
	double getFps( );
	bool isInCamera( const Vector& pos ) const;
	int getNowTime( ) const;
private:
	const char* _directory;
	
	static const int EFFECT_ID_NUM = 1000;
	std::array< int, EFFECT_ID_NUM > _effect_id;

	int _refresh_count;
	int _start_time;
	int _effekseer_fix_graph;
	int _before_time;
	double _fps;
	int _fps_count;
	int _fps_start_time;

	Vector _camera_up;
};
