#include"severConnector.h"


SEVERCONNECTOR::SEVERCONNECTOR()
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_addr.s_addr = inet_addr(SEVER_ADDRESS);
	sockAddr.sin_port = SEVER_PORT;

	severSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(severSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
}

SEVERCONNECTOR::~SEVERCONNECTOR()
{
	closesocket(severSocket);
	WSACleanup();
}

void SEVERCONNECTOR::connectServer()
{
	severSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(severSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
}

std::string SEVERCONNECTOR::receiveFromSever()
{
	std::string str;
	recv(severSocket, (char*)&str, MAX_MESSAGE_SIZE, NULL);
	return str;
}

int SEVERCONNECTOR::sendToSever(std::string str, std::string lobbyNum)
{
	std::string tem = str + "\t" + lobbyNum;
	send(severSocket, tem.c_str(), MAX_MESSAGE_SIZE, NULL);
	return 0;
}