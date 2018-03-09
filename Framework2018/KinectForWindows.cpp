#include "KinectForWindows.h"
#include "Application.h"
#include <assert.h>
#ifdef KINECT

KinectForWindowsPtr KinectForWindows::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< KinectForWindows >( fw->getTask( getTag( ) ) );
}

KinectForWindows::KinectForWindows( ) {
}

KinectForWindows::~KinectForWindows( ) {
}

//������
void KinectForWindows::initialize( ) {
	initializeKinectSensor( );
	initializeBodyReader( );
	//�^�C�}�̏�����
	_timer = 0;
}

void KinectForWindows::update( ) {
	//10�t���[���Ɉ�x�X�V����悤�ɂ���
	if ( _timer % 10 == 0 ) {
		getBodyFrame( );
		_timer = 0;
	}
	_timer++;
}

void KinectForWindows::finalize( ){
	_kinect->Close( );
	_kinect->Release( );
	_bodyFrameReader->Release( );
}

bool KinectForWindows::isExistJointPoint( int body_index ) {
	return _bodyJointData[ body_index ].size( ) > 0;
}

Vector KinectForWindows::getJointPoint(int body_index, JointType joint_type) {
	double x = ( double )_bodyJointData[body_index][joint_type].Position.X;
	double y = ( double )_bodyJointData[body_index][joint_type].Position.Y * -1;
	double z = ( double )_bodyJointData[body_index][joint_type].Position.Z;
	return Vector( x , y, z );
}

Vector KinectForWindows::getJointDepthPoint( int body_index, JointType joint_type ) {
	// �J�������W�n��Depth���W�n�ɕϊ�����
	ICoordinateMapper* mapper;
	errorCheck( _kinect->get_CoordinateMapper( &mapper ) );
	
	DepthSpacePoint point;
	mapper->MapCameraPointToDepthSpace( _bodyJointData[ body_index ][ joint_type ].Position, &point );
	return Vector( point.X, point.Y );
}

void KinectForWindows::initializeKinectSensor( ) {
	errorCheck( ::GetDefaultKinectSensor( &_kinect ) );//�Z���T�[�̎擾
	errorCheck( _kinect->Open( ) );// Kinect�Z���T�[�𓮍삳����

	// Kinect�Z���T�[�����������ǂ�����Ԃ��擾����
	BOOLEAN isOpen = false;
	errorCheck( _kinect->get_IsOpen( &isOpen ) );
	if ( !isOpen ) {
		errorCheck( S_FALSE );
	}
}

void KinectForWindows::initializeBodyReader( ) {
	IBodyFrameSource* bodyFrameSource = nullptr;
    errorCheck( _kinect->get_BodyFrameSource( &bodyFrameSource ) );
    errorCheck( bodyFrameSource->OpenReader( &_bodyFrameReader ) );
}

void KinectForWindows::getBodyFrame( ) {
	IBody* bodies[ BODY_COUNT ] = { 0 };

	IBodyFrame* bodyFrame;
	HRESULT result = _bodyFrameReader->AcquireLatestFrame( &bodyFrame );
	if ( result == S_OK ){
		bodyFrame->GetAndRefreshBodyData( BODY_COUNT, bodies );
	} else {
		return;
	}

	//�{�f�B�f�[�^�ɕϊ�
	for ( int i = 0; i < BODY_COUNT; i++ ){
		if ( _bodyJointData[ i ].size( ) > 0 ) {
			_bodyJointData[ i ].clear( );
		}

		IBody* body = bodies[ i ];
		if ( body == nullptr ){
			continue;
		}
		BOOLEAN isTracked = false;
		body->get_IsTracked( &isTracked );
		if ( !isTracked ) {
			continue;
		}

		//�W���C���g���W���C���g�^�C�v���L�[�ɂ��ĕۑ�
		Joint joints[JointType::JointType_Count];
		body->GetJoints( JointType::JointType_Count, joints );
		for ( int j = 0; j < JointType::JointType_Count; j++ ) {
			Joint joint = joints[ j ];
			_bodyJointData[ i ][ joint.JointType ] = joint;
		}
	}

	bodyFrame->Release( );
}

bool KinectForWindows::errorCheck(HRESULT result) {
	assert( S_OK == result );
	return S_OK == result;
}

#endif