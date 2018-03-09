#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include <map>
#include <string>

PTR( Sound );

class Sound : public Task {
public:
	static std::string getTag( ) { return "SOUND"; }
	static SoundPtr getTask( );
public:
	Sound( const char* directory );
	virtual ~Sound( );
public:
	void initialize( );
	void finalize( );
public:
	void update( );
	bool isPlayingSE( std::string filename );
	void playSE( std::string filename, bool loop = false );
	void stopSE( std::string filename );
	void loadSE( std::string filename );
	void stopAllSE( );
	void playBGM( std::string filename, bool loop = true );
	void stopBGM( );
	void setVolumeBGM( double volume );
	bool isPlayingBGM( );
private:
	const char* _directory;
	std::map< std::string, int > _se;
	int _bgm;
};

