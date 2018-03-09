//2017.12�@Kinect for Windows SDK 2.0
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
	Vector getJointPoint( int body_index, JointType joint_type );//�L�l�N�g���S���O�ɂ����Ƃ��̍��W�擾
	Vector getJointDepthPoint( int body_index, JointType joint_type );
	#endif

private:
	void initializeKinectSensor( );//�L�l�N�g�Z���T�[�̏�����
	void initializeBodyReader( );//�{�f�B���[�_�[�̏�����
	void getBodyFrame( );//�{�f�B�t���[���̎擾
	
	#ifdef KINECT
	bool errorCheck( HRESULT result);
	#endif

private:
	#ifdef KINECT
	IKinectSensor* _kinect;
	//�{�f�B�n�ϐ�
	IBodyFrameReader* _bodyFrameReader;
	std::array< std::map< JointType, Joint >, BODY_COUNT > _bodyJointData;//�e�{�f�B�̃W���C���g�f�[�^
	#endif
	int _timer;
};
