#include "Ntp.h"
#include "Drawer.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

const char* HOST = "ntp.jst.mfeed.ad.jp";
const unsigned short PORT = 123;
const unsigned long NTP_TIME = 2208988800U;
const int BIT_NUM = 32;

const timeval TIMEOUT = { 1, 0 };//��M�ҋ@ �b,ms

//-------------- Option ---------------//
const UINT32 LI = 0;     //�x��( 0:�Ȃ� )
const UINT32 VN = 3;     //version( 3:�ʏ��NTP )
const UINT32 MODE = 3;   //���샂�[�h( 3:�N���C�A���g )
const UINT32 STRATUM = 1;//�K�w(�T�[�o�[�ɂ��)
const UINT32 POLL = 0;   //�Ԋu?
const UINT32 PREC = 0;  //���x

const UINT32 OPTION = htonl(//�v32bit
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
	{//������
		WSADATA data = WSADATA( );
		WSAStartup( MAKEWORD( 2, 0 ), &data );
	}

	//�\�P�b�g����
	_socket = ( int )socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( _socket < 0 ) {
		return;
	}

    // �z�X�g�̏����擾
	hostent *servhost = gethostbyname( HOST );
    if( servhost == NULL){
        perror( HOST );
		return;
    }

    //�\���̂ɏ����i�[
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
// NTP �T�[�o����̎����擾
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
	//Option���M
	int send_byte = send( _socket, ( char* )&data, sizeof( PACKET ), 0 );
	if ( send_byte <= 0 ) {
		return 0;
	}

	//�ҋ@
	fd_set read = fd_set( );
	FD_ZERO( &read );
	FD_SET( _socket, &read );
	if ( select( _socket + 1, &read, NULL, NULL, &TIMEOUT ) != 1 ) {
		OutputDebugString( "Ntp:�^�C���A�E�g\n" );
		return 0;
	}
	FD_CLR( _socket, &read );

	//��M
	int recv_byte = recv( _socket, ( char* )&data, sizeof( PACKET ), 0 );
	if ( recv_byte <= 0 ) {
		return 0;
	}

	//���Ԍv�Z
	unsigned long time_s = ntohl( data.timestamp_transmit_s ) - NTP_TIME;
	unsigned long time_f = ntohl( data.timestamp_transmit_f );
	
#if 0//�m�F�p
	time_t check_t = time_s;
	char buf[ 128 ];
	ctime_s( buf, 128, ( const time_t* )&check_t );
	OutputDebugString( buf );
	exit( 1 );
#endif
	//1/1000�b���v�Z
	unsigned long mili_s = 0;
	for ( int i = 0; i < BIT_NUM; i++ ) {
		unsigned long flag = 1 << ( BIT_NUM - i - 1 );
		if ( flag & time_f ) {
			mili_s += ( int )( pow( 2, -( i + 1 ) ) * 1000 );
		}
	}


	//���l�����󂯂�1000�~���b��ǉ�
	unsigned long long time = ( unsigned long long )time_s * 1000 + mili_s;

	_before_count = GetTickCount( );
	_before_time = time;
	return _before_time;
}

unsigned long long Ntp::getTimeCalc( ) const {
	int now_count = GetTickCount( );
	return _before_time + ( now_count - _before_count );
}
