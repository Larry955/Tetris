// Teris.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Teris.h"
#define MAX_LOADSTRING 100

#define BOUND_SIZE 10	//��Ϸ�߽��С
#define TERIS_SIZE 30		//ÿ�����Ӵ�С
#define GAME_X 10		//������Ϸ�����С����10������
#define GAME_Y 20		//������Ϸ�����С����20������
#define INFO_X 6		//����÷������С����6������
#define INFO_Y GAME_Y	//����÷������С����20�����ӣ���������Ϸ�����Сһ�£�
#define MY_TIMER 1
#define DEFAULT_INTERVAL	500	//Ĭ���½��ٶȣ�500�����½�һ��

//����˹�������״
BOOL g_shape_Teris[][4][4] = 
{
	{ { 1, 1, 1, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 0, 0, 0 } },
	{ { 1, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 0, 1, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }

};
#define TERIS_TYPE (sizeof(g_shape_Teris)/sizeof(g_shape_Teris[0]))		//��ʾ���������

BOOL g_CurrTeris[4][4];				//��ǰ����˹�������״
BOOL g_NextTeris[4][4];				//������һ������˹�������״
BOOL g_stGame[GAME_X][GAME_Y];	//��¼��Щλ���з���
UINT TerisX;	//�������Ͻǵ�x����,UINT��32λ�޷�������
UINT TerisY;
UINT g_Speed;
UINT g_Score;


// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TERIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TERIS));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TERIS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TERIS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
  
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   { 
	   MessageBox(NULL, TEXT("��������ʧ��"),TEXT("��ʾ"),MB_OK);
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

INT	GetRandNum(int iMin, int iMax)
{
	//ȡ�����
	srand(GetTickCount() + g_Speed--);
	if (iMin > iMax)
		return -1;
	return iMin + rand() % (iMax - iMin); 
}

/*HDC��Windows��һ���������ͣ����豸���������������
��������ĳ���豸�йصĻ�ָ������Ϣ,��Щ�����װ���йػ����������ı�����״��Windows API*/
VOID DrawBackGround(HDC hdc)	
{
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);

	HBRUSH hBrush1 = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//HBRUSH hB = (HBRUSH)CreateSolidBrush(RGB(0xff, 0, 0));

	//������Ϸ��
	Rectangle(hdc, BOUND_SIZE, BOUND_SIZE,
		BOUND_SIZE + GAME_X * TERIS_SIZE,
		BOUND_SIZE + GAME_Y * TERIS_SIZE); /*nLeftRect��ָ���������Ͻǵ��߼�X���ꡣ
										nTopRect��ָ���������Ͻǵ��߼�Y���ꡣ
										nRightRect��ָ���������½ǵ��߼�X���ꡣ
										nBottomRect��ָ���������½ǵ��߼�Y���ꡣ*/
	SelectObject(hdc, hPen);
	//������Ϸ���е�С����
	//int x = 0, y = 0;
	//��һ��С���飺Rectangle(hdc, BOUND_SIZE, BOUND_SIZE, BOUND_SIZE + TERIS_SIZE, BOUND_SIZE + TERIS_SIZE);
	for (int x = 0; x != GAME_X; ++x){		//x��ʾ���Ǹ���������Rectangle�еĲ�����λ�ã���˸������x*TERIS_SIZE�����ÿ�����ӵľ���
		for (int y = 0; y != GAME_Y; ++y){
			if (g_stGame[x][y]){
				SelectObject(hdc, hBrush1);

			}
			else{
				SelectObject(hdc, hBrush);

			}
			Rectangle(hdc, x * TERIS_SIZE + BOUND_SIZE, 
				y * TERIS_SIZE + BOUND_SIZE, 
				x * TERIS_SIZE + TERIS_SIZE + BOUND_SIZE, 
				y * TERIS_SIZE + TERIS_SIZE + BOUND_SIZE);
		}
	}
}

VOID DrawInfo(HDC hdc)
{
	HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hBrush1 = (HBRUSH)GetStockObject(GRAY_BRUSH);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	RECT rect;		//���Ƶ÷�ģ��
	TCHAR szBuf[100] = {};	//������

	int nStartX,nStartY;	//���ڻ���Ϣ���е�С����
	Rectangle(hdc, BOUND_SIZE * 2 + GAME_X * TERIS_SIZE,
		BOUND_SIZE,
		BOUND_SIZE * 2 + (GAME_X + INFO_X) * TERIS_SIZE,
		BOUND_SIZE + GAME_Y * TERIS_SIZE);

	for (int x = 0; x != 4; ++x){
		for (int y = 0; y != 4; ++y){
			nStartX = BOUND_SIZE * 2 + GAME_X * TERIS_SIZE + (x + 1) * TERIS_SIZE;	//ÿ��С��������Ͻ�����
			nStartY = BOUND_SIZE + (y + 1) * TERIS_SIZE;
			if (g_NextTeris[x][y]){
				SelectObject(hdc, hBrush);
			}
			else{
				SelectObject(hdc, hBrush1);
			}
			Rectangle(hdc, nStartX, nStartY, nStartX + TERIS_SIZE, nStartY + TERIS_SIZE);
		}
	}

	nStartX = BOUND_SIZE * 2 + GAME_X * TERIS_SIZE;	
	nStartY = BOUND_SIZE;

	rect.left = nStartX + TERIS_SIZE;
	rect.right = nStartX + TERIS_SIZE * (INFO_X - 1);
	rect.top = nStartY + TERIS_SIZE * 6;
	rect.bottom = nStartY + TERIS_SIZE * 7;

	wsprintf(szBuf, L"�÷�:%d", g_Score);
	DrawText(hdc, szBuf, wcslen(szBuf), &rect, DT_CENTER | DT_VCENTER);

}

VOID DrawTeris(HDC hdc,int nStartX,int nStartY,BOOL bTeris[4][4])
{
	HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);

	for (int i = 0; i != 4; ++i){
		for (int j = 0; j != 4; ++j){
			if (bTeris[i][j]){
				Rectangle(hdc, BOUND_SIZE + (nStartX + j) * TERIS_SIZE,
					BOUND_SIZE + (nStartY + i) * TERIS_SIZE,
					BOUND_SIZE + (nStartX + j + 1) * TERIS_SIZE,
					BOUND_SIZE + (nStartY + i + 1) * TERIS_SIZE);
			}
		}
	}
}

