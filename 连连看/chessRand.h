#pragma once
#include"header.h"

struct CHESSRAND
{//��Ա����
	int m_ChessId[10][16];					//�洢����ID
	//��Ա����
	CHESSRAND();
	~CHESSRAND();
	void GetRandPic();						//�������ͼƬ���
	bool IsEmpty();							//�ж��Ƿ�ȫ����ȥ
	bool Clear();							//�������λ��
	//////////////////////////////////////////////////////////////////
	
};