#include "EffectDrawer.h"
#include "Drawer.h"
#include "Image.h"

const int SPRITE_SIZE = 64;

EffectDrawer::EffectDrawer( ) :
_particle_idx( 0 ),
_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < SPRITE_NUM; i++ ) {
		char path[ 36 ];
		sprintf_s( path, "TouchEffect/touch_effect_%02d.png", i + 1 ); 
		_images[ i ] = drawer->createImage( path );
		_images[ i ]->setBlend( Image::BLEND_ADD, 0.5 );
	}
}

EffectDrawer::~EffectDrawer( ) {
}

void EffectDrawer::draw( ) {
	//int num = 0;
	double size_ratio = 1.0 / SPRITE_SIZE;
	double ext_x = 6.0 * size_ratio;
	for ( int i = 0; i < MAX_PARTICLE_NUM; i++ ) {
		if ( _particles[ i ] ==  ParticlePtr( ) ) {
			continue;
		}
		//�J�E���g�ɂ��폜
		int count = _count - _particles[ i ]->count;
		if ( count >= SPRITE_NUM ) {
			_particles[ i ] = ParticlePtr( );
			continue;
		}
		//�ړI�n�܂ł̋���
		Vector vec = _particles[ i ]->end_pos - _particles[ i ]->start_pos;
		//�p�x
		double angle = Vector( 0, -1 ).angle( vec );
		if ( Vector( 0, -1 ).cross( vec ).z < 0 ) {
			angle *= -1;
		}
		//�\���摜( 1�t���[�����Ƃɉ摜��ς��� )
		int sprite = count;
		//�\�����W
		int sx1 = ( int )( _particles[ i ]->start_pos.x + vec.x * 0.5 );
		int sy1 = ( int )( _particles[ i ]->start_pos.y + vec.y * 0.5 );
		//�`��
		_images[ sprite ]->setImageRotation( sx1, sy1, SPRITE_SIZE / 2, SPRITE_SIZE / 2, angle, Vector( ext_x, vec.getLength( ) * size_ratio ) );
		_images[ sprite ]->drawRotation( );
	}
	_count++;
}

void EffectDrawer::addEffect( Vector start, Vector end ) {
	_particles[ _particle_idx ] = ParticlePtr( new Particle( start, end ) );
	_particles[ _particle_idx ]->count = _count;
	_particle_idx = ( _particle_idx + 1 ) % MAX_PARTICLE_NUM;
}