VOID RotateTeris(BOOL bTeris[4][4])
{
	BOOL bNewTeris[4][4] = {};

	/*
	�˴����ĸ���˳ʱ����ת���飬�п��ºͿ��ҵ������
	����Ҫ��д����ʹ�俿���ϣ�����������ȥ�����¿��ҵ����
	*/
	//for (int x = 0; x != 4; ++x){
	//	for (int y = 0; y != 4; ++y){
	//		bNewTeris[x][y] = bTeris[3 - y][x];	//˳ʱ����ת����
	//	}
	//}
	//memcpy(bTeris, bNewTeris, sizeof(bNewTeris));	

	int x, y;
	int xPos, yPos;
	BOOL bFlag;	//�жϱ�־

	//ʹ�俿����ת
	for (x = 0, xPos = 0; x != 4; ++x){
		bFlag = FALSE;
		for (y = 0; y != 4; ++y){
			bNewTeris[xPos][y] = bTeris[3 - y][x];
			if (bNewTeris[xPos][y])
				bFlag = TRUE;		//��ʾ��һ�������� 
		}
		if (bFlag){
			xPos++;
		}
	}
	memset(bTeris, 0, sizeof(bNewTeris));

	//ʹ�俿����ת
	for (y = 0, yPos = 0; y != 4; ++y){
		bFlag = FALSE;
		for (x = 0; x != 4; ++x){
			bTeris[x][yPos] = bNewTeris[x][y];
			if (bTeris[x][yPos])
				bFlag = TRUE;
		}
		if (bFlag){
			yPos++;
		}
	}
	//memset(bTeris,0,)
	return;
}

