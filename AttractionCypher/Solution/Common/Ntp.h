#pragma once
#include "WinSock2Inc.h"

class Ntp {
public:
	Ntp( );
	virtual ~Ntp( );
public:
	unsigned long long getTime( );
private:
	struct PACKET {
		UINT32 option;//設定 32bit
		UINT32 delay; //遅延( 通信の遅さ     )
		UINT32 disp;  //分散( 通信の不安定さ )
		UINT32 ref_id;//参照識別子( 使用しない )

		UINT32 timestamp_ref_s;     //ローカル時計最終修正時刻(秒)
		UINT32 timestamp_ref_f;     //ローカル時計最終修正時刻(ミリ秒)

		UINT32 timestamp_origin_s;  //サーバーへ発信した時間(秒)
		UINT32 timestamp_origin_f;  //サーバーへ発信した時間(ミリ秒)

		UINT32 timestamp_receive_s; //サーバーが受信した時間(秒)
		UINT32 timestamp_receive_f; //サーバーが受信した時間(ミリ秒)

		UINT32 timestamp_transmit_s;//サーバーが発信した時間(秒)
		UINT32 timestamp_transmit_f;//サーバーが発信した時間(ミリ秒)
	};
private:
	void open( );
	unsigned long long getTimeServer( );
	unsigned long long getTimeCalc( ) const;
private:
	int _socket;
	bool _connect;
	unsigned long long _before_time;
	int _before_count;
};