#pragma once
//dont worry, this has a bit of anti-paste

#include <Windows.h>

#include <stdint.h>
#include <stdio.h>
#include "orender.h"
#include "easydraw.h"




void __cdecl Drawtestcallback(Render *render, Easydraw *draw)
{

	draw->Test();
	

}


void OnInject()
{
	if (!GetModuleHandleA("engine.dll") || !GetModuleHandleA("client.dll"))
		return;


	HWND gamewindow = FindWindow("Valve001", 0);
	if (!gamewindow)
	{
		MessageBox(NULL, "Error! [0]", "Tell this a developer!", MB_OK);
		return;
	}


	AllocConsole();
	SetConsoleTitleA("Easyrender_debug");

	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	render = new Render(gamewindow, 144);

	bool drawtest = false;

	if (GetAsyncKeyState(VK_F10))
	{
		drawtest = true;
	}
	drawtest = true;

	if (drawtest)
	{
		printf("Enabled Draw-Test!\n");
		render->SetRenderCallback(&Drawtestcallback);
		return;
	}



}

void OnUnload()
{
	delete render;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{

	if (DLL_PROCESS_ATTACH)
	{
		OnInject();
		return TRUE;
	}
	/*
	if (DLL_PROCESS_DETACH)
	{
		OnUnload();
		return TRUE;
	}*/
	return TRUE;
}
