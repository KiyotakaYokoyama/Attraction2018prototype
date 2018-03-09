#pragma once
#include "WinSock2Inc.h"
#include "../MySQL/include/mysql.h"
#include <string>
#include <vector>

const std::string TABLE_MUSIC        = "CypherSync";
const std::string TABLE_ONLINE       = "OnlineList";
const std::string TABLE_MACHING_NEXT = "MatchingNext";
const std::string TABLE_MACHING_NOW  = "MatchingNow";
const std::string TABLE_AREA     = "Area";
const std::string COLUMN_TIME    = "time";
const std::string COLUMN_COUNT   = "count";
const std::string COLUMN_MUSIC   = "music";
const std::string COLUMN_MACHING = "matching";
const std::string COLUMN_ILLUST  = "Illust";

class DataBase {
public:
	DataBase( );
	virtual ~DataBase( );
public:
	//�擾 vector[ column(��) ][ row(�s) ]
	std::vector< std::vector< std::string > > selectTable( std::string table );      //�w�肵��table�擾
	std::vector< std::string > selectColumn( std::string table, std::string column );//table���̎w�肵������擾
	std::string selectField( std::string table, std::string column, int row = 0 );       //��,�s�ɂ�����̂��擾
	//�ǉ�
	void add( std::string table, std::string column, std::string value );
	void addColumn( std::string table, std::string column, std::string type );
	//�ύX
	void set( std::string table, std::string column, std::string value, std::string condition = "" );
	void copy( std::string to_table, std::string from_table, std::string to_column, std::string from_column );
	//�폜
	void erase( std::string table, std::string column, std::string value );
	void erase( std::string table, std::string column );
private:
	void open( );
	void close( );
private:
	MYSQL* _mysql;
	MYSQL_RES* _res;
};
