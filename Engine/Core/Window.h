#pragma once
#include <Windows.h>
#include <string>
#include "../Graphics/Graphics.h"

class Window {
public:
	bool Init(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow);
	HWND GetHWND();
	static Window* Get();
	

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

	friend class Config;
	friend class Graphics;

private:
	POINT m_pResolution = {1280, 720};

	HWND m_hwnd = NULL;
	Graphics* m_Graphics = nullptr;

	int test = 100;
	int test2 = 100;
	int test3 = 100;
};

