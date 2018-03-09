//2017.12　Kinect for Windows SDK 2.0
#pragma once

#include "Task.h"
#include "mathmatics.h"
#include <array>
#include <map>


#ifdef KINECT
#include <Kinect.h>
#endif

PTR( KinectForWindows );

class KinectForWindows : public Task {
public:
	static KinectForWindowsPtr getTask( );
	static std::string getTag( ) { return "KINECT_FOR_WINDOWS"; }

public:
	KinectForWindows( );
	virtual ~KinectForWindows( );
public:
	void initialize( );
	void update( );
	void finalize( );
	bool isExistJointPoint( int body_index );
	
	#ifdef KINECT
	Vector getJointPoint( int body_index, JointType joint_type );//キネクト中心を０にしたときの座標取得
	Vector getJointDepthPoint( int body_index, JointType joint_type );
	#endif

private:
	void initializeKinectSensor( );//キネクトセンサーの初期化
	void initializeBodyReader( );//ボディリーダーの初期化
	void getBodyFrame( );//ボディフレームの取得
	
	#ifdef KINECT
	bool errorCheck( HRESULT result);
	#endif

private:
	#ifdef KINECT
	IKinectSensor* _kinect;
	//ボディ系変数
	IBodyFrameReader* _bodyFrameReader;
	std::array< std::map< JointType, Joint >, BODY_COUNT > _bodyJointData;//各ボディのジョイントデータ
	#endif
	int _timer;
};
