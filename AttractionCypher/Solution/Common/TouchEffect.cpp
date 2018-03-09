#include "TouchEffect.h"
#include "Board.h"
#include "ElectricShock.h"
#include "PentaVertex.h"
#include "Panel.h"
#include "Hexagon.h"

TouchEffect::TouchEffect( PentaVertexPtr penta_vertex, PanelConstPtr penta, EffectDrawerPtr effect_drawer ) :
_penta_vertex( penta_vertex ),
_effect_drawer( effect_drawer ),
_finished( false ),
_idx_hex( -1 ) {
	_idx_penta = penta->getIdx( );
	initVisited( );
	setPentaVisited( penta );
}


TouchEffect::TouchEffect( PentaVertexPtr penta_vertex, HexagonConstPtr hex, EffectDrawerPtr effect_drawer ) :
_penta_vertex( penta_vertex ),
_effect_drawer( effect_drawer ),
_finished( false ),
_idx_penta( -1 ) {
	_idx_hex = hex->getIdx( );
	initVisited( );
	setHexVisited( hex );
}

TouchEffect::~TouchEffect( ) {
}

void TouchEffect::initVisited( ) {
	int size = _penta_vertex->getPointNum( );
	_visited.resize( size );
	for ( int i = 0; i < size; i++ ) {
		_visited[ i ] = false;
	}
}

void TouchEffect::setPentaVisited( PanelConstPtr penta ) {
	std::array< int, 5 > idx = penta->getVertexIdx( );
	for ( int i = 0; i < 5; i++ ) {
		_create_idx.push_back( idx[ i ] );
		_visited[ idx[ i ] ] = true;
	}
}

void TouchEffect::setHexVisited( HexagonConstPtr hex ) {	
	for ( int i = 0; i < HEXAGON_IN_PANEL_NUM; i++ ) {
		setPentaVisited( hex->getInPenta( i ) );
	}
}


void TouchEffect::update( ) {
	//生成場所確認
	for ( ElectricShockPtr effect : _electric_effects ) {
		int target = effect->getTargetVertexIdx( );
		if ( _visited[ target ] ) {
			continue;
		}
		_visited[ target ] = true;
		_create_idx.push_back( target );
	}
		

	//生成・削除
	_electric_effects.clear( );
	int size = ( int )_create_idx.size( );
	for ( int i = 0; i < size; i++ ) {
		PentaVertex::Point point = _penta_vertex->getPoint( _create_idx[ i ] );
		std::vector< int > idx = point.point_idx;
		int size2 = ( int )idx.size( );
		bool create = false;
		for ( int j = 0; j < size2; j++ ) {
			if ( _visited[ idx[ j ] ] ) {
				continue;
			}
			create = true;
			_electric_effects.push_back( ElectricShockPtr( new ElectricShock( idx[ j ], point.pos, _penta_vertex->getPoint( idx[ j ] ).pos, _effect_drawer ) ) );
		}
	}
	//生成数が0の場合は終了
	if ( ( int )_create_idx.size( ) == 0 ) {
		_finished = true;
	}

	_create_idx.clear( );
}

void TouchEffect::draw( ) const {
	//1フレームのみ描画
	for ( ElectricShockPtr effect : _electric_effects ) {
		effect->draw( );
	}

}

bool TouchEffect::isFinished( ) const {
	return _finished;
}

int TouchEffect::getIdxPenta( ) const {
	return _idx_penta;
}

int TouchEffect::getIdxHex( ) const {
	return _idx_hex;
}