BOOL CheckTeris(int nStartX, int nStartY, BOOL bTeris[4][4],BOOL bGame[GAME_X][GAME_Y])		//���߽�
{
	int x, y;

	//������ǽ
	if (nStartX < 0)
		return FALSE;
	
	for (x = 0; x != 4; ++x){
		for (y = 0; y != 4; ++y){
			if (bTeris[x][y]){
				//������ǽ
				if (nStartX + y >= GAME_X)
					return FALSE;
				//������ǽ
				if (nStartY + x >= GAME_Y)
					return FALSE;
				//�������з���
				if (bGame[nStartX + y][nStartY + x]){
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}


VOID RefreshTeris(int nStartX, int nStartY, BOOL bTeris[4][4], BOOL bGame[GAME_X][GAME_Y])
{
	BOOL bFlag;		//�ж�ĳһ���Ƿ�Ϊ����������������
	int newY;
	int iFullLine	= 0;	//��ȥ���еĸ��ӣ������¼����
	//�ϲ�bTerisb��bGame
	int x, y;
	for (x = 0; x != 4; ++x){
		for (y = 0; y != 4; ++y){
			if (bTeris[x][y])
				bGame[nStartX + y][nStartY + x] = TRUE;
		}
	}
	
	for (y = GAME_Y,newY = GAME_Y; y >= 0; --y){
		bFlag = FALSE;
		for (x = 0; x != 4; ++x){
			bGame[x][newY] = bGame[x][y];
			if (!bGame[x][y])		//��ʾ������
			{
				bFlag = TRUE;	
			}
		}
		if (bFlag)
			newY--;
		else
		{
			//�����������һ���滻��ǰ��
			iFullLine++;
		}
	}
	if (iFullLine)		//����ʱ��ȥ���鲢�ӷ�
	{
		g_Score += iFullLine * 10;
	}
	//�����µķ���
	memcpy(g_CurrTeris, g_NextTeris, sizeof(g_CurrTeris));
	memcpy(g_NextTeris, g_shape_Teris[GetRandNum(0, TERIS_TYPE)], sizeof(g_NextTeris));

	//ˢ��λ��
	TerisX = (GAME_X - 4) / 2;
	TerisY = 0;
}
VOID InitGame()
{
	//�����ʼλ�þ���
	TerisX = (GAME_X - 4) / 2;
	TerisY = 0;
	g_Speed = DEFAULT_INTERVAL;
	g_Score = 0;
	int iTemp = GetRandNum(0, TERIS_TYPE);		//����һ�������
	memcpy(g_CurrTeris, g_shape_Teris[iTemp], sizeof(g_CurrTeris));		//��ĳ��������״����Ϊ��ǰ����	

	iTemp = GetRandNum(0, TERIS_TYPE);
	memcpy(g_NextTeris, g_shape_Teris[iTemp], sizeof(g_NextTeris));	//��ĳ����������Ϊ��һ����

	memset(g_stGame, 0, sizeof(g_stGame));		//��ʼ��g_stGame
}
//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int nWinX, nWinY;	//���ڵĳ���
	int nClientX,nClientY;	//�ͻ����ĳ���
	RECT rect;

	BOOL bTempTeris[4][4];		//����һ����ʱ�ķ�������
	switch (message)
	{
	case WM_CREATE:
		//��ȡ���ڴ�С
		GetWindowRect(hWnd, &rect);
		nWinX = rect.right - rect.left;
		nWinY = rect.bottom - rect.top;
		
		//��ȡ�ͻ�����С
		GetClientRect(hWnd, &rect);
		nClientX = rect.right - rect.left;
		nClientY = rect.bottom - rect.top;

		MoveWindow(hWnd, 0, 0, 
			3 * BOUND_SIZE + (GAME_X + INFO_X) * TERIS_SIZE + (nWinX - nClientX),
			GAME_Y * TERIS_SIZE + 2 * BOUND_SIZE + (nWinY-nClientY),TRUE);
		
		//��ʼ���󣬷�����м� ����
		InitGame();
		SetTimer(hWnd, MY_TIMER, g_Speed,NULL);
	case WM_TIMER:
		if (CheckTeris(TerisX, TerisY + 1, g_CurrTeris, g_stGame)){
			TerisY++;
		}
		else{
			if (TerisY == 0){				//��ʾ�Ѿ�����������Ϸ����
				KillTimer(hWnd, MY_TIMER);
				MessageBox(NULL, L"��Ϸ����", L"��ʾ", MB_OK);
			}
			RefreshTeris(TerisX, TerisY, g_CurrTeris, g_stGame);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		RotateTeris(g_CurrTeris);				//����������ת����
		InvalidateRect(hWnd, NULL, NULL);		//�ú�����ָ���Ĵ����������һ������
		break;
	case WM_KEYDOWN:
		switch (wParam){
		case VK_LEFT:
			if (CheckTeris(TerisX - 1, TerisY, g_CurrTeris, g_stGame)){
				TerisX--;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_RIGHT:
			if (CheckTeris(TerisX + 1, TerisY, g_CurrTeris, g_stGame)){
				TerisX++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_UP:
			memcpy(bTempTeris, g_CurrTeris, sizeof(bTempTeris));		//�ȱ��浱ǰ����
			RotateTeris(bTempTeris);		//�ٽ��÷������
			if (CheckTeris(TerisX, TerisY, bTempTeris, g_stGame)){		
				memcpy(g_CurrTeris, bTempTeris, sizeof(g_CurrTeris));	//�����κ�ķ������¸���Ϊ��ǰ����
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_DOWN:
			while (CheckTeris(TerisX, TerisY + 1, g_CurrTeris, g_stGame)){
				TerisY++;
			}
			RefreshTeris(TerisX, TerisY, g_CurrTeris, g_stGame);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		DrawBackGround(hdc);			//������Ϸ������
		DrawInfo(hdc);				//������Ϣ��
		DrawTeris(hdc, TerisX, TerisY, g_CurrTeris);	//���Ʒ���
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, MY_TIMER);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
