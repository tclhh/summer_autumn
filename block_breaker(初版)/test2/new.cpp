#include <windows.h>
#include "LinkList.h"
#include "Breaker.h"
#include <time.h>

extern PLIST list_block;
extern PLIST list_ball;
extern PLIST list_tool;
extern Breaker breaker;
extern double timer;
extern DWORD dwSorce;//总得分

int n = 0;
int space_but = 0;
int left_but =0;
int right_but = 0;
int lig_beam = 0;

#define PX 20
#define TRANS_BK_COLOR		RGB(255,255,255)
//不同属性砖的颜色
#define BLOCK_MAGIC_COLOR RGB(132,112,255)
#define BLOCK_TOOL_COLOR RGB(255,228,181)
#define BLOCK_IRON_COLOR RGB(190,190,190)
#define BLOCK_DIR_COLOR RGB(121,205,205)
#define BLOCK_COLOR RGB(255,106,106)

//不同属性球的颜色
#define BALL_FLAME_COLOR RGB(255,0,0)
#define BALL_ICE_COLOR RGB(72,118,255)
#define BALL_LIGHT_COLOR RGB(255,215,0)
#define BALL_TIMER_COLOR RGB(154,50,205)
#define BALL_ELEC_COLOR RGB(176,196,222 )
#define BALL_COLOR RGB(28,28,28)

#define COLOR_TOOL RGB(127,255,0)
#define COLOR_BULLET  RGB(255,0,0)
#define COLOR_BREAKER RGB(28,28,28)


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR,int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void status_advance();
void init_all();
void draw_bullet(HDC);
void paint_all(HWND hwnd);
LONG OnKeyUp(HWND hwnd, UINT vk);
LONG OnKeydown(HWND hwnd, UINT vk);


RECT rect;

void init_all()
{
	srand( (unsigned)time( NULL ) );
	int i;
	for(i = 0;i < BLOCK_NUM;i ++)
		CreateBlock(2*(i%15)*PX, (i/15)*PX, BLOCK_LENGTH, BLOCK_HEIGHT);

	CreateBall(MAX_X/2,MAX_Y - BALL_LENGTH-BREAKER_HEIGHT,BALL_LENGTH,BALL_LENGTH);
}

void draw_bullet(HDC hdcmem)
{
	HPEN oldPen;
	HPEN hpen = CreatePen(PS_NULL,0,RGB(255,255,255));
	oldPen = (HPEN)SelectObject(hdcmem, hpen);
	if (lig_beam == 1&& breaker.getPowerNum() > 0){
		bullet_gen();
		MoveToEx(hdcmem, breaker.get_x()+BREAKER_LENGTH/2, 
			breaker.get_y(), NULL);
		LineTo(hdcmem,breaker.get_x()+BREAKER_LENGTH/2,bullet_gen());
		lig_beam = 0;
	}
	SelectObject(hdcmem,oldPen);
	DeleteObject(hpen);
}

