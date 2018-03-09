#pragma once
#include "Image.h"
#include <array>

PTR( PentaVertex );

class Panel {
public:
public:
	Panel( int idx, PentaVertexPtr penta_vertex );
	virtual ~Panel( );
public:
	enum STATE {
		STATE_NONE,
		STATE_DEFAULT,
		STATE_PLAYER1,
		STATE_PLAYER2,
		STATE_FADE
	};
public:
	void update( );
	void drawBase( ImagePtr image, int image_idx ) const;
	void drawDefault( ImagePtr image, int image_idx ) const;
	void drawBlackPanel( ImagePtr image ) const;
	void drawFrame( ImagePtr image, int image_idx ) const;
	void drawBlur( ImagePtr image, double fade ) const;
	void drawPanelIdx( ) const;
	void setState( STATE state );
	void initState( );
	void setTouch( ); //debug—p
	void setActive( bool active );
	bool isExistence( const Vector& pos ) const;
	bool isTouch( ) const;
	bool isActive( ) const;
	int getIdx( ) const;
	STATE getState( ) const;
	Vector getPos( );
	std::array< int, 5 > getVertexIdx( ) const;
private:
	void checkInScreen( );
	void checkTouch( );
	Vector getRotationPos( const Vector& pos ) const;
private:
	Vector _pos;
	int _idx;
	bool _in_screen;
	bool _touch;
	bool _active;
	std::array< int, 5 > _vertex_idx;
	STATE _state;
	PentaVertexPtr _penta_vertex;
};