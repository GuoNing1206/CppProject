#pragma once
#include <stdio.h>
#include <string>
#include <afxsock.h>
#include "mysql.h"

using namespace std;

class VspdCToMySQL 
{
private:
  //����
  MYSQL m_MySQL;

public:  
  VspdCToMySQL();
  ~VspdCToMySQL();

	/*---------------------------------------------------------------------------
		2015/05/28 ����: ��ʼ�����ݿ� �������ݿ� �����ַ���

		��ڲ�����
			host ��MYSQL������IP
			port:���ݿ�˿�
			Db�����ݿ�����
			user�����ݿ��û�
			passwd�����ݿ��û�������
			charset��ϣ��ʹ�õ��ַ���
			Msg:���ص���Ϣ������������Ϣ

		���ڲ�����
			int ��0��ʾ�ɹ���1��ʾʧ��

	---------------------------------------------------------------------------*/
  int ConnMySQL(char* host, char* port, char* Db, char* user, char* passwd, char* charset, char* Msg);
 
	/*---------------------------------------------------------------------------
		2015/05/28 ����: ��ѯ����
		��ڲ�����
			SQL����ѯ��SQL���
			Cnum:��ѯ������
			Msg:���ص���Ϣ������������Ϣ

		���ڲ�����
			string ׼�����÷��ص����ݣ�������¼����\n����,�����λ��\t����
			��� ���صĳ��ȣ� 0�����ʾ����

	---------------------------------------------------------------------------*/
  CString SelectData(char* SQL, char* Msg);
      
  /*
  ��Ҫ���ܣ�
  ��������
      
  ��ڲ���
  SQL����ѯ��SQL���
  Msg:���ص���Ϣ������������Ϣ
 
  ���ڲ�����
  int ��0��ʾ�ɹ���1��ʾʧ��
  */
  int InsertData(char* SQL, char* Msg);
 
  /*
  ��Ҫ���ܣ�
  �޸�����
      
  ��ڲ���
  SQL����ѯ��SQL���
  Msg:���ص���Ϣ������������Ϣ
 
  ���ڲ�����
  int ��0��ʾ�ɹ���1��ʾʧ��
  */
  int UpdateData(char* SQL, char* Msg);
 
 
  /*
  ��Ҫ���ܣ�
  ɾ������
      
  ��ڲ���
  SQL����ѯ��SQL���
  Msg:���ص���Ϣ������������Ϣ
 
  ���ڲ�����
  int ��0��ʾ�ɹ���1��ʾʧ��
  */
  int DeleteData(char* SQL, char* Msg);
      
  /*
  ��Ҫ���ܣ�
  �ر����ݿ�����
  */
  void CloseMySQLConn();
 
};