#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include "mathmatics.h"
#include <string>
#include <map>

PTR( Application );
PTR( Binary );

class Application {
public:
	Application( );
	virtual ~Application( );
public:
	static void initialize( );
	static void finalize( );
	static ApplicationPtr getInstance( );
public:
	void run( );
	void addTask( std::string tag, TaskPtr task );
	TaskPtr getTask( std::string tag );
	int getWindowWidth( ) const;
	int getWindowHeight( ) const;
	bool loadBinary( std::string filename, BinaryPtr binary );
	void saveBinary( std::string filename, BinaryPtr binary );
	std::string inputString( int sx, int sy );
	void terminate( );
	void changeWindow( bool windowed );
	void setWindowSize( int width, int height );
	void setGraphSize( int width, int height );
private:
	void initSettings( );
private:
	static ApplicationPtr _instance;
	int _screen_width;
	int _screen_height;
	int _window_width;
	int _window_height;
	std::map< std::string, TaskPtr > _task_list;
	bool _terminating;
};

