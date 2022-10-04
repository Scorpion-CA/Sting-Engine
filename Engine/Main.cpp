#include "Core/Window.h"
#include <iostream>
#include "Core/Config.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) { 
#ifdef _DEBUG
	FILE* m_conOut = stdout; //for debugging purposes
	AllocConsole();
	if (!freopen_s(&m_conOut, "CONOUT$", "w", stdout))
		std::cout << "Console Opened" << std::endl;
#endif // DEBUG

	Config::Get()->Init("Global.cfg");

	if (!Window::Get()->Init(hInst, hPrevInst, lpCmdLine, nCmdShow)) { 
		MessageBox(NULL, "Window Creation Failed", "Error", NULL); 
		return -1; 
	}
	
	return 0;
}
