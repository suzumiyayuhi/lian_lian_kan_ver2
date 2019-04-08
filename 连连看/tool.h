#pragma once
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include<string>
#include<iostream>
#include<sstream>
#include <windows.h>
std::string t_WcharToString(wchar_t *a);
wchar_t* t_StringToWchar(std::string str);
wchar_t* t_IntToWchar(int target);
std::string t_TransChessSend(int chessMap[10][16]);
void t_TransChessInf(std::string str, int res[10][16]);
std::string receiveFrom(const char* ipAddr);
void sendTo(const char* ipAddr, std::string str);
std::string clientAct(std::string serverIp, std::string sendStr);
std::string serverAct(std::string sendStr);