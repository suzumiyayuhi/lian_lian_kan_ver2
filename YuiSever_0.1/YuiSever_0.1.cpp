// YuiSever_0.1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;
#pragma comment(lib,"ws2_32.lib")

string EMPTYCHESS; //"16*10";
std::string t_WcharToString(wchar_t *a)
{
	int dwNum = WideCharToMultiByte(CP_ACP, NULL, a, -1, NULL, 0, NULL, FALSE);
	char *tem = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, a, -1, tem, dwNum, NULL, FALSE);
	std::string str = tem;
	delete[]tem;
	return str;
}

wchar_t* t_StringToWchar(std::string str)
{
	const char *temC = str.c_str();
	int len = MultiByteToWideChar(CP_OEMCP, 0, temC, strlen(temC) + 1, NULL, 0);
	wchar_t *wc = new wchar_t[len];
	MultiByteToWideChar(CP_OEMCP, 0, temC, strlen(temC) + 1, wc, len);

	return wc;
}

wchar_t* t_IntToWchar(int target)
{
	std::stringstream ss;
	ss << target;
	std::string tem;
	tem = ss.str();
	wchar_t* wc = t_StringToWchar(tem);
	ss.clear();
	return wc;
}

std::string t_TransChessSend(int chessMap[10][16])
{
	std::string str = "";
	std::stringstream ss;
	for (int i = 1; i != 10; i++)
	{
		for (int j = 1; j != 16; j++)
		{
			ss << chessMap[i][j] << "!";
		}
	}
	str = ss.str();
	return str;
}

void t_TransChessInf(std::string str, int res[10][16])
{
	int WIN = 77777;

	memset(res, 0, sizeof(res));
	if (str == "win")
	{
		res[0][0] = 77777;
		return;
	}
	else
	{
		int k = 0;
		for (int i = 1; i != 10; i++)
		{
			for (int j = 1; j != 16; j++)
			{

				int tem = 0;
				while (str[k] != '!'&&k < str.size())
				{
					tem = tem * 10 + str[k] - 48;
					k++;
				}
				res[i][j] = tem;
				k++;
			}
		}
	}

}

std::string serverAct(std::string sendStr)
{

	for (int xi = 0; xi != 160; xi++)
		EMPTYCHESS.append("0!");
	//加载套接字库
	string simpleTest[4] = { "","","","" };
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != 0)
	{
		std::cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << std::endl;;

	}

	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		std::cout << "WSADATA version is not correct!" << std::endl;
	}

	//创建套接字
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "serverSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
		std::cout << std::endl << std::endl;
	}

	//初始化服务器地址族变量
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(31022);

	//绑定
	iRet = bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		std::cout << "bind(serverSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) execute failed!" << std::endl;

	}

	//监听
	iRet = listen(serverSocket, 10);
	if (iRet == SOCKET_ERROR)
	{
		std::cout << "listen(serverSocket, 10) execute failed!" << std::endl;

	}

	//等待连接_接收_发送
	SOCKADDR_IN clientAddr;
	int len = sizeof(SOCKADDR);
	std::stringstream ss;

	while (1)
	{
		SOCKET connSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &len);

		char recvStr[2000];
		recv(connSocket, recvStr, 2000, 0);
		cout << recvStr << endl;
		string temAddr = inet_ntoa(clientAddr.sin_addr);
		if (recvStr[0] == 'c')
		{
			simpleTest[0] = "";
			simpleTest[1] = "";
			simpleTest[2] = "";
			simpleTest[3] = "";
			send(connSocket, "", 1, 0);
		}
		else
		{

			if (simpleTest[0] == "")
				simpleTest[0] = temAddr;
			if (simpleTest[0] != temAddr && simpleTest[1] == "")
				simpleTest[1] = temAddr;

			if (recvStr[0] == 'c')
			{
				send(connSocket, "0!", strlen("0!") + 1, 0);
				break;
			}
			ss << recvStr;
			if (temAddr == simpleTest[0])
			{
				simpleTest[2] = ss.str();
				sendStr = simpleTest[3];
			}
			else if (temAddr == simpleTest[1])
			{
				simpleTest[3] = ss.str();
				sendStr = simpleTest[2];
			}
			std::cout << temAddr << std::endl;
			if (sendStr == "str")
				sendStr = "";
			send(connSocket, sendStr.c_str(), strlen(sendStr.c_str()) + 1, 0);
			ss.clear();
			ss.str("");
		}
	}

	/*
	if (simpleTest[0] == "")
	simpleTest[0] = temAddr;
	if (simpleTest[0] != temAddr && simpleTest[1] == "")
	simpleTest[1] = temAddr;

	ss << recvStr;
	if (temAddr == simpleTest[0])
	{
	simpleTest[2] = ss.str();
	sendStr = simpleTest[3];
	}

	if (temAddr == simpleTest[1])
	{
	simpleTest[3] = ss.str();
	sendStr = simpleTest[2];
	}
	std::cout << temAddr << std::endl;
	send(connSocket, sendStr.c_str(), strlen(sendStr.c_str()) + 1, 0);
	ss.clear();
	ss.str("");
	*/
	return "";
}

int main()
{
	serverAct("0!1!1!1!1!1!1!1!1!");
	return 0;
}


