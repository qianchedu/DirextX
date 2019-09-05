//#include  <Windows.h>
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib");
#pragma comment(lib,"d3dx9.lib");

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "Deme window"



LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = NULL;


bool InitializeD3D(HWND hWnd);
bool InitializeObjects();
void RenderScene();
void Shutdown();

struct stD3DVertex 
{
	float x, y, z, rhw;
	unsigned long color;

};

#define D3DFVF_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


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
		NULL,
		NULL,
		WINDOW_CLASS,
		NULL

	};


	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_TITLE,WS_OVERLAPPEDWINDOW,
		100,100,640,480,GetDesktopWindow(),NULL,hInst,NULL);


	if (InitializeD3D(hWnd))
	{

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
				//没有消息要处理，就可以做其他的一些工作
				RenderScene();		//渲染场景
			}
	
		}
	}

	Shutdown();
	UnregisterClass(WINDOW_CLASS, hInst);
}


bool InitializeD3D(HWND hWnd) 
{
	D3DDISPLAYMODE displayMode;
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_D3D == NULL) return false;

	if (FAILED(g_D3D ->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_D3DDevice)))
	{
		return false;
	}

	if (!InitializeObjects())
		return false;

	return true;
}


void Shutdown()
{
	if (g_D3DDevice != NULL)
		g_D3DDevice->Release();
	if (g_D3D != NULL)
		g_D3D->Release();
	if (g_VertexBuffer)g_VertexBuffer->Release();

	g_D3DDevice = NULL;
	g_D3D = NULL;
	g_VertexBuffer = NULL;
}


void RenderScene()
{
	g_D3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	g_D3DDevice->BeginScene();

	//输出3d图形
	g_D3DDevice->SetStreamSource(0, g_VertexBuffer,0,sizeof(stD3DVertex));

	g_D3DDevice->SetFVF(D3DFVF_VERTEX);
	//绘制两条线
	//g_D3DDevice->DrawPrimitive(D3DPT_LINELIST,0,2);
	//绘制两个点
	//g_D3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 2);

	//将点连接起来的线(线带)
	//g_D3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 2);

	//绘制三角形
	//g_D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	//绘制矩形
	g_D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_D3DDevice->EndScene();

	g_D3DDevice->Present(NULL,NULL,NULL,NULL);
}


bool InitializeObjects()
{
	unsigned long col = D3DCOLOR_XRGB(255, 255, 255);

	//线
	stD3DVertex objData[] =
	{
		{420.0f,150.0f,0.5f,1.0f,col,},
		{420.0f,350.0f,0.5f,1.0f,col,},
		{220.0f,150.0f,0.5f,1.0f,col,},
		{220.0f,350.0f,0.5f,1.0f,col,},
	};

	//stD3DVertex objData[] =
	//{
	//	{320.0f,150.0f,0,1,col,},
	//	{320.0f,350.0f,0,1,col,},
	//	{220.0f,350.0f,0,1,col,},
	//};

	if (FAILED(g_D3DDevice->CreateVertexBuffer(sizeof(objData), 0, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &g_VertexBuffer, NULL)))
	{
		return false;
	}


	void *ptr;

	if (FAILED(g_VertexBuffer->Lock(0, sizeof(objData), (void**)&ptr, 0)))
		return false;

	memcpy(ptr, objData,sizeof(objData));

	g_VertexBuffer->Unlock();
	return true;
}