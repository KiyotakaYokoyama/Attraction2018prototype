#include "Sound.h"
#include "Application.h"
#include "Dxlib.h"
#include <string>

SoundPtr Sound::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Sound >( fw->getTask( Sound::getTag( ) ) );
}

Sound::Sound( const char * directory ) :
_directory( directory ){
}

void Sound::initialize( ) {
	//‚r‚d
}

void Sound::finalize( ) {
}

Sound::~Sound( ) {
	
}

void Sound::update( ) {
}

bool Sound::isPlayingSE( std::string filename ) {
	if ( _se.count( filename ) == 0 ) {
		return false;
	}

	return CheckSoundMem( _se[ filename ] ) == 1;
}

void Sound::loadSE( std::string filename ) {
	if ( _se.count( filename ) != 0 ) {
		return;
	}

	SetCreateSoundDataType( DX_SOUNDDATATYPE_MEMNOPRESS );

	std::string path = _directory;
	path += "/";
	path +=  filename;
	int id = LoadSoundMem( path.c_str( ) );
	if ( id < 0 ) {
		path = "../" + path;
		id = LoadSoundMem( path.c_str( ) );
	}
	_se[ filename ] = id;
}

void Sound::playSE( std::string filename, bool loop ) {
	loadSE( filename );

	if ( loop ) {
		PlaySoundMem( _se[ filename ], DX_PLAYTYPE_LOOP );
	} else {
		PlaySoundMem( _se[ filename ], DX_PLAYTYPE_BACK );
	}
}

void Sound::stopSE( std::string filename ) {
	if ( _se.count( filename ) == 0 ) {
		return;
	}

	StopSoundMem( _se[ filename ] );
}

void Sound::playBGM( std::string filename, bool loop ) {
	// ‘O‰ñ‚Ì‚a‚f‚l‚ðíœ
	DeleteSoundMem( _bgm );

	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;
	
	std::string path = _directory;
	path += "/";
	path +=  filename;
	int id = LoadSoundMem( path.c_str( ) );
	if ( id < 0 ) {
		path = "../" + path;
		id = LoadSoundMem( path.c_str( ) );
	}
	
	_bgm = id;
	
	int flg = DX_PLAYTYPE_BACK;
	if ( loop ) {
		flg = DX_PLAYTYPE_LOOP;
	}
	
	PlaySoundMem( _bgm, flg );
}

void Sound::setVolumeBGM( double volume ) {
	if ( _bgm < 0 ) {
		return;
	}
	ChangeVolumeSoundMem( ( int )( volume * 255 ), _bgm ); 
}

void Sound::stopBGM( ) {
	DeleteSoundMem( _bgm );
	_bgm = -1;
}

void Sound::stopAllSE( ) {
	std::map< std::string, int >::iterator it = _se.begin( );
	while ( it != _se.end( ) ) {
		StopSoundMem( (*it).second );
		it++;
	}
}

bool Sound::isPlayingBGM( ) {
	return CheckSoundMem( _bgm ) == 1;
}
