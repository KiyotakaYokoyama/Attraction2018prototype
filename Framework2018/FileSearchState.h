#include <DxLib.h>
#include <iostream>
#include <vector>
#include <string>

class FileSearchState {
public:
    class iterator : public std::iterator< std::input_iterator_tag, std::string > {
    public:
        iterator( const char* path );
        ~iterator( );
        const FILEINFO& operator*( ) const;
        const FILEINFO* operator->( ) const;
        iterator& operator++( );
        bool operator==( const iterator &x ) const;
        bool operator!=( const iterator &x ) const;
    private:
        FILEINFO fileinfo;
        ULONGLONG findhandle;
        bool isValid;
    };

    FileSearchState( const char* path );
    iterator begin( ) const;
    iterator& end( ) const;
private:
    const char* path;
};

FileSearchState findFile( const char* path );
FileSearchState findFile( const std::string path );
