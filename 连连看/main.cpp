//程序主体部分代码

#include"header.h"

//定义资源
#define ID_TIMER  1				//1号计时器负责界面绘制
#define ID_TIMER2 2				//2号计时器负责连击判定
#define ID_TIMER3 3				//3号计时器控制联机游戏数据传输间隔
#define IDE_EDIT1 40003			//注册窗口账号文本框
#define IDE_EDIT2 40004			//注册窗口密码文本框
#define IDE_EDIT3 40011			//主界面的文本框，用于输入用于简易连接的ip地址

////////////////////////////////////////////////////

HWND		g_edAccount;				//用户编辑框句柄
HWND		g_edPassword;				//密码编辑框句柄
HWND		g_edIpAddr;					//用于简易联机的ip地址
HINSTANCE	g_hInstance;				//全局实例对象
HINSTANCE	tem_hInstance;				//方便创建联机游戏窗口
int			tem_cmdShow;				//方便创建联机游戏窗口
DRAWWINDOW	*drawWnd;					//窗口绘制实例对象
DRAWWINDOW	*drawWnd2;					//ip输入窗口绘制实例对象
DRAWMULTIPLYWINDOW *drawMultiplyWindow;	//联机游戏窗口绘制实例对象
CHESSRAND	chessRand;					//单机游戏棋子操作实例对象
CHESSRAND	chessRand2;					//联机游戏棋子操作实例对象
DBCONNECTOR	dbConnector;				//注册窗口数据库操作实例对象
DBCONNECTOR dbConnector2;				//单机游戏窗口数据库操作实例对象
DBCONNECTOR dbConnector3;				//联机游戏窗口数据库操作实例对象
int			modeFlag = 0;		//主窗口界面标识,0为主界面,1为游戏界面,2为帮助界面,3为关于界面,4为失败界面,5为成功通关界面
int			limitTime = 180;	//游戏限制时间
int			clickTime = 0;		//单机游戏点击次数
int			clickTime2 = 0;		//联机游戏点击次数
int			score = 0;			//游戏当前得分
int			combo = 0;			//当前连击
int			preCombo = 0;		//1400ms前的combo(ID_TIMER2)
int			score2 = 0;			//游戏当前得分（联机版本）
int			combo2 = 0;			//当前连击（联机版本）
int			preCombo2 = 0;		//1400ms前的combo(ID_TIMER2)（联机版本）
int			flagSC = 1;			//辨别该机的的角色（0服务端or1客户端）
bool		isRegister = false;	//确认登入是否成功
wchar_t		acc[32];			//用户账户
wchar_t		pwd[32];			//用户密码
wchar_t		ipAddr[32];			//用于简易联机的ip地址
std::string str_ipAddr;			//用于简易联机的ip地址化为string
int			prevX, prevY, prevV, curX, curY, curV;
int			prevX2, prevY2, prevV2, curX2, curY2, curV2;
//该模块文件的所有函数声明
LRESULT CALLBACK RegisProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//注册窗口消息处理函数
HRESULT InitRegis(HINSTANCE hInstance, int cmdShow);											//初始化注册窗口										
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//主窗口消息处理函数
HRESULT InitWindow(HINSTANCE hInstance, int cmdShow);											//创始化主窗口
LRESULT CALLBACK MultiplyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);			//联机游戏窗口消息处理
HRESULT InitMultiplyWindow(HINSTANCE hInstance, int cmdShow);									//初始化联机游戏窗口
HRESULT RegisMultiply(HINSTANCE hInstance);														//注册联机游戏窗口类
LRESULT CALLBACK IpGetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//ip输入窗口消息处理函数
HRESULT InitIpGetWindow(HINSTANCE hInstance, int cmdShow);										//ip输入窗口创建
HRESULT RegisIpGetWindow(HINSTANCE hInstance);													//ip输入窗口类创建
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);	//程序入口函数
//////////////////////////////////////////////////////////////////////////////////////////////

HRESULT RegisMultiply(HINSTANCE hInstance)
{
	WNDCLASSEX	wc;
	g_hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursorFromFile(TEXT("cursor.ani"));
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = MultiplyProc;
	wc.lpszClassName = TEXT("multiply");
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册类失败"), NULL, 0);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InitMultiplyWindow(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;

	hWnd = CreateWindow(TEXT("multiply"), TEXT("来联机吧！★"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 1420, 600, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), NULL, 0);
		return E_FAIL;
	}

	SetWindowPos(hWnd, NULL, 0, 0, 1420, 600, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;

}

