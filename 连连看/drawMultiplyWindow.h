#pragma once
#include<Windows.h>

struct DRAWMULTIPLYWINDOW
{
	const TCHAR		*m_picPath[24];									//储存棋子图片路径
	const TCHAR		*m_picSelectedPath[24];							//储存被点击中的棋子图片路径
	int				m_picMap1[10][16];								//储存自家棋子分布
	int				m_picMap2[10][16];								//储存其他玩家棋子分布
	HDC				m_hdc;											//上下文句柄
	HINSTANCE		m_hInstance;									//窗口实例

	////////////////////////////////////////////////////////

	//成员函数声明
	DRAWMULTIPLYWINDOW(HDC &hdc, HINSTANCE &hInstance);				//有参构造函数
	DRAWMULTIPLYWINDOW();											//无参默认构造函数
	~DRAWMULTIPLYWINDOW();											//析构函数
	void DrawSelected(int x, int y, int val, bool mark);			//绘制棋子选中效果
	void DrawGameMap(HWND hWnd, int score, int combo);				//绘制游戏界面
	void GetPic(int val[10][16]);									//取得自家当前棋子分布
	void GetPic2(int val[10][16]);									//取得其他玩家当前棋子分布
	bool IfDelete(int x1, int y1, int v1, int x2, int y2, int v2);	//消去算法输入部分
	bool sol(int x, int y);											//消去算法输出部分
};