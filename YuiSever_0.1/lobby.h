#pragma once
#include<string>
#include<iostream>
#include<strstream>
#define MAX_PLAYER_SIZE 2			//ͬһ������������

struct LOBBY
{
	int lobbyNum;								//������
	std::string playerIpAddr[MAX_PLAYER_SIZE];	//���淿�������ip���


};