LRESULT CALLBACK MultiplyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	switch (message)
	{
	case WM_CREATE:
	{
		HDC hdc1 = GetDC(hWnd);
		chessRand2.Clear();
		chessRand2.GetRandPic();
		drawMultiplyWindow = new DRAWMULTIPLYWINDOW(hdc1, g_hInstance);
		drawMultiplyWindow->GetPic(chessRand2.m_ChessId);
		SetTimer(hWnd, ID_TIMER2, 1400, NULL);
		SetTimer(hWnd, ID_TIMER3, 1000, NULL);
	}
	break;
		

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		drawMultiplyWindow->DrawGameMap(hWnd, score2, combo2);
		EndPaint(hWnd, &ps);
	}
	break;
		

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		hdc = GetDC(hWnd);

		if (x > 100 && x < 660 && y>200 && y < 520)//游戏棋子区域
		{
			int temCol = (x - 100) / 40 + 1;
			int temRow = (y - 200) / 40 + 1;
			clickTime2++;
			if (clickTime2 % 2 == 0)//点击数为双数时才进行消除判断
			{
				curX2 = temCol;
				curY2 = temRow;
				curV2 = drawMultiplyWindow->m_picMap1[curY2][curX2];
				bool sss = drawMultiplyWindow->IfDelete(curX2, curY2, curV2, prevX2, prevY2, prevV2);

				if (sss)
				{
					drawMultiplyWindow->m_picMap1[curY2][curX2] = 0;
					drawMultiplyWindow->m_picMap1[prevY2][prevX2] = 0;
					chessRand2.m_ChessId[curY2][curX2] = 0;
					chessRand2.m_ChessId[prevY2][prevX2] = 0;
					//游戏性元素/////////////////////////////////////////////////
					score2 += 100;
					combo2++;
					if (combo2 > 10)
						score2 += 500;
					else if (combo2 > 8)
						score2 += 300;
					else if (combo2 > 5)
						score2 += 200;
					else if (combo2 > 3)
						score2 += 100;
					////////////////////////////////////////////////////////////
					drawMultiplyWindow->DrawGameMap(hWnd, score2, combo2);

					if (chessRand2.IsEmpty())
					{
						//销毁计时器
						KillTimer(hWnd, ID_TIMER2);
						KillTimer(hWnd, ID_TIMER3);
						clientAct(str_ipAddr, "clear");
						MessageBox(NULL, TEXT("你赢了"), NULL, NULL);
						DestroyWindow(hWnd);
						//多人游戏获胜
						break;
					}
				}
			}
			else
			{
				prevX2 = temCol;
				prevY2 = temRow;
				prevV2 = drawMultiplyWindow->m_picMap1[prevY2][prevX2];
				drawMultiplyWindow->DrawSelected(temCol, temRow, drawMultiplyWindow->m_picMap1[temRow][temCol], true);
			}
		}
	}
	break;

	case WM_TIMER:
	{
		if (wParam == ID_TIMER3)
		{
			//每秒种向服务端发送棋盘布局信息
			std::string temChess = clientAct(str_ipAddr,(t_TransChessSend(drawMultiplyWindow->m_picMap1)));
			if (temChess == "clear")
			{
				DestroyWindow(hWnd);
				break;
			}

			t_TransChessInf(temChess, drawMultiplyWindow->m_picMap2);
			drawMultiplyWindow->DrawGameMap(hWnd, score2, combo2);
		}

		if (wParam == ID_TIMER2)
		{
			if (preCombo2 == combo2)
			{
				preCombo2 = 0;
				combo2 = 0;
			}
			else
			{
				preCombo2 = combo2;
			}
		}
	}	
	break;
	
	case WM_DESTROY:
	{
		//销毁计时器
		KillTimer(hWnd, ID_TIMER2);
		KillTimer(hWnd, ID_TIMER3);
		score2 = 0;
		preCombo2 = 0;
		combo2 = 0;
		preCombo2 = 0;
		clickTime2 = 0;
		delete(drawMultiplyWindow);
		clientAct(str_ipAddr, "clear");//清理服务端信息
//		DestroyWindow(hWnd);
	}
	break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT InitRegis(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;
	WNDCLASSEX	wc;
	g_hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursorFromFile(TEXT("cursor.ani"));
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = RegisProc;
	wc.lpszClassName = TEXT("regis");
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册类失败"), NULL, 0);
		return E_FAIL;
	}
	
	hWnd = CreateWindow(TEXT("regis"), TEXT("你得先登入da☆ze~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 350, NULL, NULL, hInstance, NULL);
	g_edAccount=CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD |ES_LEFT ,
		200, 55, 100, 20, hWnd, (HMENU)IDE_EDIT1, hInstance, NULL);
	g_edPassword=CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_LEFT|ES_PASSWORD,
		200, 135, 100, 20, hWnd, (HMENU)IDE_EDIT2, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), NULL, 0);
		return E_FAIL;
	}
	SetWindowPos(hWnd, NULL, 0, 0, 500, 350, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;
}

