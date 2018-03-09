#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include <string>

PTR( Client );
PTR( Device );
PTR( Sound );
PTR( Inputter );
PTR( Data );

class Client : public Task {
public:
	static std::string getTag( ) { return "CLIENT"; }
	static ClientPtr getTask( );
public:
	Client( DataPtr data_udp, DataPtr data_tcp );
	virtual ~Client( );
public:
	virtual void update( );
public:
	std::string getPhase( ) const;
	void sendTcp( DataPtr data );
	bool isRecievingUDP( ) const;
	std::string getServerIPStr( ) const;
	std::string getClientIPStr( ) const;
private:
	enum PHASE {
		PHASE_READY,
		PHASE_CONNECTING,
	};
private:
	void updateReady( );
	void updateConnecting( );
	bool loadIP( );
	bool connect( );
	void recieveUdp( );
	void recieveTcp( );
private:
	DataPtr _data_udp;
	DataPtr _data_tcp;
	PHASE _phase;
	unsigned char _ip[ 4 ];
	int _tcp_handle;
	int _udp_handle;
	int _network_state_time;
	bool _recieving_udp;
};

