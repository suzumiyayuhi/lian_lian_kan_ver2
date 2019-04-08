#include"tool.h"
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

void t_TransChessInf(std::string str,int res[10][16])
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
				while (k < str.size() && str[k] != '!')
				{
					tem = tem*10 + str[k] - 48;
					k++;
				}
				res[i][j] = tem;
				k++;
			}
		}
	}
	
}


std::string receiveFrom(const char* ipAddr)//省事版
{
	WSADATA wsadata;

	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;							//IPV4地址族
	sockAddr.sin_addr.s_addr = inet_addr(ipAddr);			//指定服务器ip地址
	sockAddr.sin_port = htons(31022);						//指定服务器端口号

	bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
	listen(serverSocket, 10);

	//处理客户端请求
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
	std::string str;
	recv(clientSocket, (char*)&str, 1000, NULL);

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	return str;
}

void sendTo(const char* ipAddr, std::string str)
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	//服务端信息
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;						//IPV4地址族
	sockAddr.sin_addr.s_addr = inet_addr(ipAddr);		//指定服务器ip地址
	sockAddr.sin_port = htons(31022);					//指定服务器端口号
														////////////////////////////////////////////////

	SOCKET severSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(severSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));

	send(severSocket, str.c_str(), 1000, NULL);

	closesocket(severSocket);
	WSACleanup();
	return;
}

std::string clientAct(std::string serverIp,std::string sendStr)
{
	
	//加载套接字库
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != 0)
	{
		std::cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << std::endl;
		
	}

	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		std::cout << "WSADATA version is not correct!" << std::endl;
		
	}

	//创建套接字
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
		
	}

	//初始化服务器端地址族变量
	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr(serverIp.c_str());
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(31022);

	//连接服务器
	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (0 != iRet)
	{
		std::cout << "connect failed!" << std::endl;
		
	}

	

	//发送消息
	send(clientSocket, sendStr.c_str(), strlen(sendStr.c_str()) + 1, 0);

	//接收消息
	char recvBuf[2000];
	
	recv(clientSocket, recvBuf, 2001, 0);
	std::stringstream ss;
	ss << recvBuf;
	//清理
	closesocket(clientSocket);
	WSACleanup();
	std::string tem = ss.str();
	return tem;
}

std::string serverAct(std::string sendStr)
{
	//加载套接字库
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

	}

	//初始化服务器地址族变量
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

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
	char recvStr[2000];
	while (1)
	{
		SOCKADDR_IN clientAddr;
		int len = sizeof(SOCKADDR);

		SOCKET connSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &len);
		if (connSocket == INVALID_SOCKET)
		{
			std::cout << "accept(serverSocket, (SOCKADDR*)&clientAddr, &len) execute failed!" << std::endl;

		}
		send(connSocket, sendStr.c_str(), strlen(sendStr.c_str()) + 1, 0);

		
		recv(connSocket, recvStr, 2000, 0);
		closesocket(connSocket);
	}
	
	
	return recvStr;
}