#include"drawMultiplyWindow.h"
#include<atlimage.h>
#include"tool.h"
int			mg_curve, mg_cx, mg_cy, mg_x1, mg_x2, mg_y1, mg_y2;//消去算法处理需要
const int	dir[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
bool		m_vis[10][16];

DRAWMULTIPLYWINDOW::DRAWMULTIPLYWINDOW(HDC &hdc, HINSTANCE &hInstance):m_hdc(hdc), m_hInstance(hInstance)
{
	m_picPath[0] = TEXT("res\\chess\\1.bmp");
	m_picPath[1] = TEXT("res\\chess\\2.bmp");
	m_picPath[2] = TEXT("res\\chess\\3.bmp");
	m_picPath[3] = TEXT("res\\chess\\4.bmp");
	m_picPath[4] = TEXT("res\\chess\\5.bmp");
	m_picPath[5] = TEXT("res\\chess\\6.bmp");
	m_picPath[6] = TEXT("res\\chess\\7.bmp");
	m_picPath[7] = TEXT("res\\chess\\8.bmp");
	m_picPath[8] = TEXT("res\\chess\\9.bmp");
	m_picPath[9] = TEXT("res\\chess\\10.bmp");
	m_picPath[10] = TEXT("res\\chess\\11.bmp");
	m_picPath[11] = TEXT("res\\chess\\12.bmp");
	m_picPath[12] = TEXT("res\\chess\\13.bmp");
	m_picPath[13] = TEXT("res\\chess\\14.bmp");
	m_picPath[14] = TEXT("res\\chess\\15.bmp");
	m_picPath[15] = TEXT("res\\chess\\16.bmp");
	m_picPath[16] = TEXT("res\\chess\\17.bmp");
	m_picPath[17] = TEXT("res\\chess\\18.bmp");
	m_picPath[18] = TEXT("res\\chess\\19.bmp");
	m_picPath[19] = TEXT("res\\chess\\20.bmp");
	m_picPath[20] = TEXT("res\\chess\\21.bmp");
	m_picPath[21] = TEXT("res\\chess\\22.bmp");
	m_picPath[22] = TEXT("res\\chess\\23.bmp");

	m_picSelectedPath[0] = TEXT("res\\chess\\31.bmp");
	m_picSelectedPath[1] = TEXT("res\\chess\\32.bmp");
	m_picSelectedPath[2] = TEXT("res\\chess\\33.bmp");
	m_picSelectedPath[3] = TEXT("res\\chess\\34.bmp");
	m_picSelectedPath[4] = TEXT("res\\chess\\35.bmp");
	m_picSelectedPath[5] = TEXT("res\\chess\\36.bmp");
	m_picSelectedPath[6] = TEXT("res\\chess\\37.bmp");
	m_picSelectedPath[7] = TEXT("res\\chess\\38.bmp");
	m_picSelectedPath[8] = TEXT("res\\chess\\39.bmp");
	m_picSelectedPath[9] = TEXT("res\\chess\\40.bmp");
	m_picSelectedPath[10] = TEXT("res\\chess\\41.bmp");
	m_picSelectedPath[11] = TEXT("res\\chess\\42.bmp");
	m_picSelectedPath[12] = TEXT("res\\chess\\43.bmp");
	m_picSelectedPath[13] = TEXT("res\\chess\\44.bmp");
	m_picSelectedPath[14] = TEXT("res\\chess\\45.bmp");
	m_picSelectedPath[15] = TEXT("res\\chess\\46.bmp");
	m_picSelectedPath[16] = TEXT("res\\chess\\47.bmp");
	m_picSelectedPath[17] = TEXT("res\\chess\\48.bmp");
	m_picSelectedPath[18] = TEXT("res\\chess\\49.bmp");
	m_picSelectedPath[19] = TEXT("res\\chess\\50.bmp");
	m_picSelectedPath[20] = TEXT("res\\chess\\51.bmp");
	m_picSelectedPath[21] = TEXT("res\\chess\\52.bmp");
	m_picSelectedPath[22] = TEXT("res\\chess\\53.bmp");
	memset(m_picMap1, 0, sizeof(m_picMap1));
	memset(m_picMap2, 0, sizeof(m_picMap2));
}

DRAWMULTIPLYWINDOW::DRAWMULTIPLYWINDOW()
{

}

DRAWMULTIPLYWINDOW::~DRAWMULTIPLYWINDOW()
{
	DeleteDC(m_hdc);
}

void DRAWMULTIPLYWINDOW::DrawSelected(int x, int y, int val, bool mark)
{
	if (mark && val != 0)
	{
		HDC memDC = CreateCompatibleDC(m_hdc);
		HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, m_picSelectedPath[val - 1], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(memDC, hh);
		BitBlt(m_hdc, 60 + 40 * x - 10, 160 + 40 * y - 10, 60, 60, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
	}

	if (mark == 0 && val != 0)
	{
		HDC memDC = CreateCompatibleDC(m_hdc);
		const TCHAR *p = m_picPath[val - 1];
		HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, p, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(memDC, hh);
		BitBlt(m_hdc, 60 + 40 * x, 160 + 40 * y, 40, 40, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
	}
}

void DRAWMULTIPLYWINDOW::GetPic(int val[10][16])
{
	for (int row = 1; row<9; row++)
		for (int column = 1; column<15; column++)
			m_picMap1[row][column] = val[row][column];
}

void DRAWMULTIPLYWINDOW::GetPic2(int val[10][16])
{
	for (int row = 1; row<9; row++)
		for (int column = 1; column<15; column++)
			m_picMap2[row][column] = val[row][column];
}

bool DRAWMULTIPLYWINDOW::IfDelete(int x1, int y1, int v1, int x2, int y2, int v2)//消除算法的预处理部分
{
	if (v1 != v2)
		return false;
	if (x1 == x2 && y1 == y2)
		return false;
	mg_cx = x1;
	mg_cy = y1;
	mg_x1 = x1;
	mg_y1 = y1;
	mg_x2 = x2;
	mg_y2 = y2;
	mg_curve = 0;
	memset(m_vis, false, sizeof(m_vis));
	m_vis[y1][x1] = true;
	if (sol(mg_x1, mg_y1))
		return true;
	else
		return false;
}

bool DRAWMULTIPLYWINDOW::sol(int x, int y)//消除算法主体部分
{
	if (x == mg_x2 && y == mg_y2 && mg_curve <= 2)
		return true;
	if (mg_curve>2)
		return false;
	for (int i = 0; i != 4; i++)
	{
		int nx = x + dir[i][0];
		int ny = y + dir[i][1];

		if (nx>16 || ny>10 || nx < 0 || ny < 0)
			continue;
		if (m_picMap1[ny][nx] != 0 || m_vis[ny][nx])
			if (nx != mg_x2 || ny != mg_y2)
				continue;
		m_vis[ny][nx] = true;
		if (nx != mg_cx && ny != mg_cy)
		{
			mg_curve++;
			int temx = mg_cx;
			int temy = mg_cy;
			mg_cx = x;
			mg_cy = y;
			if (sol(nx, ny))
				return true;
			mg_curve--;
			mg_cx = temx;
			mg_cy = temy;
			m_vis[ny][nx] = false;
		}
		else
		{
			if (sol(nx, ny))
				return true;
			m_vis[ny][nx] = false;
		}
	}
	return false;
}


void DRAWMULTIPLYWINDOW::DrawGameMap(HWND hWnd, int score, int combo)//绘制游戏界面
{
	HDC hdc;
	hdc = GetDC(hWnd);
	HDC bgDC = CreateCompatibleDC(m_hdc);
	RECT rect;
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(bgDC, hBrush);
	GetClientRect(hWnd, &rect);
	Rectangle(bgDC, rect.left, rect.top, rect.right, rect.bottom);
	HBITMAP allBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(bgDC, allBitmap);
	/**/
	CImage image;
	image.Load(TEXT("res\\interface\\multiplyGame.bmp"));
	image.Draw(bgDC, 0, 0, image.GetWidth(), image.GetHeight());
	image.Destroy();
	
	//绘制自家棋子
	
	for (int iRow = 1; iRow<9; iRow++)
	{
		for (int iCol = 1; iCol<15; iCol++)
		{
			if (m_picMap1[iRow][iCol] != 0)
			{
				HDC memDC = CreateCompatibleDC(m_hdc);
				const TCHAR *p = m_picPath[m_picMap1[iRow][iCol] - 1];
				HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, p, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				SelectObject(memDC, hh);

				CImage image;
				image.Load(p);
				image.Draw(memDC, 0, 0, image.GetWidth(), image.GetHeight());

				BitBlt(bgDC, 60 + iCol * 40, 160 + iRow * 40, 40, 40, memDC, 0, 0, SRCCOPY);
				DeleteObject(hh);
				DeleteDC(memDC);
			}
		}
	}
	/////////////////////////////////////////////////////
	//绘制其他玩家棋子
	
	for (int iRow = 1; iRow<9; iRow++)
	{
		for (int iCol = 1; iCol<15; iCol++)
		{
			if (m_picMap2[iRow][iCol] != 0)
			{
				HDC memDC = CreateCompatibleDC(m_hdc);
				const TCHAR *p = m_picPath[m_picMap2[iRow][iCol] - 1];
				HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, p, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				SelectObject(memDC, hh);

				CImage image;
				image.Load(p);
				image.Draw(memDC, 0, 0, image.GetWidth(), image.GetHeight());

				BitBlt(bgDC, 720 + iCol * 40, 160 + iRow * 40, 40, 40, memDC, 0, 0, SRCCOPY);
				DeleteObject(hh);
				DeleteDC(memDC);
			}
		}
	}


	/////////////////////////////////////////////////////

	BitBlt(m_hdc, 0, 0, 1420, 600, bgDC, 0, 0, SRCCOPY);
	//	CDC* pDC = this->GetDC();
	SetTextColor(hdc, RGB(255, 50, 50));
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 330, 120, L"当前得分:", 5);
	wchar_t *tem_wchar = t_IntToWchar(score);
	TextOut(hdc, 400, 120, tem_wchar, WideCharToMultiByte(CP_ACP, NULL, tem_wchar, -1, NULL, 0, NULL, FALSE) - 1);

	TextOut(hdc, 330, 140, L"当前连击:", 5);
	wchar_t *tem_wchar2 = t_IntToWchar(combo);
	TextOut(hdc, 400, 140, tem_wchar2, WideCharToMultiByte(CP_ACP, NULL, tem_wchar2, -1, NULL, 0, NULL, FALSE) - 1);

	delete[]tem_wchar2;
	delete[]tem_wchar;
	DeleteObject(allBitmap);
	//	DeleteObject(hBrush);
	DeleteDC(bgDC);
}