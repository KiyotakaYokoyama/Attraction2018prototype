#include "Gesture.h"
#include "Application.h"

#ifdef KINECT
////////////////////////////////////////////////////////////////////
//
// GestureImpl ( with Kinect )
//
////////////////////////////////////////////////////////////////////
#include <kinect.h>
#include <assert.h>
#include <array>
#include <map>

#pragma comment( lib, "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/lib/x64/kinect20.lib" )

const int SKIP_COUNT = 5;

class GestureImpl {
public:
	GestureImpl( ) { }
	virtual ~GestureImpl( ) { }
public:
	void initialize( ) {
		initializeKinectSensor( );
		initializeBodyReader( );

		errorCheck( _kinect->get_CoordinateMapper( &_mapper ) );
	}
	void finalize( ) {
		_kinect->Close( );
		_kinect->Release( );
		_bodyFrameReader->Release( );
	}
public:
	bool isExistJointPoint( int body_index, Gesture::JOINT joint ) {
		return _joints[ body_index ][ conv( joint ) ].TrackingState != TrackingState_NotTracked;
	}

	Vector getJointPoint( int body_index, Gesture::JOINT joint ) {
		CameraSpacePoint pos = _joints[ body_index ][ conv( joint ) ].Position;
		return Vector( pos.X, pos.Y, pos.Z );
	}
	Vector getJointDepthPoint( int body_index, Gesture::JOINT joint ) {
		DepthSpacePoint point;
		_mapper->MapCameraPointToDepthSpace( _joints[ body_index ][ conv( joint ) ].Position, &point );
		return Vector( point.X, point.Y );	
	}
	Vector getJointColorPoint( int body_index, Gesture::JOINT joint ) {
		ColorSpacePoint point;
		_mapper->MapCameraPointToColorSpace( _joints[ body_index ][ conv( joint ) ].Position, &point );
		return Vector( point.X, point.Y );	
	}

	void updateBodyFrame( ) {
		for ( int i = 0; i < BODY_COUNT; i++ ) {
			for ( int j = 0; j < JointType::JointType_Count; j++ ) {
				_joints[ i ][ j ].TrackingState = TrackingState_NotTracked;
			}
		}

		IBodyFrame* body_frame;
		HRESULT result = _bodyFrameReader->AcquireLatestFrame( &body_frame );
		if ( result != S_OK ) {
			return;
		}

		IBody* bodies[ BODY_COUNT ] = { nullptr };
		body_frame->GetAndRefreshBodyData( BODY_COUNT, bodies );

		//ボディデータに変換
		for ( int i = 0; i < BODY_COUNT; i++ ) {
			if ( bodies[ i ] == nullptr ) {
				continue;
			}

			BOOLEAN tracked = false;
			bodies[ i ]->get_IsTracked( &tracked );
			if ( !tracked ) {
				continue;
			}

			Joint joints[ JointType::JointType_Count ];
			bodies[ i ]->GetJoints( JointType::JointType_Count, joints );

			for ( int j = 0; j < JointType::JointType_Count; j++ ) {
				_joints[ i ][ joints[ j ].JointType ] = joints[ j ];
			}
		}

		body_frame->Release( );
	}

private:
	JointType conv( Gesture::JOINT joint ) {
		switch ( joint ) {
		case Gesture::JOINT_HEAD:
			return JointType_Head;
		case Gesture::JOINT_SPINE_SHOULDER:
			return JointType_SpineMid;
			
		case Gesture::JOINT_SHOULDER_RIGHT:
			return JointType_ShoulderRight;
		case Gesture::JOINT_SHOULDER_LEFT:
			return JointType_ShoulderLeft;
			
		case Gesture::JOINT_HAND_RIGHT:
			return JointType_HandRight;
		case Gesture::JOINT_HAND_LEFT:
			return JointType_HandLeft;
		case Gesture::JOINT_FOOT_RIGHT:
			return JointType_FootRight;
		case Gesture::JOINT_FOOT_LEFT:
			return JointType_FootLeft;
		}

		return JointType_Head;
	}

	void initializeKinectSensor( ) {
		errorCheck( ::GetDefaultKinectSensor( &_kinect ) );//センサーの取得
		errorCheck( _kinect->Open( ) );// Kinectセンサーを動作させる

		// Kinectセンサーが動いたかどうか状態を取得する
		BOOLEAN isOpen = false;
		errorCheck( _kinect->get_IsOpen( &isOpen ) );
		if ( !isOpen ) {
			errorCheck( S_FALSE );
		}
	}
	void initializeBodyReader( ) {
		IBodyFrameSource* bodyFrameSource = nullptr;
		errorCheck( _kinect->get_BodyFrameSource( &bodyFrameSource ) );
		errorCheck( bodyFrameSource->OpenReader( &_bodyFrameReader ) );
	}
	
	bool errorCheck( HRESULT result ) {
		assert( S_OK == result );
		return S_OK == result;
	}
private:
	IKinectSensor* _kinect;
	IBodyFrameReader* _bodyFrameReader;
	ICoordinateMapper* _mapper;
	Joint _joints[ BODY_COUNT ][ JointType::JointType_Count ];
};

#else

////////////////////////////////////////////////////////////////////
//
// GestureImpl ( without Kinect )
//
////////////////////////////////////////////////////////////////////
class GestureImpl {
public:
	GestureImpl( ) { }
	virtual ~GestureImpl( ) { }
	void initialize( ) { }
	void finalize( ) { }
	void updateBodyFrame( ) { }
	bool isExistJointPoint( int , Gesture::JOINT ) { return false; }
	Vector getJointPoint( int , Gesture::JOINT  ) { return Vector( ); }
	Vector getJointColorPoint( int , Gesture::JOINT  ) { return Vector( ); }
	Vector getJointDepthPoint( int , Gesture::JOINT  ) { return Vector( ); }
};

#endif

////////////////////////////////////////////////////////////////////
//
// Gesture
//
////////////////////////////////////////////////////////////////////
GesturePtr Gesture::getTask( ) {
	return std::dynamic_pointer_cast< Gesture >( Application::getInstance( )->getTask( getTag( ) ) );
}

Gesture::Gesture( ) {
	_impl = GestureImplPtr( new GestureImpl( ) );
}

Gesture::~Gesture( ) {
}

void Gesture::update( ) {
}

void Gesture::updateBodyFrame( ) { 
	_impl->updateBodyFrame( );
}

void Gesture::initialize( ) {
	_impl->initialize( );
}

void Gesture::finalize( ) {
	_impl->finalize( );
}

bool Gesture::isExistJointPoint( int body_index, JOINT joint ) {
	return _impl->isExistJointPoint( body_index, joint );
}

Vector Gesture::getJointPoint( int body_index, JOINT joint ) {
	return _impl->getJointPoint( body_index, joint );
}


Vector Gesture::getJointDepthPoint( int body_index, Gesture::JOINT joint ) {
	return _impl->getJointDepthPoint( body_index, joint );
}

Vector Gesture::getJointColorPoint( int body_index, Gesture::JOINT joint ) {
	return _impl->getJointColorPoint( body_index, joint );
}

