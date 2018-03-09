#include "DataBase.h"
#include <string>
#include <vector>

//通信規格:IP:ポート
const char* SQL_HOST     = "160.16.140.103";//接続先  
const char* SQL_USER     = "sqluser";       //mysqlユーザー名
const char* SQL_PASSWORD = "pR3zwa6fR+";    //パスワード
const char* SQL_DATABASE = "test";          //データベース名

#pragma comment( lib, "libmysql.lib" )

DataBase::DataBase( ) {
	open( );
}


DataBase::~DataBase( ) {
	close( );
}

void DataBase::open( ) {
	//初期化
	_mysql = mysql_init( NULL );

	//MySQL&DataBaseに接続
	if ( mysql_real_connect( _mysql, SQL_HOST, SQL_USER, SQL_PASSWORD, SQL_DATABASE, MYSQL_PORT, NULL, 0 ) == NULL ) {
		//接続できなかった
		OutputDebugString( "MySQLの接続に失敗\n" );
		return;
	}
}

void DataBase::close( ) {
	//接続を切断
	mysql_free_result( _res );
	mysql_close( _mysql );
}

std::vector< std::vector< std::string > > DataBase::selectTable( std::string table ) {
	std::vector< std::vector< std::string > > result;
	//-----クエリ生成------//
	std::string query = "select * from " + table;
	
	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//MySQLからデータを取得
	_res = mysql_store_result( _mysql );
	if ( _res == NULL ) {
		//取得できない
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//行数
	int num_rows = ( int )mysql_num_rows( _res );
	//列数
	int num_column = ( int )mysql_num_fields( _res );
	//フィールド取得( セル )
	MYSQL_FIELD* field = mysql_fetch_fields( _res );

	result.resize( num_column );
	//MYSQL_FIELD tes;
	for ( int i = 0; i < num_column; i++ ) {
		result[ i ].resize( num_rows + 1 );
		result[ i ][ 0 ] = field[ i ].name;
	}

	//ロウ(行)取得
	MYSQL_ROW row = mysql_fetch_row( _res );
	int row_count = 0;
	while ( row != NULL ) {
		row_count++;

		for ( int i = 0; i < num_column; i++ ) {
			result[ i ][ row_count ] = row[ i ];
		}

		row = mysql_fetch_row( _res );
	}

	return result;
}

std::vector< std::string > DataBase::selectColumn( std::string table, std::string column ) {
	std::vector< std::string > result;
	//-----クエリ生成------//
	std::string query = "select " + column + " from " + table;
	
	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//MySQLからデータを取得
	_res = mysql_store_result( _mysql );
	if ( _res == NULL ) {
		//取得できない
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//行数
	int num_rows = ( int )mysql_num_rows( _res );
	//列数
	int num_column = ( int )mysql_num_fields( _res );
	//フィールド取得( セル )
	MYSQL_FIELD* field = mysql_fetch_fields( _res );

	result.resize( num_column );
	//MYSQL_FIELD tes;
	for ( int i = 0; i < num_column; i++ ) {
		result.resize( num_rows + 1 );
		result[ 0 ] = field[ i ].name;
	}

	//ロウ(行)取得
	MYSQL_ROW row = mysql_fetch_row( _res );
	int row_count = 0;
	while ( row != NULL ) {
		row_count++;
		if ( row[ 0 ] != NULL ) {
			result[ row_count ] = row[ 0 ];
		} else {
			result[ row_count ] = "";
		}
		row = mysql_fetch_row( _res );
	}

	return result;
}


std::string DataBase::selectField( std::string table, std::string column, int row ) {
	std::vector< std::string > tmp = selectColumn( table, column );
	if ( row >= ( int )tmp.size( ) ) {
		return "";
	}
	return tmp[ row ];
}

void DataBase::add( std::string table, std::string column, std::string value ) {
	//INSERT INTO テーブル名 (列名) VALUES (値)

	//-----クエリ生成------//
	std::string query = "insert into " + table + " ("+ column + ") VALUES ('" + value + "')";
	
	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::addColumn( std::string table, std::string column, std::string type ) {
	
	//INSERT INTO テーブル名 (列名) VALUES (値)

	//-----クエリ生成------//
	std::string query = "alter table " + table + " add column " + column + " " + type;
	
	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::set( std::string table, std::string column, std::string value, std::string condition ) {
	//UPDATE テーブル名 SET 列名 = データ, 列名 = データ ,.... ;

	//-----クエリ生成------//
	std::string query = "update " + table + " set " + column + " ='" + value + "'";//
	if ( condition != "" ) {
		query += " where " + condition;
	}
	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return ;
	}
}

void DataBase::copy( std::string to_table, std::string from_table, std::string to_column, std::string from_column ) {
	//update table set ( column ) = ( select column from table )
	std::string query = "insert into " + to_table + " select * from " + from_table;
	//std::string query = "update " + to_table + " set " + to_column + " = select " + from_column + " from " + from_table;

	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::erase( std::string table, std::string column, std::string condition ) {
	//DELETE FROM `test`.`Matching` WHERE  `IP`='a' LIMIT 1;
	//-----クエリ生成------//
	std::string query = "delete from " + table + " WHERE " + condition;

	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::erase( std::string table, std::string column ) {
	//DELETE FROM `test`.`Matching` WHERE  `IP`='a' LIMIT 1;
	//-----クエリ生成------//
	std::string query = "alter table " + table + " drop column " + column;

	//-----クエリ送信-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//送信に失敗
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}