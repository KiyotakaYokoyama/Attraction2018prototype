#include "Ntp.h"
#include "Drawer.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

const char* HOST = "ntp.jst.mfeed.ad.jp";
const unsigned short PORT = 123;
const unsigned long NTP_TIME = 2208988800U;
const int BIT_NUM = 32;

const timeval TIMEOUT = { 1, 0 };//受信待機 秒,ms

//-------------- Option ---------------//
const UINT32 LI = 0;     //警告( 0:なし )
const UINT32 VN = 3;     //version( 3:通常のNTP )
const UINT32 MODE = 3;   //動作モード( 3:クライアント )
const UINT32 STRATUM = 1;//階層(サーバーによる)
const UINT32 POLL = 0;   //間隔?
const UINT32 PREC = 0;  //精度

const UINT32 OPTION = htonl(//計32bit
	( LI      << 30 ) |//2bit
	( VN      << 27 ) |//3bit
	( MODE    << 24 ) |//3bit
	( STRATUM << 16 ) |//8bit
	( POLL    << 8  ) |//8bit
	( PREC & 0xff ) ); //8bit

Ntp::Ntp( ) :
_connect( false ),
_before_count( 0 ),
_before_time( 0 ) {
	open( );
}


Ntp::~Ntp( ) {
	closesocket( _socket );
	WSACleanup( );
}

void Ntp::open( ) {
	{//初期化
		WSADATA data = WSADATA( );
		WSAStartup( MAKEWORD( 2, 0 ), &data );
	}

	//ソケット生成
	_socket = ( int )socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( _socket < 0 ) {
		return;
	}

    // ホストの情報を取得
	hostent *servhost = gethostbyname( HOST );
    if( servhost == NULL){
        perror( HOST );
		return;
    }

    //構造体に情報を格納
	sockaddr_in addr = sockaddr_in( );
    addr.sin_family = AF_INET;
	addr.sin_addr = *( IN_ADDR* )servhost->h_addr_list[ 0 ];
	addr.sin_port = htons( PORT );

	if ( connect( _socket, ( sockaddr* )&addr, sizeof( addr ) ) < 0 ) {
		return;
	}
	_connect = true;
}


//-----------------------------------------------------------------
// NTP サーバからの時刻取得
//-----------------------------------------------------------------
unsigned long long Ntp::getTime( ) {
	if ( !_connect ) {
		open( );
		if ( !_connect ) {
			return 0;
		}
	}
	if ( _before_time > 0 ) {
		return getTimeCalc( );
	} else {
		return getTimeServer( );
	}
}
unsigned long long Ntp::getTimeServer( ) {
	PACKET data = PACKET( );
	data.option = OPTION;
	data.delay = htonl( 1 << 16 );
	data.disp  = htonl( 1 << 16 );
	//Option送信
	int send_byte = send( _socket, ( char* )&data, sizeof( PACKET ), 0 );
	if ( send_byte <= 0 ) {
		return 0;
	}

	//待機
	fd_set read = fd_set( );
	FD_ZERO( &read );
	FD_SET( _socket, &read );
	if ( select( _socket + 1, &read, NULL, NULL, &TIMEOUT ) != 1 ) {
		OutputDebugString( "Ntp:タイムアウト\n" );
		return 0;
	}
	FD_CLR( _socket, &read );

	//受信
	int recv_byte = recv( _socket, ( char* )&data, sizeof( PACKET ), 0 );
	if ( recv_byte <= 0 ) {
		return 0;
	}

	//時間計算
	unsigned long time_s = ntohl( data.timestamp_transmit_s ) - NTP_TIME;
	unsigned long time_f = ntohl( data.timestamp_transmit_f );
	
#if 0//確認用
	time_t check_t = time_s;
	char buf[ 128 ];
	ctime_s( buf, 128, ( const time_t* )&check_t );
	OutputDebugString( buf );
	exit( 1 );
#endif
	//1/1000秒を計算
	unsigned long mili_s = 0;
	for ( int i = 0; i < BIT_NUM; i++ ) {
		unsigned long flag = 1 << ( BIT_NUM - i - 1 );
		if ( flag & time_f ) {
			mili_s += ( int )( pow( 2, -( i + 1 ) ) * 1000 );
		}
	}


	//下四桁を空けて1000ミリ秒を追加
	unsigned long long time = ( unsigned long long )time_s * 1000 + mili_s;

	_before_count = GetTickCount( );
	_before_time = time;
	return _before_time;
}

unsigned long long Ntp::getTimeCalc( ) const {
	int now_count = GetTickCount( );
	return _before_time + ( now_count - _before_count );
}
