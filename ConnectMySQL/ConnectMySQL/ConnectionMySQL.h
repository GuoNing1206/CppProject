#pragma once

#define HAS_MYSQL 1

//if you use Microsoft SQL Server
#if defined(HAS_MSQL)
	#include <msql.h>
//you use mysql
#elif defined(HAS_MYSQL)
	#include <mysql.h>
#endif

class Result;
class Row;

#if defined(HAS_MSQL)
	#include <msql.h>
	#define T_RESULT m_result
	#define T_ROW    m_row
#elif defined(HAS_MYSQL)
	#include <mysql.h>
	#define T_RESULT MYSQL_RES
	#define T_ROW    MYSQL_ROW
#endif

class Connection
{
public:
	Connection(const char*,const char*);
	Connection(const char*,const char*,const char*,const char*);
	~Connection();

	void Close();
	void Connect(const char* host,const char* dataBase,
		const char* userID,const char* password);
	int GetAffectedRows() const;
	const char* GetError();
	int IsConnected() const;
	Result* Query(const char* sqlCommand);

private:
	int affectedRows;
#if defined(HAS_MSQL)
	int connection;
#elif defined(HAS_MYSQL)
	MYSQL mysql;
	MYSQL* connection;
#else
	#error No database defined.
#endif
};

/*===========================================================================

	功能:     2015/05/28

===========================================================================*/
class Result
{
public:
	Result(T_RESULT* );
	~Result();

	void Close();
	Row* GetCurrentRow();
	int GetRowCount();
	int Next();

private:
	int rowCount;
	T_RESULT* result;
	Row* currentRow;
};

/*===========================================================================

	功能:     2015/05/28

===========================================================================*/
class Row
{
public:
	Row(T_RESULT* ,T_ROW);
	~Row();

	char* GetField(int index);
	int GetFieldCount();
	int IsClosed() const;
	void Close();

private:
	T_RESULT* result;
	T_ROW fields;
};

/*===========================================================================

	功能:     2015/05/29

===========================================================================*/
class ConnectionMySQL
{
private:
	Integer m_AffectedRows;
	MYSQL m_MySQL;
	MYSQL* m_Connection;

public:
	ConnectionMySQL();
	~ConnectionMySQL();

	bool Connect(ConstCharPtr A_Host, ConstCharPtr A_DataBase,
		ConstCharPtr A_UserID, ConstCharPtr A_PassWord);
	bool IsConnected() const;
	void Close();
	
	Result* Query(ConstCharPtr A_SQL);

	Integer GetAffectedRows() const;
	ConstCharPtr GetError();

};//end ConnectionMySQL
