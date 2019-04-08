#pragma once
#include <windows.h>
#include<iostream>
#include<string>
#include"tool.h"
#include"mysql.h"
struct DBCONNECTOR
{
	//成员函数
	bool Open();																//打开连接
	void Close();																//关闭连接
	bool IsLogin(wchar_t *account,wchar_t *password);							//判断登入用户名密码
	bool RunRegister(wchar_t *account, wchar_t *password);						//注册功能函数
	bool ScoreSave(wchar_t *account,int score);									//游戏成绩上传
	std::string DBCONNECTOR::GetSelect(std::string tabName);					//取得指定表中数据
	std::string DBCONNECTOR::GetSelect(std::string tabName,std::string order);	//取得指定表中数据，附带排序
	std::string DBCONNECTOR::GetRankSelect();									//取得排行榜表中数据
	/////////////////////////////////////////////
	//成员变量
	MYSQL			mysql;
	MYSQL_RES		*result;
};