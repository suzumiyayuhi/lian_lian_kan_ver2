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
	SEVERCONNECTOR();										//�޲ι��캯��
	~SEVERCONNECTOR();										//��������
	void connectServer();									//�򿪵�������������
	std::string receiveFromSever();							//��ȡ���������������
	int sendToSever(std::string str, std::string lobbyNum);	//���������Լ�������������
};
