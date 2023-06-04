#pragma once

#include "AdapterReader.h"

class GFX {
public: 
	BOOL Initialization(HWND hwnd, INT width, INT height);

private:
	BOOL InitializeDirectX11(HWND hwnd, INT width, INT height);

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
};