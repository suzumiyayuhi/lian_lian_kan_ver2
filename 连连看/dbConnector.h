#pragma once
#include <windows.h>
#include<iostream>
#include<string>
#include"tool.h"
#include"mysql.h"
struct DBCONNECTOR
{
	//��Ա����
	bool Open();																//������
	void Close();																//�ر�����
	bool IsLogin(wchar_t *account,wchar_t *password);							//�жϵ����û�������
	bool RunRegister(wchar_t *account, wchar_t *password);						//ע�Ṧ�ܺ���
	bool ScoreSave(wchar_t *account,int score);									//��Ϸ�ɼ��ϴ�
	std::string DBCONNECTOR::GetSelect(std::string tabName);					//ȡ��ָ����������
	std::string DBCONNECTOR::GetSelect(std::string tabName,std::string order);	//ȡ��ָ���������ݣ���������
	std::string DBCONNECTOR::GetRankSelect();									//ȡ�����а��������
	/////////////////////////////////////////////
	//��Ա����
	MYSQL			mysql;
	MYSQL_RES		*result;
};