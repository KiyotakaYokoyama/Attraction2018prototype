#pragma once
#include "Task.h"
#include "mathmatics.h"
#include "smart_ptr.h"
#include <string>

PTR( Gesture );
PTR( GestureImpl );

class Gesture : public Task {
public:
	static const int MAX_BODY = 6;
	enum JOINT {
		JOINT_HEAD,
		JOINT_SPINE_SHOULDER,	
		JOINT_SHOULDER_RIGHT,	
		JOINT_SHOULDER_LEFT,	
		JOINT_HAND_RIGHT,
		JOINT_HAND_LEFT,
		JOINT_FOOT_RIGHT,
		JOINT_FOOT_LEFT,
		MAX_JOINT,
		JOINT_NONE,
	};
public:
	static std::string getTag( ) { return "GESTURE"; };
	static GesturePtr getTask( );
public:
	Gesture( );
	virtual ~Gesture( );
public:
	void initialize( );
	void finalize( );
	void update( );
	void updateBodyFrame( );
	bool isExistJointPoint( int body_index, JOINT joint );
	Vector getJointPoint( int body_index, JOINT joint );
	Vector getJointDepthPoint( int body_index, JOINT joint );
	Vector getJointColorPoint( int body_index, JOINT joint );
private:
	GestureImplPtr _impl;
};

