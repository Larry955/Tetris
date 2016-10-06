// Teris.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Teris.h"
#define MAX_LOADSTRING 100

#define BOUND_SIZE 10	//游戏边界大小
#define TERIS_SIZE 30		//每个格子大小
#define GAME_X 10		//横向游戏区域大小，共10个格子
#define GAME_Y 20		//纵向游戏区域大小，共20个格子
#define INFO_X 6		//横向得分区域大小，共6个格子
#define INFO_Y GAME_Y	//纵向得分区域大小，共20个格子（和纵向游戏区域大小一致）
#define MY_TIMER 1
#define DEFAULT_INTERVAL	500	//默认下降速度，500毫秒下降一格

//俄罗斯方块的形状
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
#define TERIS_TYPE (sizeof(g_shape_Teris)/sizeof(g_shape_Teris[0]))		//表示方块的类型

BOOL g_CurrTeris[4][4];				//当前俄罗斯方块的形状
BOOL g_NextTeris[4][4];				//保存下一个俄罗斯方块的形状
BOOL g_stGame[GAME_X][GAME_Y];	//记录哪些位置有方块
UINT TerisX;	//方块左上角的x坐标,UINT是32位无符号整数
UINT TerisY;
UINT g_Speed;
UINT g_Score;


// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
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

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TERIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TERIS));

	// 主消息循环: 
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
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
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
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中
  
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   { 
	   MessageBox(NULL, TEXT("创建窗口失败"),TEXT("提示"),MB_OK);
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

INT	GetRandNum(int iMin, int iMax)
{
	//取随机数
	srand(GetTickCount() + g_Speed--);
	if (iMin > iMax)
		return -1;
	return iMin + rand() % (iMax - iMin); 
}

/*HDC是Windows的一种数据类型，是设备上下文描述句柄，
它包含了某个设备有关的汇指属性信息,这些对象封装了有关绘制线条、文本和形状的Windows API*/
VOID DrawBackGround(HDC hdc)	
{
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);

	HBRUSH hBrush1 = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//HBRUSH hB = (HBRUSH)CreateSolidBrush(RGB(0xff, 0, 0));

	//绘制游戏区
	Rectangle(hdc, BOUND_SIZE, BOUND_SIZE,
		BOUND_SIZE + GAME_X * TERIS_SIZE,
		BOUND_SIZE + GAME_Y * TERIS_SIZE); /*nLeftRect：指定矩形左上角的逻辑X坐标。
										nTopRect：指定矩形左上角的逻辑Y坐标。
										nRightRect：指定矩形右下角的逻辑X坐标。
										nBottomRect：指定矩形右下角的逻辑Y坐标。*/
	SelectObject(hdc, hPen);
	//绘制游戏区中的小方块
	//int x = 0, y = 0;
	//画一个小方块：Rectangle(hdc, BOUND_SIZE, BOUND_SIZE, BOUND_SIZE + TERIS_SIZE, BOUND_SIZE + TERIS_SIZE);
	for (int x = 0; x != GAME_X; ++x){		//x表示的是格子数，而Rectangle中的参数是位置，因此格子序号x*TERIS_SIZE后才是每个格子的距离
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
	RECT rect;		//绘制得分模块
	TCHAR szBuf[100] = {};	//缓冲区

	int nStartX,nStartY;	//用于画信息栏中的小方块
	Rectangle(hdc, BOUND_SIZE * 2 + GAME_X * TERIS_SIZE,
		BOUND_SIZE,
		BOUND_SIZE * 2 + (GAME_X + INFO_X) * TERIS_SIZE,
		BOUND_SIZE + GAME_Y * TERIS_SIZE);

	for (int x = 0; x != 4; ++x){
		for (int y = 0; y != 4; ++y){
			nStartX = BOUND_SIZE * 2 + GAME_X * TERIS_SIZE + (x + 1) * TERIS_SIZE;	//每隔小方块的左上角坐标
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

	wsprintf(szBuf, L"得分:%d", g_Score);
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
	此处沿四个角顺时针旋转方块，有靠下和靠右的情况，
	所以要重写代码使其靠左靠上，即设置条件去掉靠下靠右的情况
	*/
	//for (int x = 0; x != 4; ++x){
	//	for (int y = 0; y != 4; ++y){
	//		bNewTeris[x][y] = bTeris[3 - y][x];	//顺时针旋转方块
	//	}
	//}
	//memcpy(bTeris, bNewTeris, sizeof(bNewTeris));	

	int x, y;
	int xPos, yPos;
	BOOL bFlag;	//判断标志

	//使其靠上旋转
	for (x = 0, xPos = 0; x != 4; ++x){
		bFlag = FALSE;
		for (y = 0; y != 4; ++y){
			bNewTeris[xPos][y] = bTeris[3 - y][x];
			if (bNewTeris[xPos][y])
				bFlag = TRUE;		//表示这一列有数据 
		}
		if (bFlag){
			xPos++;
		}
	}
	memset(bTeris, 0, sizeof(bNewTeris));

	//使其靠左旋转
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

BOOL CheckTeris(int nStartX, int nStartY, BOOL bTeris[4][4],BOOL bGame[GAME_X][GAME_Y])		//检测边界
{
	int x, y;

	//碰到左墙
	if (nStartX < 0)
		return FALSE;
	
	for (x = 0; x != 4; ++x){
		for (y = 0; y != 4; ++y){
			if (bTeris[x][y]){
				//碰到右墙
				if (nStartX + y >= GAME_X)
					return FALSE;
				//碰到下墙
				if (nStartY + x >= GAME_Y)
					return FALSE;
				//碰到已有方块
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
	BOOL bFlag;		//判断某一行是否为满，用于消除方块
	int newY;
	int iFullLine	= 0;	//消去满行的格子，这里记录行数
	//合并bTerisb到bGame
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
			if (!bGame[x][y])		//表示不满格
			{
				bFlag = TRUE;	
			}
		}
		if (bFlag)
			newY--;
		else
		{
			//如果满格，用上一行替换当前行
			iFullLine++;
		}
	}
	if (iFullLine)		//满格时消去方块并加分
	{
		g_Score += iFullLine * 10;
	}
	//生成新的方块
	memcpy(g_CurrTeris, g_NextTeris, sizeof(g_CurrTeris));
	memcpy(g_NextTeris, g_shape_Teris[GetRandNum(0, TERIS_TYPE)], sizeof(g_NextTeris));

	//刷新位置
	TerisX = (GAME_X - 4) / 2;
	TerisY = 0;
}
VOID InitGame()
{
	//方块初始位置居中
	TerisX = (GAME_X - 4) / 2;
	TerisY = 0;
	g_Speed = DEFAULT_INTERVAL;
	g_Score = 0;
	int iTemp = GetRandNum(0, TERIS_TYPE);		//生成一个随机数
	memcpy(g_CurrTeris, g_shape_Teris[iTemp], sizeof(g_CurrTeris));		//将某个方块形状设置为当前方块	

	iTemp = GetRandNum(0, TERIS_TYPE);
	memcpy(g_NextTeris, g_shape_Teris[iTemp], sizeof(g_NextTeris));	//将某个方块设置为下一方块

	memset(g_stGame, 0, sizeof(g_stGame));		//初始化g_stGame
}
//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int nWinX, nWinY;	//窗口的长宽
	int nClientX,nClientY;	//客户区的长宽
	RECT rect;

	BOOL bTempTeris[4][4];		//定义一个临时的方块数组
	switch (message)
	{
	case WM_CREATE:
		//获取窗口大小
		GetWindowRect(hWnd, &rect);
		nWinX = rect.right - rect.left;
		nWinY = rect.bottom - rect.top;
		
		//获取客户区大小
		GetClientRect(hWnd, &rect);
		nClientX = rect.right - rect.left;
		nClientY = rect.bottom - rect.top;

		MoveWindow(hWnd, 0, 0, 
			3 * BOUND_SIZE + (GAME_X + INFO_X) * TERIS_SIZE + (nWinX - nClientX),
			GAME_Y * TERIS_SIZE + 2 * BOUND_SIZE + (nWinY-nClientY),TRUE);
		
		//初始化后，方块从中间 出现
		InitGame();
		SetTimer(hWnd, MY_TIMER, g_Speed,NULL);
	case WM_TIMER:
		if (CheckTeris(TerisX, TerisY + 1, g_CurrTeris, g_stGame)){
			TerisY++;
		}
		else{
			if (TerisY == 0){				//表示已经到顶部，游戏结束
				KillTimer(hWnd, MY_TIMER);
				MessageBox(NULL, L"游戏结束", L"提示", MB_OK);
			}
			RefreshTeris(TerisX, TerisY, g_CurrTeris, g_stGame);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		RotateTeris(g_CurrTeris);				//按鼠标左键旋转方块
		InvalidateRect(hWnd, NULL, NULL);		//该函数向指定的窗口区域更新一个矩形
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
			memcpy(bTempTeris, g_CurrTeris, sizeof(bTempTeris));		//先保存当前方块
			RotateTeris(bTempTeris);		//再将该方块变形
			if (CheckTeris(TerisX, TerisY, bTempTeris, g_stGame)){		
				memcpy(g_CurrTeris, bTempTeris, sizeof(g_CurrTeris));	//将变形后的方块重新复制为当前方块
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
		// 分析菜单选择: 
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
		// TODO:  在此添加任意绘图代码...
		DrawBackGround(hdc);			//绘制游戏区背景
		DrawInfo(hdc);				//绘制信息区
		DrawTeris(hdc, TerisX, TerisY, g_CurrTeris);	//绘制方块
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

// “关于”框的消息处理程序。
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
