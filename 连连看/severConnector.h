#pragma once
#include<Windows.h>
#include<iostream>
#include<strstream>
#include<stdio.h>
#include<tchar.h>
#include<string.h>
#include<string>
#include<iostream>
#include<fstream>
#include<strstream>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

#define SEVER_ADDRESS		"173.254.200.92"
#define SEVER_PORT			31022
#define MAX_MESSAGE_SIZE	2000

struct SEVERCONNECTOR
{
	WSADATA wsadata;
	sockaddr_in sockAddr;
	SOCKET severSocket;
	SOCKET clientSocket;
	//////////////////////////////////////////////////////////
	SEVERCONNECTOR();										//无参构造函数
	~SEVERCONNECTOR();										//析构函数
	void connectServer();									//打开到服务器的连接
	std::string receiveFromSever();							//获取服务器传输的数据
	int sendToSever(std::string str, std::string lobbyNum);	//发送数据以及房间号至服务端
};
