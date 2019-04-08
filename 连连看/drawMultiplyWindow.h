#pragma once
#include<Windows.h>

struct DRAWMULTIPLYWINDOW
{
	const TCHAR		*m_picPath[24];									//��������ͼƬ·��
	const TCHAR		*m_picSelectedPath[24];							//���汻����е�����ͼƬ·��
	int				m_picMap1[10][16];								//�����Լ����ӷֲ�
	int				m_picMap2[10][16];								//��������������ӷֲ�
	HDC				m_hdc;											//�����ľ��
	HINSTANCE		m_hInstance;									//����ʵ��

	////////////////////////////////////////////////////////

	//��Ա��������
	DRAWMULTIPLYWINDOW(HDC &hdc, HINSTANCE &hInstance);				//�вι��캯��
	DRAWMULTIPLYWINDOW();											//�޲�Ĭ�Ϲ��캯��
	~DRAWMULTIPLYWINDOW();											//��������
	void DrawSelected(int x, int y, int val, bool mark);			//��������ѡ��Ч��
	void DrawGameMap(HWND hWnd, int score, int combo);				//������Ϸ����
	void GetPic(int val[10][16]);									//ȡ���Լҵ�ǰ���ӷֲ�
	void GetPic2(int val[10][16]);									//ȡ��������ҵ�ǰ���ӷֲ�
	bool IfDelete(int x1, int y1, int v1, int x2, int y2, int v2);	//��ȥ�㷨���벿��
	bool sol(int x, int y);											//��ȥ�㷨�������
};