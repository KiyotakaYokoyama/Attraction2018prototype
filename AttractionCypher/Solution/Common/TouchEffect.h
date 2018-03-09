#pragma once
#include "smart_ptr.h"
#include <vector>

PTR( ElectricShock );
PTR( PentaVertex );
PTR( EffectDrawer );
PTR( Panel );
PTR( Hexagon );

class TouchEffect {
public:
	TouchEffect( PentaVertexPtr penta_vertex, PanelConstPtr penta, EffectDrawerPtr effect_drawer );
	TouchEffect( PentaVertexPtr penta_vertex, HexagonConstPtr hex, EffectDrawerPtr effect_drawer );
	virtual ~TouchEffect( );
public:
	void update( );
	void draw( ) const;
	int getIdxPenta( ) const;
	int getIdxHex( ) const;
	bool isFinished( ) const;
private:
	void initVisited( );
	void setPentaVisited( PanelConstPtr penta );
	void setHexVisited( HexagonConstPtr hex );
private:
	int _idx_penta;
	int _idx_hex;
	int _count;
	bool _finished;
	PentaVertexPtr _penta_vertex;
	std::vector< int > _create_idx;
	std::vector< bool > _visited;
	std::vector< ElectricShockPtr > _electric_effects;
	EffectDrawerPtr _effect_drawer;
};