void paint_all(HWND hwnd)
{
	int i,j,k;

	HPEN hpen,hPenBoundary;

	HDC hdc, hdcmem;
	HBITMAP hbmMem;

	HBITMAP bmp;
	/*HBITMAP BG_NONE,BG_ICE,BG_FLAME,BG_LIG,BG_ELEC,BG_TIMEER;*/

	//存储pen
	HPEN hOldPen;

	HBRUSH hOldBrush;
	//画砖的画刷
	HBRUSH hbrBlock;
	HBRUSH hbrBlockT;
	HBRUSH hbrBlockM;
	HBRUSH hbrBlockD;
	HBRUSH hbrBlockI;

	HBRUSH hbrBall;
	HBRUSH hbrBallE;
	HBRUSH hbrBallI;
	HBRUSH hbrBallL;
	HBRUSH hbrBallF;
	HBRUSH hbrBallT;

	HBRUSH hbrTool;
	HBRUSH hbrBreaker;
	HBRUSH hbrBoundary;

	GetClientRect(hwnd,&rect);
	hdc = GetDC(hwnd);
	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
	rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);
	/*hbrBackground =  CreateSolidBrush(RGB(255,255,255));*/

	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	/*BGN =(HBITMAP) LoadImage(NULL, "sky.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);*/

	/*if (BGN == NULL)
	{
		MessageBox(hwnd, "bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}*/

	/*SelectObject(hdcmem, BGN);*/
	//GetObject(BGN, sizeof(BITMAP), &bmp);
	//SetStretchBltMode(hdcmem, STRETCH_HALFTONE);//解决图片缩放失真

	/*StretchBlt(hdcmem,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		hdcmem,
		0, 0,
		0, 0,
		SRCCOPY);*/

	hpen = CreatePen(PS_NULL,0,RGB(255,255,255));
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	//画砖
	for(i = 0;i < BLOCK_NUM;i ++)
	{
		if (get_block_at(i)->get_block_type() == BLOCK_TYPE_NONE)
			continue;
		switch (get_block_at(i)->get_block_type())
		{
		case BLOCK_TYPE_BLOCK:
			hbrBlock = CreateSolidBrush(BLOCK_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBlock);
			break;
		case BLOCK_TYPE_MAGIC:
			hbrBlockM = CreateSolidBrush(BLOCK_MAGIC_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBlockM);
			break;
		case BLOCK_TYPE_IRON:
			hbrBlockI = CreateSolidBrush(BLOCK_IRON_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBlockI);
			break;
		case BLOCK_TYPE_TOOL:
			hbrBlockT = CreateSolidBrush(BLOCK_TOOL_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBlockT);
			break;
		case BLOCK_TYPE_DIR: 
			hbrBlockD = CreateSolidBrush(BLOCK_DIR_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBlockD);
			break;
		default:
			break;
		}

		Rectangle(hdcmem,
			get_block_at(i)->get_x(),
			get_block_at(i)->get_y(),
			get_block_at(i)->get_x()+BLOCK_LENGTH,
			get_block_at(i)->get_y()+BLOCK_HEIGHT);

		SelectObject(hdcmem, hOldBrush);

		switch (get_block_at(i)->get_block_type())
		{
		case BLOCK_TYPE_BLOCK:
			DeleteObject(hbrBlock);
			break;
		case BLOCK_TYPE_MAGIC:
			DeleteObject(hbrBlockM); 
			break;
		case BLOCK_TYPE_IRON:
			DeleteObject(hbrBlockI); 
			break;
		case BLOCK_TYPE_TOOL:
			DeleteObject(hbrBlockT);
			break;
		case BLOCK_TYPE_DIR: 
			DeleteObject(hbrBlockD);
			break;
		default:
			break;
		}
	}
	//画breaker
	hbrBreaker = CreateSolidBrush(COLOR_BREAKER);
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBreaker);
	Rectangle(hdcmem,
		breaker.get_x(),
		breaker.get_y(),
		breaker.get_x()+breaker.get_length(),
		breaker.get_y()+BREAKER_HEIGHT);

	SelectObject(hdcmem,hOldBrush);
	DeleteObject(hbrBreaker);
	//画子弹
	draw_bullet(hdcmem);

	//画ball
	for (j = 0;j < ListSize(list_ball);j++)
	{
		
		switch(get_ball_at(j)->get_ball_type())
		{
		case BALL_TYPE_LIG:
			hbrBallL = CreateSolidBrush(BALL_LIGHT_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBallL);
			break;
		case BALL_TYPE_FLAME:
			hbrBallF = CreateSolidBrush(BALL_FLAME_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBallF);
			break;
		case BALL_TYPE_ICE:
			hbrBallI = CreateSolidBrush(BALL_ICE_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBallI);
			break;
		case BALL_TYPE_ELEC:
			hbrBallE = CreateSolidBrush(BALL_ELEC_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBallE);
			break;
		case BALL_TYPE_TIMER:
			hbrBallT = CreateSolidBrush(BALL_TIMER_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBallT);
			break;
		case BALL_TYPE_NONE:
			hbrBall = CreateSolidBrush(BALL_COLOR);
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBall);
			break;
		}
		int a=get_ball_at(j)->get_x(),b=get_ball_at(j)->get_y();
		Ellipse(hdcmem,
			a,
			b,
			a+BALL_LENGTH,
			b+BALL_LENGTH);
		//Ellipse(hdcmem,
		//	get_ball_at(j)->get_x(),
		//	get_ball_at(j)->get_y(),
		//	get_ball_at(j)->get_x()+BALL_LENGTH,
		//	get_ball_at(j)->get_y()+BALL_LENGTH);
		SelectObject(hdcmem,hOldBrush);

		switch(get_ball_at(j)->get_ball_type())
		{
		case BALL_TYPE_LIG:
			DeleteObject(hbrBallL);
			break;
		case BALL_TYPE_FLAME:
			DeleteObject(hbrBallF);
			break;
		case BALL_TYPE_ICE:
			DeleteObject(hbrBallI);
			break;
		case BALL_TYPE_ELEC:
			DeleteObject(hbrBallE);
			break;
		case BALL_TYPE_TIMER:
			DeleteObject(hbrBallT);
			break;
		case BALL_TYPE_NONE:
			DeleteObject(hbrBall);
			break;
		}
	}

	//画tool
	for (k = 0;k < ListSize(list_tool);k ++){
		hbrTool = CreateSolidBrush(COLOR_TOOL);
		hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrTool);

		Ellipse(hdcmem,
			get_tool_at(k)->get_x(),
			get_tool_at(k)->get_y(),
			get_tool_at(k)->get_x()+TOOL_LENGTH,
			get_tool_at(k)->get_y()+TOOL_HEIGHT);

		SelectObject(hdcmem, hOldBrush);
		DeleteObject(hbrTool);
	}

	//画边界
	hPenBoundary = CreatePen(PS_NULL,0,RGB(255,255,255));
	hOldPen = (HPEN)SelectObject(hdcmem, hPenBoundary);

	// 将PEN移动到需要绘制的方框的左上角
	MoveToEx(hdcmem, rect.left, rect.top, NULL);
	// 画了一个方框。演示LineTo函数
	LineTo(hdcmem, rect.left, rect.bottom);
	LineTo(hdcmem, rect.right, rect.bottom);
	LineTo(hdcmem, rect.right, rect.top);
	LineTo(hdcmem, rect.left, rect.top);

	SelectObject(hdcmem, hOldPen);
	DeleteObject(hPenBoundary);

	BitBlt(hdc,
	0, 0, rect.right - rect.left, rect.bottom - rect.top,
	hdcmem, 0, 0, SRCCOPY);

	DeleteObject(hpen);
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	/*DeleteObject(BGN);*/

	ReleaseDC(hwnd, hdc);
}

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// 左上角
	POINT ptRightBottom;	// 右下角
	RECT rectWindow;

	// 设置游戏边界
	rect.left = 0;
	rect.top = 0;
	rect.right = 0 + MAX_X;
	rect.bottom = 0 + MAX_Y;

	// 计算上下左右角的位置
	ptLeftTop.x = rect.left;
	ptLeftTop.y = rect.top;
	ptRightBottom.x = rect.right;
	ptRightBottom.y = rect.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// 计算好了，设置窗口大小。
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
		rect.bottom+40, //给积分信息留出显示空间。
		TRUE);
}

