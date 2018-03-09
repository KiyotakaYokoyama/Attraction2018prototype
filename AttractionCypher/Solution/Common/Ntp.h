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
		UINT32 option;//�ݒ� 32bit
		UINT32 delay; //�x��( �ʐM�̒x��     )
		UINT32 disp;  //���U( �ʐM�̕s���肳 )
		UINT32 ref_id;//�Q�Ǝ��ʎq( �g�p���Ȃ� )

		UINT32 timestamp_ref_s;     //���[�J�����v�ŏI�C������(�b)
		UINT32 timestamp_ref_f;     //���[�J�����v�ŏI�C������(�~���b)

		UINT32 timestamp_origin_s;  //�T�[�o�[�֔��M��������(�b)
		UINT32 timestamp_origin_f;  //�T�[�o�[�֔��M��������(�~���b)

		UINT32 timestamp_receive_s; //�T�[�o�[����M��������(�b)
		UINT32 timestamp_receive_f; //�T�[�o�[����M��������(�~���b)

		UINT32 timestamp_transmit_s;//�T�[�o�[�����M��������(�b)
		UINT32 timestamp_transmit_f;//�T�[�o�[�����M��������(�~���b)
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