#pragma once
#include<string>
#include<iostream>
#include<strstream>
#define MAX_PLAYER_SIZE 2			//同一房间最大玩家数

struct LOBBY
{
	int lobbyNum;								//房间编号
	std::string playerIpAddr[MAX_PLAYER_SIZE];	//储存房间内玩家ip编号


};