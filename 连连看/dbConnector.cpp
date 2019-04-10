#include"dbConnector.h"
#include<strstream>
#define USER_NAME	"lianuser"
#define USER_PWD	"lianUserPassword"
#define DB_ACCOUNT	"liandb"
#define DATABASE_HOST "173.254.200.92"

bool DBCONNECTOR::Open()
{

	mysql_library_init(0, NULL, NULL);														//初始化数据库
	mysql_init(&mysql);																		//初始化数据结构
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");									//设置字符集
	mysql_real_connect(&mysql, DATABASE_HOST, USER_NAME, USER_PWD, DB_ACCOUNT, 3306, NULL, 0);	//数据库连接
	return	true;
}

void DBCONNECTOR::Close()
{
	mysql_free_result(result);
	mysql_close(&mysql);
	mysql_server_end();
}

bool DBCONNECTOR::IsLogin(wchar_t *account, wchar_t *password)
{
	bool					flag =	false;
	std::string				sqlstr;
	std::string				strAccount;
	std::string				strPassword;
	char *temAccount;
	char *temPassword;

	strAccount = t_WcharToString(account);
	strPassword = t_WcharToString(password);
	/*
	int dwNum = WideCharToMultiByte(CP_ACP, NULL, account, -1, NULL, 0, NULL, FALSE);
	temAccount = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, account, -1, temAccount, dwNum, NULL, FALSE);
	strAccount = temAccount;
	delete[]temAccount;

	dwNum = WideCharToMultiByte(CP_ACP, NULL, password, -1, NULL, 0, NULL, FALSE);
	temPassword = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, password, -1, temPassword, dwNum, NULL, FALSE);
	strPassword = temPassword;
	delete[]temPassword;
	*/
	if (strAccount.empty() || strPassword.empty())
		return false;
	//sqlstr = "SELECT * FROM liandb.user";
	sqlstr = "select * from liandb.user where account='" + strAccount+"'";
	Open();
	mysql_query(&mysql, sqlstr.c_str());
	result = mysql_store_result(&mysql);

	int rowcount = mysql_num_rows(result);		//行数
	int fieldrow = mysql_num_fields(result);	//字段数
	MYSQL_ROW row;

	row = mysql_fetch_row(result);

	while (row)
	{
		int i = 0;
		std::string tem1 = row[i];
		std::string tem2 = row[i + 1];

		if (tem1 == strAccount&&tem2 == strPassword)
			flag = true;
		row = mysql_fetch_row(result);

	}
	Close();
	return flag;
}

bool DBCONNECTOR::RunRegister(wchar_t *account,wchar_t *password)
{
	Open();
	std::string				sqlstr;
	char					*temAccount;
	char					*temPassword;
	std::string				strAccount;
	std::string				strPassword;
	
	int dwNum = WideCharToMultiByte(CP_ACP, NULL, account, -1, NULL, 0, NULL, FALSE);
	temAccount = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, account, -1, temAccount, dwNum, NULL, FALSE);
	strAccount = temAccount;
	delete[]temAccount;

	dwNum = WideCharToMultiByte(CP_ACP, NULL, password, -1, NULL, 0, NULL, FALSE);
	temPassword = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, password, -1, temPassword, dwNum, NULL, FALSE);
	strPassword = temPassword;
	delete[]temPassword;

	if (strAccount.empty() || strPassword.empty())
		return false;

	sqlstr = "INSERT INTO `liandb`.`user` (`account`, `pwd`) ";
	sqlstr += "VALUES ( '" + strAccount + "', '" + strPassword + "');";
	mysql_query(&mysql, sqlstr.c_str());
	Close();
	return true;
}

std::string DBCONNECTOR::GetSelect(std::string tabName)
{
	Open();
	std::string			sqlstr;
	std::stringstream	ss;
	std::string			result="";
	sqlstr = "SELECT * FROM `liandb`.`" + tabName + "`";
	mysql_query(&mysql, sqlstr.c_str());
	MYSQL_RES *res = mysql_store_result(&mysql);
	if (res == NULL)
		return "";
	else
	{
		int rowsNum = mysql_num_rows(res);
		int fieldsNum = mysql_num_fields(res);
		MYSQL_ROW row;
		row = mysql_fetch_row(res);
		while (row)
		{
			for (int i = 0; i != fieldsNum; i++)
			{
				result += row[i];
				result += "\t\t\t";
			}
			result += "\n";
			row = mysql_fetch_row(res);
		}
	}
	mysql_free_result(res);
	Close();
	return result;
}


std::string DBCONNECTOR::GetSelect(std::string tabName, std::string order)
{
	Open();
	std::string			sqlstr;
	std::stringstream	ss;
	std::string			result = "用户名\t\t\t得分\n";
	sqlstr = "SELECT * FROM liandb." + tabName + " order by score " + order + " limit 10";
	mysql_query(&mysql, sqlstr.c_str());
	MYSQL_RES *res = mysql_store_result(&mysql);
	if (res == NULL)
		return "还没有人上传得分QAQ";
	else
	{
		int rowsNum = mysql_num_rows(res);
		int fieldsNum = mysql_num_fields(res);
		MYSQL_ROW row;
		row = mysql_fetch_row(res);
		while (row)
		{
			for (int i = 0; i != fieldsNum; i++)
			{
				result += row[i];
				result += "\t\t\t";
			}
			result += "\n";
			row = mysql_fetch_row(res);
		}
	}
	mysql_free_result(res);
	Close();
	return result;
}

bool DBCONNECTOR::ScoreSave(wchar_t *account, int score)
{
	Open();
	std::string				sqlstr;
	std::string				strAccount;
	std::string				strScore;
	std::stringstream		ss;
	ss << score;
	strAccount = t_WcharToString(account);
	strScore = ss.str();
	sqlstr = "INSERT INTO `liandb`.`rank` (`account`, `score`) ";
	sqlstr += "VALUES ( '" + strAccount + "', '" + strScore + "');";
	int res = mysql_query(&mysql, sqlstr.c_str());
	ss.clear();
	Close();
	return true;
}