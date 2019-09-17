#include <Windows.h>
#include "main.h"

HWND g_hWnd;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;


	case WM_KEYUP:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}


int WINAPI WinMain(HINSTANCE h,
	HINSTANCE p,
	LPTSTR cmd,
	int show)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,MsgProc,0,0,
	h,NULL,NULL,(HBRUSH)GetStockObject(WHITE_BRUSH),NULL,WINDOW_CLASS,NULL };

	RegisterClassEx(&wc);

	if (FULLSCREEN)
	{
		g_hWnd = CreateWindowEx(NULL,WINDOW_CLASS,WINDOW_TITLE,
			WS_POPUP|WS_SYSMENU|WS_VISIBLE,
			NULL,NULL,WIN_WIDTH,WIN_HEIGHT,NULL,NULL,h,NULL);

	
	}
	else 
	{
		g_hWnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			NULL, NULL, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, h, NULL);
	}

	if (g_hWnd)
	{
		ShowWindow(g_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(g_hWnd);
	}
	
	if (InitializeEngine())
	{
		if (GameInitialize())
		{
			MSG msg;
			ZeroMemory(&msg,sizeof(msg));
		
			while (msg.message != WM_QUIT)
			{
			
				if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else 
				{
					GameLoop();
				}
			}
		}
	}
	GameShutdown();
	ShutdownEngine();

	UnregisterClass(WINDOW_CLASS, h);
}

bool InitializeEngine()
{
	return true;
}


bool GameInitialize()
{
	return true;
}

void GameLoop()
{

}


void GameShutdown()
{

}

void ShutdownEngine()
{

}


