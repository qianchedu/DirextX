#include  <Windows.h>

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "Deme window"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KEYUP:
		if (wParam = VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;


	default:
		break;
	}


	return DefWindowProc(hWnd, msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInst,
	HINSTANCE prevhInst,
	LPTSTR cmdLine,
	int show)
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0,
		0,
		hInst,
		NULL,
		NULL,
		(HBRUSH)GetStockObject(WHITE_BRUSH),
		NULL,
		WINDOW_CLASS,
		NULL

	};


	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_TITLE,WS_OVERLAPPEDWINDOW,
		100,100,640,480,GetDesktopWindow(),NULL,hInst,NULL);

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	UpdateWindow(hWnd);


	MSG msg;
	ZeroMemory(&msg,sizeof(msg));

	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			//û����ϢҪ�����Ϳ�����������һЩ����
		}
	
	}

	UnregisterClass(WINDOW_CLASS, hInst);
}