void status_advance()
{
	int i;
	HWND hwnd;
	if (left_but == 1)
		breaker.LMoving();
	if(right_but == 1)
		breaker.RMoving();
	for (i = 0;i < ListSize(list_ball);i++)
	{
		get_ball_at(i)->Moving();
	}
	for (i = 0;i < ListSize(list_tool);i++)
	{
		get_tool_at(i)->tMoving();
		if (get_tool_at(i)->IsCatched())
			tool_effect(i);
	}
	tool_destroy();
	timer -= 0.01;
}
int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{
	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = "MainMenu";
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, TEXT("创建窗口class失败"), TEXT("错误！"), MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// 窗口类名，必须是已经注册了的窗口类
		"Game!!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);

	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
	&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	}

LONG OnKeyDown(WPARAM key,HWND hwnd)
{
	switch (key)
	{
	case VK_LEFT:
		left_but = 1;
		break;
	case VK_RIGHT:
		right_but = 1;
		break;
	case VK_SPACE:
		space_but = 1;
		break;
	default:
		// 其他键不响应
		break;
	}
	return 0;
}

LONG OnKeyUp(WPARAM w,HWND hwnd)
{
	switch (w)
	{
	case VK_LEFT:
		left_but = 0;
		break;
	case VK_RIGHT:
		right_but = 0;
		break;
	case VK_SPACE:
		space_but = 0;
		lig_beam = 1;
		break;
	default:
		// 其他键不响应
		break;
	}
	return 0;
}

LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		init_all();
		SetTimer(hwnd,1,10,NULL);
		ReSizeGameWnd(hwnd);
		break;

	case WM_PAINT:
		paint_all(hwnd);
		break;

	case WM_KEYDOWN:
		OnKeyDown(wParam,hwnd);
		break;

	case WM_KEYUP:
		OnKeyUp(wParam,hwnd);
		break;

	case WM_TIMER:
		status_advance();
		paint_all(hwnd);
		if (GameOver(list_ball) == YOULOST && n == 0)
		{
			n = 1;
			MessageBox(hwnd,"you lost","heh",MB_OK);
			ExitProcess(0);
		}
		else if (GameOver(list_ball) == YOUWIN && n == 0)
		{
			n = 1;
			MessageBox(hwnd,"you win","heh",MB_OK);
			ExitProcess(0);
		}
		break;

	case WM_DESTROY:
		ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}
