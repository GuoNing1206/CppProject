#include "StdAfx.h"
#include "ConnectionMySQL.h"

/*===========================================================================

	功能:     2015/05/28

===========================================================================*/
/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
Connection::Connection(const char* host,const char* dataBase)
	:affectedRows(0)
{
#if defined(HAS_MSQL)
	connection=-1;
#elif defined(HAS_MYSQL)
	connection=(MYSQL*)NULL;
#else
	#error No database linked.
#endif

	Connect(host,dataBase,(const char*)NULL,(const char*)NULL);
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
Connection::Connection(const char* host,const char* dataBase,
	const char* userID,const char* password)
	:affectedRows(0)
{
#if defined(HAS_MSQL)
	connection=-1;
#elif defined(HAS_MYSQL)
	connection=(MYSQL*)NULL;
#else
	#error No database linked.
#endif

	Connect(host,dataBase,userID,password);
}

void Connection::Connect(const char* host,const char* dataBase,
	const char* userID,const char* password)
{
	int state = 0;

	if(IsConnected())
		throw "Connection has been established.";

#if defined(HAS_MSQL)
	connection = msqlConnect(host);
	state = msqlSelectDB(connection,dataBase);
#elif defined(HAS_MYSQL)
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql,host,userID,password,dataBase,0,NULL,0);
	if(mysql_set_character_set(&mysql,"GBK") != 0)
		state = -1;
#else
	#error No database linked.
#endif

	if(!IsConnected())
		throw GetError();

	if(state<0)
		throw GetError();
}

Connection::~Connection()
{
	if(IsConnected())
		Close();
}

void Connection::Close()
{
	if(!IsConnected())
		return;

#if defined HAS_MSQL
	msqlClose(connection);
	connection=-1;
#elif defined HAS_MYSQL
	mysql_close(connection);
	connection=(MYSQL*)NULL;
#else
#error No database linked
#endif
}

Result* Connection::Query(const char* sqlCommand)
{
	T_RESULT* result;
	int state;

	if(!IsConnected())
		throw "DataBase not connected.";

#if defined(HAS_MSQL)
	state = mysqlQuery(connection,sqlCommand);
#elif defined(HAS_MYSQL)
	state = mysql_query(connection,sqlCommand);
#else
	#error No dataBase Linked.
#endif

	if(state!=0)
		throw GetError();

#if defined(HAS_MSQL)
	result = msqlStoreResult();
#elif defined(HAS_MYSQL)
	result = mysql_store_result(connection);
#else
	#error No database linked.
#endif

	//if the result was null,it was an update or an error occurred
	if(result==(T_RESULT*)NULL)
	{
#if defined(HAS_MSQL)
		affectedRows=state;
#elif defined(HAS_MYSQL)
		int fieldCount=mysql_field_count(connection);
		if(fieldCount!=0)
			throw GetError();
		else
			affectedRows = (int)mysql_affected_rows(connection);
#else
#error No database linked.
#endif
		//return NULL for updates
		return (Result*)NULL;
	}
	//return a Result instance for queries
	return new Result(result);
}

int Connection::GetAffectedRows() const
{
	return affectedRows;
}

const char* Connection::GetError()
{
#if defined(HAS_MSQL)
	return msqlErrMsg;
#elif defined(HAS_MYSQL)
	if(IsConnected())
		return mysql_error(connection);
	else
		return mysql_error(&mysql);
#else
#error No database linked.
#endif
}

int Connection::IsConnected() const
{
#if defined(HAS_MSQL)
	return connection>0;
#elif defined(HAS_MYSQL)
	return (connection!=NULL);
#else
#error No database linked.
#endif
}

/*===========================================================================

	功能:     2015/05/28

===========================================================================*/
Result::Result(T_RESULT* res)
	:rowCount(-1),result(res),currentRow((Row*)NULL)
{}

Result::~Result()
{
	Close();
}

int Result::Next()
{
	T_ROW row;

	if(result==(T_RESULT*)NULL)
		throw "Result set closed.";

#if defined(HAS_MSQL)
	row = msqlFetchRow(result);
#elif defined(HAS_MYSQL)
	row = mysql_fetch_row(result);
#else
	#error No database linked.
#endif

	if(!row)
	{
		currentRow = (Row*)NULL;
		return 0;
	}
	else
	{
		currentRow = new Row(result, row);
		return 1;
	}
}