LRESULT CALLBACK RegisProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	switch (message)
	{
	case WM_CREATE:
	{
		HDC hdc1 = GetDC(hWnd);
		drawWnd = new DRAWWINDOW(hdc1, g_hInstance);
	}
	break;
	
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		drawWnd->DrawInterface(L"res\\interface\\login.bmp");
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		hdc = GetDC(hWnd);
		if (x > 225 && x < 275 && y>210 && y < 240)
		{
			
			SendMessage(g_edAccount, WM_GETTEXT, 32, (LPARAM)acc);
			SendMessage(g_edAccount, WM_SETTEXT, NULL, (LPARAM)acc);

			SendMessage(g_edPassword, WM_GETTEXT, 32, (LPARAM)pwd);
			SendMessage(g_edPassword, WM_SETTEXT, NULL, (LPARAM)pwd);
			GetWindowText(g_edPassword, pwd, 32);
			if (t_WcharToString(acc) == "s"&&
				t_WcharToString(pwd) == "s")
			{
				isRegister = true;
				break;
			}
				
			if (dbConnector.IsLogin(acc, pwd))
			{
				isRegister = true;
				MessageBox(NULL, L"登入成功啦~", L"ok da☆ze", 0);
				SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			}
			else
			{
				isRegister = true;
				MessageBox(NULL, L"账户名或者密码错误啦。。。", L"fail daze.", 0);
			}
			break;
		}
		if (x > 225 && x < 275 && y>270 && y < 300)
		{
			wchar_t			acc[32];
			wchar_t			pwd[32];
			SendMessage(g_edAccount, WM_GETTEXT, 32, (LPARAM)acc);
			SendMessage(g_edAccount, WM_SETTEXT, NULL, (LPARAM)acc);

			SendMessage(g_edPassword, WM_GETTEXT, 32, (LPARAM)pwd);
			
			SendMessage(g_edPassword, WM_SETTEXT, NULL, (LPARAM)pwd);
			if (dbConnector.RunRegister(acc, pwd))
			{
				MessageBox(NULL, L"注册成功啦~", L"ok da☆ze", 0);
				wcscat(pwd, L"");
			}
			else
			{
				MessageBox(NULL, L"注册失败了的说。。。", L"error ze", 0);
				wcscat(pwd, L"");
			}
		}
		break;
	}

	case WM_DESTROY:
	{
		if (!isRegister)
		{
			delete(drawWnd);
			PostQuitMessage(0);
		}
		else
		{
			delete(drawWnd);
			DestroyWindow(hWnd);
		}
		break;
	}

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT InitWindow(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;
	WNDCLASSEX	wc;
	g_hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursorFromFile(TEXT("cursor.ani"));
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("main frame");
	wc.lpszMenuName = TEXT("MainMenu");
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册类失败"), NULL, 0);
		return E_FAIL;
	}

	hWnd = CreateWindow(TEXT("main frame"), TEXT("超可爱的连连看☆~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), NULL, 0);
		return E_FAIL;
	}
	SetWindowPos(hWnd, NULL, 0, 0, 800, 600, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT			ps;
	HDC					hdc;
	switch (message)
	{
	case WM_CREATE:
	{
		HDC hdc1 = GetDC(hWnd);
		drawWnd = new DRAWWINDOW(hdc1, g_hInstance);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		if (modeFlag == 0)
			drawWnd->DrawInterface(TEXT("res\\interface\\start.bmp"));
		if (modeFlag == 1)
			drawWnd->DrawGameMap(hWnd, limitTime, score, combo);
		if (modeFlag == 2)
			drawWnd->DrawInterface(TEXT("res\\interface\\help.bmp"));
		if (modeFlag == 3)
			drawWnd->DrawInterface(TEXT("res\\interface\\about.bmp"));
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_MENU_MULTIPLY://多人游戏菜单
			InitIpGetWindow(tem_hInstance, tem_cmdShow);
			break;

		case IDM_MENU_RANK://得分榜菜单
			std::string temmm = dbConnector2.GetSelect("rank", "DESC");
			MessageBox(NULL, t_StringToWchar(temmm), TEXT("可爱的得分榜★"), 0);
			break;

		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		hdc = GetDC(hWnd);
		if (modeFlag == 0 && x > 250 && x < 550 && y>270 && y < 330)//开始游戏
		{

			modeFlag = 1;
			limitTime = 180;
			chessRand.Clear();

			chessRand.GetRandPic();

			drawWnd->GetPic(chessRand.m_ChessId);
			drawWnd->DrawGameMap(hWnd, limitTime, score, combo);
			SetTimer(hWnd, ID_TIMER, 1000, NULL);
			SetTimer(hWnd, ID_TIMER2, 1400, NULL);
			break;
		}
		if (modeFlag == 1 && x > 100 && x < 660 && y>200 && y < 520)//游戏棋子区域
		{
			int temCol = (x - 100) / 40 + 1;
			int temRow = (y - 200) / 40 + 1;
			clickTime++;
			if (clickTime % 2 == 0)//点击数为双数时才进行消除判断
			{
				curX = temCol;
				curY = temRow;
				curV = drawWnd->m_picMap[curY][curX];
				bool sss = drawWnd->IfDelete(curX, curY, curV, prevX, prevY, prevV);

				if (sss)
				{
					drawWnd->m_picMap[curY][curX] = 0;
					drawWnd->m_picMap[prevY][prevX] = 0;
					chessRand.m_ChessId[curY][curX] = 0;
					chessRand.m_ChessId[prevY][prevX] = 0;
					//游戏性元素/////////////////////////////////////////////////
					score += 100;
					combo++;
					if (combo > 10)
						score += 500;
					else if (combo > 8)
						score += 300;
					else if (combo > 5)
						score += 200;
					else if (combo > 3)
						score += 100;

					////////////////////////////////////////////////////////////
					drawWnd->DrawGameMap(hWnd, limitTime, score, combo);

					if (chessRand.IsEmpty())
					{
						dbConnector2.ScoreSave(acc, score);
						modeFlag = 5;
						drawWnd->DrawInterface(TEXT("res\\interface\\success.bmp"));
						break;
					}
				}
			}
			else
			{
				prevX = temCol;
				prevY = temRow;
				prevV = drawWnd->m_picMap[prevY][prevX];
				//				drawWnd->DrawSelected(prevX, prevY, prevV, false);
				drawWnd->DrawSelected(temCol, temRow, drawWnd->m_picMap[temRow][temCol], true);
			}
		}
		if (modeFlag == 1 && x > 680 && x < 780 && y>200 && y < 250)//游戏消除界面重来
		{

			chessRand.Clear();
			chessRand.GetRandPic();
			limitTime = 180;
			score = 0;
			preCombo = 0;
			combo = 0;
			drawWnd->GetPic(chessRand.m_ChessId);
			drawWnd->DrawGameMap(hWnd, limitTime, score, combo);
		}
		if (modeFlag == 1 && x > 680 && x < 780 && y>300 && y < 350)//游戏消除界面提示
		{
			if (score)
				score -= rand() % score;
			drawWnd->Tip();
		}
		if (modeFlag == 0 && x > 250 && x < 550 && y>355 && y < 415)//帮助
		{
			modeFlag = 2;
			drawWnd->DrawInterface(TEXT("res\\interface\\help.bmp"));
		}

		if (modeFlag == 0 && x > 250 && x < 550 && y>440 && y < 500)//关于
		{
			modeFlag = 3;
			drawWnd->DrawInterface(TEXT("res\\interface\\about.bmp"));
		}
		if (modeFlag == 4)											//失败重来
		{
			dbConnector2.ScoreSave(acc, score);
			modeFlag = 1;
			chessRand.Clear();
			chessRand.GetRandPic();
			limitTime = 180;
			score = 0;
			combo = 0;
			drawWnd->GetPic(chessRand.m_ChessId);
			drawWnd->DrawGameMap(hWnd, limitTime, score, combo);
		}
		if (modeFlag == 5)											//通关重来
		{
			modeFlag = 1;
			chessRand.Clear();
			chessRand.GetRandPic();
			limitTime = 180;
			score = 0;
			preCombo = 0;
			combo = 0;
			drawWnd->GetPic(chessRand.m_ChessId);
			drawWnd->DrawGameMap(hWnd, limitTime, score, combo);
		}
		if (modeFlag != 0 && x > 100 && x < 200 && y<150 && y > 100)//返回
		{
			score = 0;
			preCombo = 0;
			combo = 0;
			modeFlag = 0;
			drawWnd->DrawInterface(TEXT("res\\interface\\start.bmp"));
		}
	}
	break;

	case WM_TIMER:
	{
		if (wParam == ID_TIMER)
		{
			limitTime--;
			if (modeFlag == 1 && limitTime == 0)
			{
				modeFlag = 4;
				drawWnd->DrawInterface(TEXT("res\\interface\\fail.bmp"));
			}
			if (modeFlag == 1)
			{

				drawWnd->DrawGameMap(hWnd, limitTime, score, combo);
			}
		}

		if (wParam == ID_TIMER2)
		{
			if (preCombo == combo)
			{
				preCombo = 0;
				combo = 0;
			}
			else
			{
				preCombo = combo;
			}
		}
	}
	break;

	case WM_DESTROY:
	{
		delete(drawWnd);
		PostQuitMessage(0);
	}
	break;

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT RegisIpGetWindow(HINSTANCE hInstance)
{
	WNDCLASSEX	wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursorFromFile(TEXT("cursor.ani"));
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = IpGetProc;
	wc.lpszClassName = TEXT("IpGet");
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册类失败"), NULL, 0);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InitIpGetWindow(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;

	hWnd = CreateWindow(TEXT("IpGet"), TEXT("输入服务端的IP地址☆~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 350, NULL, NULL, hInstance, NULL);

	g_edIpAddr = CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_LEFT,
		200, 55, 100, 20, hWnd, (HMENU)IDE_EDIT3, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), NULL, 0);
		return E_FAIL;
	}
	SetWindowPos(hWnd, NULL, 0, 0, 500, 350, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;
}

LRESULT CALLBACK IpGetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT			ps;
	HDC					hdc;
	switch (message)
	{
	case WM_CREATE:
	{
		HDC hdc1 = GetDC(hWnd);
		drawWnd2= new DRAWWINDOW(hdc1, g_hInstance);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		drawWnd2->DrawInterface(L"res\\interface\\ipGet.bmp");
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		hdc = GetDC(hWnd);
		if (x > 225 && x < 275 && y>210 && y < 240)//确认输入ip地址
		{
				SendMessage(g_edIpAddr, WM_GETTEXT, 32, (LPARAM)ipAddr);
				SendMessage(g_edIpAddr, WM_SETTEXT, NULL, (LPARAM)ipAddr);
				str_ipAddr = t_WcharToString(ipAddr);
				InitMultiplyWindow(tem_hInstance, tem_cmdShow);
				DestroyWindow(hWnd);
		}
	}
	break;

	case WM_DESTROY:
	{
//		delete(drawWnd2);
		DestroyWindow(hWnd);
	}
	break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	if(!isRegister)
		if (FAILED(InitRegis(hInstance, cmdShow)))
			return 0;

	tem_cmdShow = cmdShow;
	tem_hInstance = hInstance;
	RegisIpGetWindow(hInstance);
	RegisMultiply(hInstance);
	if (isRegister)
	{
		if (FAILED(InitWindow(hInstance, cmdShow)))
		{
			return 0;
		}
	}
		

	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)
	{
		if (isRegister)
		{
			isRegister = false;
			if (FAILED(InitWindow(hInstance, cmdShow)))
				return 0;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

