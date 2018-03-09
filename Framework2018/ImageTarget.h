#pragma once
#include "Image.h"
#include <string>

class ImageTarget: public Image {
public:
	ImageTarget( );
	virtual ~ImageTarget( );
public:
	void create( int width, int height );
	void clear( );
	void save( std::string path );
};

