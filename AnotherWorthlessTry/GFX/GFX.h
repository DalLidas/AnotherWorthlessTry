#pragma once

#include "AdapterReader.h"
#include "Shaders.h"

class GFX {
public: 
	BOOL Initialization(HWND hwnd, INT width, INT height);
	void RenderFrame();

private:
	BOOL InitializeDirectX11(HWND hwnd, INT width, INT height);
	BOOL InitializeShaders();

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	
	VertexShader vertexShader;
	PixelShader pixelShader;
};