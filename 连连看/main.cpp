//�������岿�ִ���

#include"header.h"

//������Դ
#define ID_TIMER  1				//1�ż�ʱ������������
#define ID_TIMER2 2				//2�ż�ʱ�����������ж�
#define ID_TIMER3 3				//3�ż�ʱ������������Ϸ���ݴ�����
#define IDE_EDIT1 40003			//ע�ᴰ���˺��ı���
#define IDE_EDIT2 40004			//ע�ᴰ�������ı���
#define IDE_EDIT3 40011			//��������ı��������������ڼ������ӵ�ip��ַ

////////////////////////////////////////////////////

HWND		g_edAccount;				//�û��༭����
HWND		g_edPassword;				//����༭����
HWND		g_edIpAddr;					//���ڼ���������ip��ַ
HINSTANCE	g_hInstance;				//ȫ��ʵ������
HINSTANCE	tem_hInstance;				//���㴴��������Ϸ����
int			tem_cmdShow;				//���㴴��������Ϸ����
DRAWWINDOW	*drawWnd;					//���ڻ���ʵ������
DRAWWINDOW	*drawWnd2;					//ip���봰�ڻ���ʵ������
DRAWMULTIPLYWINDOW *drawMultiplyWindow;	//������Ϸ���ڻ���ʵ������
CHESSRAND	chessRand;					//������Ϸ���Ӳ���ʵ������
CHESSRAND	chessRand2;					//������Ϸ���Ӳ���ʵ������
DBCONNECTOR	dbConnector;				//ע�ᴰ�����ݿ����ʵ������
DBCONNECTOR dbConnector2;				//������Ϸ�������ݿ����ʵ������
DBCONNECTOR dbConnector3;				//������Ϸ�������ݿ����ʵ������
int			modeFlag = 0;		//�����ڽ����ʶ,0Ϊ������,1Ϊ��Ϸ����,2Ϊ��������,3Ϊ���ڽ���,4Ϊʧ�ܽ���,5Ϊ�ɹ�ͨ�ؽ���
int			limitTime = 180;	//��Ϸ����ʱ��
int			clickTime = 0;		//������Ϸ�������
int			clickTime2 = 0;		//������Ϸ�������
int			score = 0;			//��Ϸ��ǰ�÷�
int			combo = 0;			//��ǰ����
int			preCombo = 0;		//1400msǰ��combo(ID_TIMER2)
int			score2 = 0;			//��Ϸ��ǰ�÷֣������汾��
int			combo2 = 0;			//��ǰ�����������汾��
int			preCombo2 = 0;		//1400msǰ��combo(ID_TIMER2)�������汾��
int			flagSC = 1;			//���û��ĵĽ�ɫ��0�����or1�ͻ��ˣ�
bool		isRegister = false;	//ȷ�ϵ����Ƿ�ɹ�
wchar_t		acc[32];			//�û��˻�
wchar_t		pwd[32];			//�û�����
wchar_t		ipAddr[32];			//���ڼ���������ip��ַ
std::string str_ipAddr;			//���ڼ���������ip��ַ��Ϊstring
int			prevX, prevY, prevV, curX, curY, curV;
int			prevX2, prevY2, prevV2, curX2, curY2, curV2;
//��ģ���ļ������к�������
LRESULT CALLBACK RegisProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//ע�ᴰ����Ϣ������
HRESULT InitRegis(HINSTANCE hInstance, int cmdShow);											//��ʼ��ע�ᴰ��										
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//��������Ϣ������
HRESULT InitWindow(HINSTANCE hInstance, int cmdShow);											//��ʼ��������
LRESULT CALLBACK MultiplyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);			//������Ϸ������Ϣ����
HRESULT InitMultiplyWindow(HINSTANCE hInstance, int cmdShow);									//��ʼ��������Ϸ����
HRESULT RegisMultiply(HINSTANCE hInstance);														//ע��������Ϸ������
LRESULT CALLBACK IpGetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//ip���봰����Ϣ������
HRESULT InitIpGetWindow(HINSTANCE hInstance, int cmdShow);										//ip���봰�ڴ���
HRESULT RegisIpGetWindow(HINSTANCE hInstance);													//ip���봰���ഴ��
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);	//������ں���
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
		MessageBox(NULL, TEXT("����ע����ʧ��"), NULL, 0);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InitMultiplyWindow(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;

	hWnd = CreateWindow(TEXT("multiply"), TEXT("�������ɣ���"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 1420, 600, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), NULL, 0);
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

		if (x > 100 && x < 660 && y>200 && y < 520)//��Ϸ��������
		{
			int temCol = (x - 100) / 40 + 1;
			int temRow = (y - 200) / 40 + 1;
			clickTime2++;
			if (clickTime2 % 2 == 0)//�����Ϊ˫��ʱ�Ž��������ж�
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
					//��Ϸ��Ԫ��/////////////////////////////////////////////////
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
						//���ټ�ʱ��
						KillTimer(hWnd, ID_TIMER2);
						KillTimer(hWnd, ID_TIMER3);
						clientAct(str_ipAddr, "clear");
						MessageBox(NULL, TEXT("��Ӯ��"), NULL, NULL);
						DestroyWindow(hWnd);
						//������Ϸ��ʤ
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
			//ÿ���������˷������̲�����Ϣ
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
		//���ټ�ʱ��
		KillTimer(hWnd, ID_TIMER2);
		KillTimer(hWnd, ID_TIMER3);
		score2 = 0;
		preCombo2 = 0;
		combo2 = 0;
		preCombo2 = 0;
		clickTime2 = 0;
		delete(drawMultiplyWindow);
		clientAct(str_ipAddr, "clear");//����������Ϣ
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
		MessageBox(NULL, TEXT("����ע����ʧ��"), NULL, 0);
		return E_FAIL;
	}
	
	hWnd = CreateWindow(TEXT("regis"), TEXT("����ȵ���da��ze~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 350, NULL, NULL, hInstance, NULL);
	g_edAccount=CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD |ES_LEFT ,
		200, 55, 100, 20, hWnd, (HMENU)IDE_EDIT1, hInstance, NULL);
	g_edPassword=CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_LEFT|ES_PASSWORD,
		200, 135, 100, 20, hWnd, (HMENU)IDE_EDIT2, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), NULL, 0);
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
				MessageBox(NULL, L"����ɹ���~", L"ok da��ze", 0);
				SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			}
			else
			{
				isRegister = true;
				MessageBox(NULL, L"�˻����������������������", L"fail daze.", 0);
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
				MessageBox(NULL, L"ע��ɹ���~", L"ok da��ze", 0);
				wcscat(pwd, L"");
			}
			else
			{
				MessageBox(NULL, L"ע��ʧ���˵�˵������", L"error ze", 0);
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
		MessageBox(NULL, TEXT("����ע����ʧ��"), NULL, 0);
		return E_FAIL;
	}

	hWnd = CreateWindow(TEXT("main frame"), TEXT("���ɰ�����������~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), NULL, 0);
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
		case IDM_MENU_MULTIPLY://������Ϸ�˵�
			InitIpGetWindow(tem_hInstance, tem_cmdShow);
			break;

		case IDM_MENU_RANK://�÷ְ�˵�
			std::string temmm = dbConnector2.GetSelect("rank", "DESC");
			MessageBox(NULL, t_StringToWchar(temmm), TEXT("�ɰ��ĵ÷ְ��"), 0);
			break;

		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		hdc = GetDC(hWnd);
		if (modeFlag == 0 && x > 250 && x < 550 && y>270 && y < 330)//��ʼ��Ϸ
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
		if (modeFlag == 1 && x > 100 && x < 660 && y>200 && y < 520)//��Ϸ��������
		{
			int temCol = (x - 100) / 40 + 1;
			int temRow = (y - 200) / 40 + 1;
			clickTime++;
			if (clickTime % 2 == 0)//�����Ϊ˫��ʱ�Ž��������ж�
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
					//��Ϸ��Ԫ��/////////////////////////////////////////////////
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
		if (modeFlag == 1 && x > 680 && x < 780 && y>200 && y < 250)//��Ϸ������������
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
		if (modeFlag == 1 && x > 680 && x < 780 && y>300 && y < 350)//��Ϸ����������ʾ
		{
			if (score)
				score -= rand() % score;
			drawWnd->Tip();
		}
		if (modeFlag == 0 && x > 250 && x < 550 && y>355 && y < 415)//����
		{
			modeFlag = 2;
			drawWnd->DrawInterface(TEXT("res\\interface\\help.bmp"));
		}

		if (modeFlag == 0 && x > 250 && x < 550 && y>440 && y < 500)//����
		{
			modeFlag = 3;
			drawWnd->DrawInterface(TEXT("res\\interface\\about.bmp"));
		}
		if (modeFlag == 4)											//ʧ������
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
		if (modeFlag == 5)											//ͨ������
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
		if (modeFlag != 0 && x > 100 && x < 200 && y<150 && y > 100)//����
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
		MessageBox(NULL, TEXT("����ע����ʧ��"), NULL, 0);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InitIpGetWindow(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;

	hWnd = CreateWindow(TEXT("IpGet"), TEXT("�������˵�IP��ַ��~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 350, NULL, NULL, hInstance, NULL);

	g_edIpAddr = CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_LEFT,
		200, 55, 100, 20, hWnd, (HMENU)IDE_EDIT3, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), NULL, 0);
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
		if (x > 225 && x < 275 && y>210 && y < 240)//ȷ������ip��ַ
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

