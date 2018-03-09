#include "FileSearchState.h"

FileSearchState::iterator::iterator( const char* path ) :
isValid( false ){
    if ( path == nullptr ) {
        findhandle = -1;
        isValid = false;
    } else {
        findhandle = FileRead_findFirst(path, &fileinfo);
        isValid = (findhandle != -1);
    }
}

FileSearchState::iterator::~iterator( ) {
	if ( findhandle != -1 ) {
        FileRead_findClose(findhandle);
    }
}

const FILEINFO& FileSearchState::iterator::operator*() const {
	return fileinfo;
}

const FILEINFO* FileSearchState::iterator::operator->() const {
	return &fileinfo;
}

FileSearchState::iterator& FileSearchState::iterator::operator++( ) {
    isValid = ( FileRead_findNext( findhandle, &fileinfo ) != -1 );
    return *this;
}

bool FileSearchState::iterator::operator==( const iterator &x ) const {
    return (isValid && x.isValid && findhandle == x.findhandle) || (!isValid && !x.isValid);
}

bool FileSearchState::iterator::operator!=( const iterator &x ) const {
	return !( *this == x );
}

FileSearchState::FileSearchState( const char* path ) {
    this->path = path;
}

FileSearchState::iterator FileSearchState::begin( ) const {
    return iterator( path );
}

FileSearchState::iterator& FileSearchState::end( ) const {
    static iterator endItr( nullptr );
    return endItr;
}

FileSearchState findFile( const char* path ){
    return FileSearchState(path);
}

FileSearchState findFile( const std::string path ) {
    return FileSearchState(path.c_str());
}
