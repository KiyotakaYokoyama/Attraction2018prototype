#pragma once

class Data {
public:
	Data( ) { }
	virtual ~Data( ) { }
public:
	virtual void * getPtr( ) = 0;
	virtual int getSize( ) = 0;
};