Row* Result::GetCurrentRow()
{
	if(result==(T_RESULT*)NULL)
		throw "Result set closed.";

	return currentRow;
}

void Result::Close()
{
	if(result==(T_RESULT*)NULL)
		return;

#if defined(HAS_MSQL)
	msqlFreeResult(result);
#elif defined(HAS_MYSQL)
	mysql_free_result(result);
#else
#error No database linked.
#endif
	result=(T_RESULT*)NULL;
}

int Result::GetRowCount()
{
	if(result==(T_RESULT*)NULL)
		throw "Result set closed.";

#if defined(HAS_MSQL)
	rowCount = msqlNumRows(result);
#elif defined(HAS_MYSQL)
	rowCount = (int)mysql_num_rows(result);
#else
	#error No database linked.
#endif

	if(rowCount > -1)
		return rowCount;
	else
		return 0;
}

/*===========================================================================

	功能:     2015/05/28

===========================================================================*/
Row::Row(T_RESULT* res,T_ROW row)
	:result(res),fields(row)
{}

Row::~Row()
{
	if(!IsClosed())
		Close();
}

void Row::Close()
{
	if(IsClosed())
		throw "Row closed.";

	fields=(T_ROW)NULL;
	result=(T_RESULT*)NULL;
}

int Row::GetFieldCount()
{
	if(IsClosed())
		throw "Row closed.";

#if defined(HAS_MSQL)
	return msqlNumFields(result);
#elif defined(HAS_MYSQL)
	return mysql_num_fields(result);
#else
#error No database linked.
#endif
}

char* Row::GetField(int field)
{
	if(IsClosed())
		throw "Row closed.";

	if(field<0 || field>GetFieldCount()-1)
		throw "Field index out of bounds.";

	return fields[field];
}

int Row::IsClosed() const
{
	return (fields==(T_ROW)NULL);
}

/*===========================================================================

	功能:     2015/05/29

===========================================================================*/
/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
ConnectionMySQL::ConnectionMySQL()
	: m_AffectedRows(0), m_Connection(NULL)
{}

ConnectionMySQL::~ConnectionMySQL()
{
	if(IsConnected())
		Close();
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
bool ConnectionMySQL::Connect(ConstCharPtr A_Host, ConstCharPtr A_DataBase,
	ConstCharPtr A_UserID, ConstCharPtr A_PassWord)
{
	if(IsConnected())
	{
		throw "Connection has been established.";
		return false;
	}

	mysql_init(&m_MySQL);
	m_Connection = mysql_real_connect(
		&m_MySQL, A_Host, A_UserID, A_PassWord, A_DataBase, 0, NULL, 0);

	if(!IsConnected())
	{
		throw GetError();
		return false;
	}

	bool bSuccess = (mysql_set_character_set(&m_MySQL, "GBK") == 0);
	if(!bSuccess)
	{
		throw GetError();
		return false;
	}
	return true;
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
void ConnectionMySQL::Close()
{
	if(!IsConnected())
		return;

	mysql_close(m_Connection);
	m_Connection = (MYSQL*)NULL;
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
Result* ConnectionMySQL::Query(ConstCharPtr A_SQL)
{
	if(!IsConnected())
		throw "DataBase not connected.";

	Integer nState = mysql_query(m_Connection, A_SQL);

	if(nState != 0)
		throw GetError();

	MYSQL_RES* oResult = mysql_store_result(m_Connection);

	//if the result was null,it was an update or an error occurred
	if(oResult == (MYSQL_RES*)NULL)
	{
		Integer nFieldCount = mysql_field_count(m_Connection);
		if(nFieldCount != 0)
			throw GetError();
		else
			m_AffectedRows = (Integer)mysql_affected_rows(m_Connection);

		//return NULL for updates
		return (Result*)NULL;
	}
	//return a Result instance for queries
	return new Result(oResult);
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
Integer ConnectionMySQL::GetAffectedRows() const
{
	return m_AffectedRows;
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
ConstCharPtr ConnectionMySQL::GetError()
{
	if(IsConnected())
		return mysql_error(m_Connection);
	else
		return mysql_error(&m_MySQL);
}

/*---------------------------------------------------------------------------
	2015/05/29 功能: 
---------------------------------------------------------------------------*/
bool ConnectionMySQL::IsConnected() const
{
	return (m_Connection != NULL);
}
