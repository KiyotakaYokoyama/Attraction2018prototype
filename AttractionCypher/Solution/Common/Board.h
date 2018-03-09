#pragma once

#include <array>
#include <vector>
#include "Image.h"
#include "define.h"
#include "Panel.h"

PTR( Panel );
PTR( Hexagon );
PTR( TouchEffecter );

class Board : public std::enable_shared_from_this< Board > {
public:
	Board( );
	~Board( );
public:
	void init( PentaVertexPtr penta_vertex = PentaVertexPtr( ) );
	void update( );
	void updateTouchEffect( );
	void initPanelState( );
public://draw
	void drawElectric( ) const;
	void drawBase( ) const;
	void drawDefault( ) const;
	void drawFrame( ) const;
	void drawButton( ) const;
	void drawBullet( int draw_hex_idx, double size ) const;
	void drawBlackPanel( std::vector< int > draw_penta_idxs ) const;
	void drawBlur( std::vector< int > draw_penta_idxs ) const;
	void drawPanelForIllust( PLAYER player ) const;
	void drawIdx( ) const;
public://get is
	PanelPtr getPanel( int idx ) const;
	HexagonPtr getHexagon( int idx ) const;
	int getPanelIdx( const Vector& pos ) const;
	bool isTouchPanel( int penta_idx ) const;
	bool isTouchHexagon( int hex_idx ) const;
	bool isActiveHexagon( int hex_idx ) const;
	std::array< int, HEXAGON_IN_PANEL_NUM > getHexInPentaIdx( int hex_idx ) const;
public://other
	void setPanelState( Panel::STATE state, std::vector< int > panels );
	void setActiveHexagon( int idx, bool active );
	void setWinnerIllust( const Panel::STATE winner );
	void addButton( int hex_idx );
	void addEffectHex( int hex_idx );
	void addEffectPenta( int penta_idx );
	void clearButton( );
private://update
	void updatePanels( );
	void updateHexagons( );
	void updateTouchedPanelToHexagons( );
private:
	void checkTouchedHexagons( int touch_panel );
	bool isMacthPanel( std::vector< int > _clear_panels, int idx ) const;
	void fading( );
	void initFading( );
private:
	double _fade;
	bool _fade_in;
	std::array< PanelPtr, PENTA_NUM > _panels;
	std::array< HexagonPtr, HEXAGON_BUTTON_NUM > _hexagons;
	std::vector< int > _button_idx;
	ImagePtr _image_penta;
	ImagePtr _image_penta_base;
	ImagePtr _image_penta_default;
	ImagePtr _image_pentaframe;
	ImagePtr _image_hex;
	TouchEffecterPtr _touch_effecter;
};

