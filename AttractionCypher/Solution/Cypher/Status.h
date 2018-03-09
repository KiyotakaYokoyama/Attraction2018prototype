#pragma once
class Status {
public:
	Status( );
	virtual ~Status( );
public:
	int getLevel( ) const;
private:
	int _level;
};

