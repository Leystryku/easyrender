#pragma once


#include <cstdint>


#define CUSTOM_FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE  | D3DFVF_TEX1

class Render
{
private:
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	int64_t fps;
	uint8_t fps_limit;
	uint32_t fps_frames;

	int64_t fps_timelast;
	
	void*callback;
	void*window;
	void*targetwindow;

	void* renderthread;

	void* d3d_instance;
	void* d3d_device;

	void* easydraw;

	uint16_t renderid;


public:
	Render(void *targetwindow, uint8_t framespersecond = 60);

	~Render();

	void SetRenderCallback(void* rendercallback)
	{
		callback = rendercallback;
	};

	void *GetRenderCallback()
	{
		return callback;
	}

	void* GetRenderThread()
	{
		return renderthread;
	}

	uint16_t GetWidth()
	{
		return width;
	}

	uint16_t GetHeight()
	{
		return height;
	}

	uint32_t GetFPSLimit()
	{
		return fps_limit;
	}

	void SetFPSLimit(uint8_t limit)
	{
		fps_limit = limit;
	}

	int64_t GetFPS()
	{
		return fps;
	}

	void OnRenderInit();
	void* OnRender();
	void* OnWndProc(void *hwnd, uint32_t message, uintptr_t* wparam, int32_t *lparam);

	void *GetD3DDevice()
	{
		return d3d_device;
	}


	void *GetD3DInstance()
	{
		return d3d_instance;
	}


	void *GetDraw()
	{
		return easydraw;
	}

	uint16_t GetRenderID()
	{
		return renderid;
	}
};

extern Render *render;
