#include "StdAfx.h"
#include "VspdCToMySQL.h"


VspdCToMySQL::VspdCToMySQL()
{
}

VspdCToMySQL::~VspdCToMySQL()
{
}

//初始化数据
int VspdCToMySQL::ConnMySQL(char* host, char* port, char* Db, char* user, char* passwd, char* charset, char* Msg)
{
	if(mysql_init(&m_MySQL) == NULL)
	{
		Msg = "inital mysql handle error";
		return 1;
	}    

	if (mysql_real_connect(&m_MySQL,host,user,passwd,Db,0,NULL,0) == NULL)
	{
		Msg = "Failed to connect to database: Error";
		return 1;
	}    

	if(mysql_set_character_set(&m_MySQL,"GBK") != 0)
	{
		Msg = "mysql_set_character_set Error";
		return 1;
	}
	return 0;
}

//查询数据
CString VspdCToMySQL::SelectData(char* SQL, char* Msg)
{
	MYSQL_ROW m_row;
	MYSQL_RES *m_res;
	char sql[2048];
	sprintf(sql,SQL);
	int rnum = 0;

	if(mysql_query(&m_MySQL,sql) != 0)
	{
		Msg = "select ps_info Error";
		return "";
	}
	m_res = mysql_store_result(&m_MySQL);

	if(m_res==NULL)
	{
		Msg = "select username Error";
		return "";
	}
	CString str("");
	while(m_row = mysql_fetch_row(m_res))
	{
		for(int i = 0;i < m_res->field_count;i++)
		{
			str += m_row[i];
			str += "\t";
		}
		str += "\r\n";             
		rnum++;
	}

	mysql_free_result(m_res);

	str += mysql_get_host_info(&m_MySQL);

	return str;
}

//插入数据
int VspdCToMySQL::InsertData(char * SQL,char * Msg)
{
	char sql[2048];
	sprintf(sql,SQL);
	if(mysql_query(&m_MySQL,sql) != 0)
	{
		Msg = "Insert Data Error";
		return 1;
	}
	return 0;
}

//更新数据
int VspdCToMySQL::UpdateData(char * SQL,char * Msg)
{
	char sql[2048];
	sprintf(sql,SQL);
	if(mysql_query(&m_MySQL,sql) != 0)
	{
		Msg = "Update Data Error";
		return 1;
	}
	return 0;
}

//删除数据
int VspdCToMySQL::DeleteData(char * SQL,char * Msg)
{
	char sql[2048];
	sprintf(sql,SQL);
	if(mysql_query(&m_MySQL,sql) != 0)
	{
		Msg = "Delete Data error";
		return 1;
	}
	return 0;
}

//关闭数据库连接
void VspdCToMySQL::CloseMySQLConn()
{
	mysql_close(&m_MySQL);
}