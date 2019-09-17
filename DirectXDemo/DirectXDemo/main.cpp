//#include  <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib");
#pragma comment(lib,"d3dx9.lib");

#define WINDOW_CLASS    "UGPDX"
#define WINDOW_TITLE    "Deme window"
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480




LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;
//矩阵
//D3DXMATRIX g_ortho;
//D3DXMATRIX g_projection;

D3DXMATRIX g_projection;
D3DXMATRIX g_ViewMatrix;
//D3DXMATRIX g_translation;
//D3DXMATRIX g_rotation;

float g_angle = 0.0f;

LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = NULL;


bool InitializeD3D(HWND hWnd);
bool InitializeObjects();
void RenderScene();
void Shutdown();

struct stD3DVertex 
{
	//float x, y, z, rhw;
	float x, y, z;
	unsigned long color;

};

#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


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
		100,100,WINDOW_WIDTH,WINDOW_HEIGHT,GetDesktopWindow(),NULL,hInst,NULL);


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

	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);

	////创建一个矩阵
	//D3DXMatrixTranslation(&g_translation,0.0f,0.0f,3.0f);
	////沿着中心轴的Y轴旋转
	//D3DXMatrixRotationY(&g_rotation, g_angle);
	//g_D3DDevice->SetTransform(D3DTS_WORLD, &g_worldMatrix);

	//g_worldMatrix = g_rotation * g_translation;

	//g_angle += 0.01f;

	//if (g_angle >= 360) g_angle = 0.0f;


	//输出3d图形
	g_D3DDevice->SetStreamSource(0, g_VertexBuffer,0,sizeof(stD3DVertex));

	g_D3DDevice->SetFVF(D3DFVF_VERTEX);

	g_D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	

	g_D3DDevice->EndScene();

	g_D3DDevice->Present(NULL,NULL,NULL,NULL);
}


bool InitializeObjects()
{


	//创建一个左手坐标系正交投影矩阵。
	//D3DXMatrixOrthoLH(&g_ortho,WINDOW_WIDTH,WINDOW_HEIGHT,0.1f,1);
	D3DXMatrixPerspectiveFovLH(&g_projection,45.0f,WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,1000);

	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &g_projection);

	D3DXVECTOR3 cameraPos(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 lookAtPos(0.0f, 0.0f,0.0f);
	D3DXVECTOR3 upDir(0.0f,1.0f,0.0f);

	D3DXMatrixLookAtLH(&g_ViewMatrix, &cameraPos, &lookAtPos, &upDir);



	g_D3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	unsigned long col = D3DCOLOR_XRGB(255, 255, 255);

	

	stD3DVertex objData[] =
	{
		{ -0.3f,-0.3f,0.0f,D3DCOLOR_XRGB(255,255,0)},
		{  0.3f,-0.3f,0.0f,D3DCOLOR_XRGB(255,0,  0)},
		{  0.0f, 0.3f,0.0f,D3DCOLOR_XRGB(0,  0,255)},
	};




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