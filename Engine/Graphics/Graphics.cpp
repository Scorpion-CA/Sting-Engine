#include "Graphics.h"
#include "../Core/Window.h"

#ifdef _DEBUG
#include <iostream>
#endif

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3d10.lib")

Graphics::Graphics(HWND hWnd) {
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pDevice,
		NULL,
		&m_pDevCon);

	if (hr != S_OK) {
#ifdef _DEBUG
		std::cout << "Direct3D Initialization Failed" << std::endl;
		std::cout << std::hex << hr;
#endif
		return;
	}

#ifdef _DEBUG
	if (m_pDevCon != nullptr && m_pDevice != nullptr && m_pSwapChain != nullptr)
		std::cout << "Direct3D Initialized" << std::endl;
#endif

	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBuffer);
	pBackBuffer->Release();

	m_pDevCon->OMSetRenderTargets(1, &m_pBackBuffer, NULL);

#ifdef _DEBUG
	if (m_pDevCon && m_pDevice && m_pSwapChain)
		std::cout << "Render Target Set" << std::endl;
#endif

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = Window::Get()->m_pResolution.x;
	vp.Height = Window::Get()->m_pResolution.y;

	m_pDevCon->RSSetViewports(1, &vp);

#ifdef _DEBUG
	if (m_pDevCon && m_pDevice && m_pSwapChain)
		std::cout << "Viewport Set" << std::endl;
#endif

	float col[4] = { 0.0f, 0.3f, 0.6f, 1.0f };
	Render(col);
}

void Graphics::Render(float col[4]) {
	m_pDevCon->ClearRenderTargetView(m_pBackBuffer, col);

	m_pSwapChain->Present(0, 0);
}

Graphics::~Graphics(void) {
	m_pSwapChain->Release();
	m_pDevice->Release();
	m_pDevCon->Release();
	m_pBackBuffer->Release();
}
