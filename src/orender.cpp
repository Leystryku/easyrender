#pragma comment (lib, "Dwmapi.lib")
#pragma comment (lib, "Winmm.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")

#include <dwmapi.h>
#include <d3dx9.h>

#include <stdio.h>
#include <chrono>

#include "orender.h"
#include "easydraw.h"


int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf; // fix dxerr shit

Render *render = 0;



LRESULT CALLBACK Render__WindowProcRedirect(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == 0x0081)
	{
		SetWindowLongPtr((HWND)hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
		SetWindowPos((HWND)hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	}


	Render* currender = (Render*)GetWindowLongPtr(hwnd, GWLP_USERDATA);


	if (currender)
	{
		return (LONG_PTR)currender->OnWndProc(hwnd, message, (uintptr_t*)wparam, (int32_t*)lparam);
	}


	return DefWindowProc(hwnd, message, wparam, lparam);
}

unsigned long WINAPI RenderThreadStub(Render *currender)
{
	if (!currender) return -1;

	currender->OnRenderInit();

	while (1)
	{
		if ( !currender->GetWidth() || !currender->GetRenderThread()||currender->OnRender())
		{
			printf("Closing render thread...\n");
			break;
		}

	
	}

	void *vdevice = currender->GetD3DDevice();
	void *vinstance = currender->GetD3DInstance();


	auto device = (LPDIRECT3DDEVICE9)vdevice;
	auto instance = (LPDIRECT3D9)vinstance;


	device->Release();
	instance->Release();

	return 0;
}


Render::~Render()
{
	fps_limit = 0;
	width = 0;
	height = 0;
	callback = 0;
	window = 0;
	renderthread = 0;
	targetwindow = 0;

	TerminateThread(renderthread, 0);

	if (easydraw)
	{
		delete easydraw;
		easydraw = 0;
	}

	void *vdevice = d3d_device;
	void *vinstance = d3d_instance;

	if (vdevice)
	{

		auto device = (LPDIRECT3DDEVICE9)vdevice;
		device->Release();
	}

	if (vinstance)
	{
		auto instance = (LPDIRECT3D9)vdevice;
		instance->Release();
	}



}

static uint16_t render_renderid = 0;

Render::Render(void *target, uint8_t framespersecond)
{
	printf("Render::Render\n");

	renderid = render_renderid;
	render_renderid = render_renderid + 1;

	easydraw = 0;
	d3d_device = 0;
	d3d_instance = 0;

	fps = 0;
	fps_timelast = 0;
	fps_frames = 0;
	fps_limit = framespersecond;

	callback = 0;
	targetwindow = target;

	RECT targetrect;
	if (!GetWindowRect((HWND)targetwindow, &targetrect))
	{
		MessageBox(0, "ERROR, A-81", "Tell this a developer!", MB_OK);
		return;
	}

	uint16_t fwidth = (uint16_t)(targetrect.right - targetrect.left);
	uint16_t fheight = (uint16_t)(targetrect.bottom - targetrect.top);

	if (fwidth > 0xFFFF || fheight > 0xFFFF || 0xFF > fwidth || 0xFF > fheight)
	{
		MessageBox(0, "ERROR, A-2!\nGame appears to be in fullscreen mode!\nFix: Run it with -window -noborder!\n", "Game is in fullscreen mode!\n", MB_OK);
		return;
	}


	RECT targetrect_cl;
	GetClientRect((HWND)targetwindow, &targetrect_cl);

	int border_thickness = ((targetrect.right - targetrect.left) - (targetrect_cl.right - targetrect_cl.left)) ;
	int titlebarheight = ((targetrect.bottom - targetrect.top) - (targetrect_cl.bottom - targetrect_cl.top));


	int curwidth = targetrect.right - targetrect.left;
	int curheight = targetrect.bottom - targetrect.top;

	curwidth -= border_thickness;
	curheight -= titlebarheight;

	width = (uint16_t)curwidth;
	height = (uint16_t)curheight;




	printf("Initializing Render Thread...\n");
	renderthread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RenderThreadStub, this, 0, 0);

}

void *Render::OnWndProc(void *hwnd, uint32_t message, uintptr_t* wparam, int32_t *lparam)
{

	if (message == WM_ACTIVATE)
	{
		if (wparam != 0)
			SetForegroundWindow((HWND)targetwindow);



	}

	
	if(message == WM_CREATE)
	{
		MARGINS margins = { -1 };

		if (!FAILED(DwmExtendFrameIntoClientArea((HWND)hwnd, &margins)))
		{
			Sleep(2);
		}

		DWM_BLURBEHIND bb = { 0 };

		// Specify blur-behind and blur region.
		bb.dwFlags = DWM_BB_ENABLE;
		bb.fEnable = true;
		bb.hRgnBlur = NULL;

		// Enable blur-behind.
		DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
	};

	return (void*)DefWindowProc((HWND)hwnd, message, (WPARAM)wparam, (LPARAM)lparam);
}

void Render::OnRenderInit()
{

	printf("Initializing Render Window... [%hu:%hu]\n", width, height);



}


void* Render::OnRender()
{
	MSG msg = { 0 };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return 0;
	}

	if (!fps_limit)
	{
		return 0;
	}




	return 0;
}