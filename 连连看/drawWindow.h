#pragma once
#include<Windows.h>
struct DRAWWINDOW
{
	
	//��Ա��������
	const TCHAR		*m_picPath[24];			//��������ͼƬ·��
	const TCHAR		*m_picSelectedPath[24];	//���汻����е�����ͼƬ·��
	int				m_picMap[10][16];		//�������ӷֲ�
	HDC				m_hdc;					//�����ľ��
	HINSTANCE		m_hInstance;			//����ʵ��
	////////////////////////////////////////////////////////

	//��Ա��������
	DRAWWINDOW(HDC &hdc,HINSTANCE &hInstance );						//���캯��
	DRAWWINDOW();													//���캯��
	~DRAWWINDOW();													//��������
	void DrawGameMap(HWND hWnd,int timeLength,int score,int combo);	//������Ϸ�����һЩ���������
	void DrawTimeBar(HWND hWnd, int timeLength);					//������Ϸʱ����
	void DrawSelected(int x, int y, int val, bool mark);			//��������ѡ��Ч��
	void DrawInterface(const TCHAR *picPath);						//���ƽ���
	void GetPic(int val[10][16]);									//����ֲ�����
	bool IfDelete(int x1, int y1, int v1, int x2, int y2, int v2);	//��ȥ�㷨���벿��
	bool sol(int x, int y);											//��ȥ�㷨�������
	bool Tip();														//ʹ����ʾ
	////////////////////////////////////////////////////////
};