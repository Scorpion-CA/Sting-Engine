#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d10.h>

class Graphics {
public:
	Graphics(HWND hWnd);
	void Render(float col[4]);
	~Graphics(void);

	friend class Config;

private:
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDevCon = nullptr;
	ID3D11RenderTargetView* m_pBackBuffer = nullptr;
};

