#include "Effect.h"
#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include <assert.h>

const int PARTICLE = 3000; //Effekseerの最大パーティクル数

EffectPtr Effect::getTask( ) {
	return std::dynamic_pointer_cast< Effect >( Application::getInstance( )->getTask( getTag( ) ) );
}

Effect::Effect( const char * directory ) :
_handle_num( 0 ),
_directory( directory ) {
	// DirectX9を使用するようにする。
	// Effekseerを使用するには必ず設定する。
	//SetUseDirect3DVersion(DX_DIRECT3D_9);


	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if ( Effkseer_Init( PARTICLE ) == -1 ) {
		return;
	}

		
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();


	std::string path = _directory;
	path += "/";
	path += "effekseer_fix.png";
	_effekseer_graph = LoadGraph( path.c_str( ) );
	assert( _effekseer_graph >= 0 );
	DrawGraph( 0, 0, _effekseer_graph, TRUE );

	for ( int i = 0; i < MAX_EFFECT_NUM; i++ ) {
		_handles[ i ] = -1;
	}
}

Effect::~Effect( ) {
}

void Effect::initialize( ) {
}

void Effect::finalize( ) {
	for ( int i = 0; i < _handle_num; i++ ) {
		DeleteEffekseerEffect( _handles[ _handle_num ] );
	}
	Effkseer_End( );
}

void Effect::update( ) {
}

int Effect::loadEffect( const char * filename ) {
	std::string path = _directory;
	path += "/";
	path +=  filename;
	int result = LoadEffekseerEffect( path.c_str( ) );
	if ( result < 0 ) {
		path = "../" + path;
		result = LoadEffekseerEffect( path.c_str( ) );
		assert( result >= 0 );
	}
	_handles[ _handle_num ] = result;
	_handle_num = ( _handle_num + 1 ) % MAX_EFFECT_NUM;
	return result;
}

int Effect::playEffect( const int effect_handle ) {
	return PlayEffekseer3DEffect( effect_handle );
}

void Effect::updateEffectTransform( const int effect_handle, const Vector& pos, const double size, const Vector& rotate ) const {
	SetScalePlayingEffekseer3DEffect( effect_handle,
		( float )size, ( float )size, ( float )size );
	SetRotationPlayingEffekseer3DEffect( effect_handle,
		( float )rotate.x, ( float )rotate.y, ( float )rotate.z );
	SetPosPlayingEffekseer3DEffect( effect_handle,
		( float )pos.x, ( float )pos.y, ( float )pos.z );
}

void Effect::drawEffect( ) const {
	DrawGraph( 0, 0, _effekseer_graph, TRUE );
	RenderVertex( );

	// EffekseerをDxLibのカメラと同期させる
	Effekseer_Sync3DSetting( );

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D( );

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D( );
}

void Effect::stopEffect( int effect_handle ) const {
	StopEffekseer3DEffect( effect_handle );
}
