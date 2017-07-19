#include <windows.h>
#include "LinkList.h"
#include "Breaker.h"
#include <time.h>

extern PLIST list_block;
extern PLIST list_ball;
extern PLIST list_tool;
extern Breaker breaker;
extern double timer;
extern DWORD dwSorce;//�ܵ÷�

int n = 0;
int space_but = 0;
int left_but =0;
int right_but = 0;
int lig_beam = 0;

#define PX 20
#define TRANS_BK_COLOR		RGB(255,255,255)
//��ͬ����ש����ɫ
#define BLOCK_MAGIC_COLOR RGB(132,112,255)
#define BLOCK_TOOL_COLOR RGB(255,228,181)
#define BLOCK_IRON_COLOR RGB(190,190,190)
#define BLOCK_DIR_COLOR RGB(121,205,205)
#define BLOCK_COLOR RGB(255,106,106)

//��ͬ���������ɫ
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

	//�洢pen
	HPEN hOldPen;

	HBRUSH hOldBrush;
	//��ש�Ļ�ˢ
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
	//SetStretchBltMode(hdcmem, STRETCH_HALFTONE);//���ͼƬ����ʧ��

	/*StretchBlt(hdcmem,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		hdcmem,
		0, 0,
		0, 0,
		SRCCOPY);*/

	hpen = CreatePen(PS_NULL,0,RGB(255,255,255));
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	//��ש
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
	//��breaker
	hbrBreaker = CreateSolidBrush(COLOR_BREAKER);
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrBreaker);
	Rectangle(hdcmem,
		breaker.get_x(),
		breaker.get_y(),
		breaker.get_x()+breaker.get_length(),
		breaker.get_y()+BREAKER_HEIGHT);

	SelectObject(hdcmem,hOldBrush);
	DeleteObject(hbrBreaker);
	//���ӵ�
	draw_bullet(hdcmem);

	//��ball
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

	//��tool
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

	//���߽�
	hPenBoundary = CreatePen(PS_NULL,0,RGB(255,255,255));
	hOldPen = (HPEN)SelectObject(hdcmem, hPenBoundary);

	// ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
	MoveToEx(hdcmem, rect.left, rect.top, NULL);
	// ����һ��������ʾLineTo����
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
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;

	// ������Ϸ�߽�
	rect.left = 0;
	rect.top = 0;
	rect.right = 0 + MAX_X;
	rect.bottom = 0 + MAX_Y;

	// �����������ҽǵ�λ��
	ptLeftTop.x = rect.left;
	ptLeftTop.y = rect.top;
	ptRightBottom.x = rect.right;
	ptRightBottom.y = rect.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rect.bottom+40, //��������Ϣ������ʾ�ռ䡣
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
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{
	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon��Ա����ָ�����ڵ�ͼ��
	// ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor������꣬�������趨����������ʽ��
	// ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
	// ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.lpszMenuName = "MainMenu";
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, TEXT("��������classʧ��"), TEXT("����"), MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// �����������������Ѿ�ע���˵Ĵ�����
		"Game!!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
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
		// ����������Ӧ
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
		// ����������Ӧ
		break;
	}
	return 0;
}

LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
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
