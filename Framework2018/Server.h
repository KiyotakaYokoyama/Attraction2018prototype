#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include <array>

PTR( Server );
PTR( Data );

class Server : public Task {
public:
	static std::string getTag( ) { return "SERVER"; }
	static ServerPtr getTask( );
public:
	static const int MAX_MACHINES = 20;
public:
	Server( DataPtr data );
	virtual ~Server( );
public:
	virtual void update( );
	void saveIP( );
	std::string getClientIPStr( int index ) const;
	std::string getServerIPStr( ) const;
	void sendUdp( DataPtr data );
	int getRecievingIdx( ) const;
private:
	void listenForAcceptNewClient( );
	void listenForPackets( );
	void lostMachine( );
	void recieveTcp( );
	void sendCondition( );
private:
	std::array< int, MAX_MACHINES > _machine;
	int _udp_handle;
	DataPtr _data;
	int _target_michine;
	int _recieving_idx;
	int _condition_time;
};