#include "TouchEffecter.h"
#include "TouchEffect.h"
#include "Board.h"
#include "define.h"
#include "Panel.h"
#include "PentaVertex.h"
#include "EffectDrawer.h"

TouchEffecter::TouchEffecter( PentaVertexPtr penta_vertex, BoardConstPtr board ) :
_penta_vertex( penta_vertex ),
_board( board ) {
	_effect_drawer = EffectDrawerPtr( new EffectDrawer );
}

TouchEffecter::~TouchEffecter( ) {
}


void TouchEffecter::update( ) {
	std::list< TouchEffectPtr >::iterator ite = _touch_effects.begin( );
	while ( ite != _touch_effects.end( ) ) {
		TouchEffectPtr effect = *ite;
		if ( effect->isFinished( ) ) {
			ite = _touch_effects.erase( ite );
			continue;
		}
		effect->update( );
		ite++;
	}
}

void TouchEffecter::draw( ) const {
	for ( TouchEffectPtr effect : _touch_effects ) {
		effect->draw( );
	}
	_effect_drawer->draw( );
}

bool TouchEffecter::isAlreadyPenta( int idx ) const {
	for ( TouchEffectPtr effect : _touch_effects ) {
		if ( effect->getIdxPenta( ) == idx ) {
			return true;
		}
	}
	return false;
}

bool TouchEffecter::isAlreadyHex( int idx ) const {
	for ( TouchEffectPtr effect : _touch_effects ) {
		if ( effect->getIdxHex( ) == idx ) {
			return true;
		}
	}
	return false;
}

void TouchEffecter::addEffectPenta( int penta_idx ) {
	if ( isAlreadyPenta( penta_idx ) ) {
		return;
	}
	PanelPtr penta = _board->getPanel( penta_idx );
	_touch_effects.push_back( TouchEffectPtr( new TouchEffect( _penta_vertex, penta, _effect_drawer ) ) );
}

void TouchEffecter::addEffectHex( int hex_idx ) {
	HexagonPtr hex = _board->getHexagon( hex_idx );
	_touch_effects.push_back( TouchEffectPtr( new TouchEffect( _penta_vertex, hex, _effect_drawer ) ) );
}
