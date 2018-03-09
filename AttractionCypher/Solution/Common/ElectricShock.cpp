#include "ElectricShock.h"
#include "EffectDrawer.h"


ElectricShock::ElectricShock( int target_vertex_idx, Vector start_pos, Vector target_pos, EffectDrawerPtr effect_drawer ) :
_start_pos( start_pos ),
_target_vertex_idx( target_vertex_idx ),
_target_pos( target_pos ),
_effect_drawer( effect_drawer ) {
}


ElectricShock::~ElectricShock( ) {
}

void ElectricShock::draw( ) const {
	_effect_drawer->addEffect( _start_pos, _target_pos );
}

int ElectricShock::getTargetVertexIdx( ) const {
	return _target_vertex_idx;
}
