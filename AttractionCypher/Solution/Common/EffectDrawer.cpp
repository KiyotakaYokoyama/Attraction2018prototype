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
		//カウントによる削除
		int count = _count - _particles[ i ]->count;
		if ( count >= SPRITE_NUM ) {
			_particles[ i ] = ParticlePtr( );
			continue;
		}
		//目的地までの距離
		Vector vec = _particles[ i ]->end_pos - _particles[ i ]->start_pos;
		//角度
		double angle = Vector( 0, -1 ).angle( vec );
		if ( Vector( 0, -1 ).cross( vec ).z < 0 ) {
			angle *= -1;
		}
		//表示画像( 1フレームごとに画像を変える )
		int sprite = count;
		//表示座標
		int sx1 = ( int )( _particles[ i ]->start_pos.x + vec.x * 0.5 );
		int sy1 = ( int )( _particles[ i ]->start_pos.y + vec.y * 0.5 );
		//描画
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
