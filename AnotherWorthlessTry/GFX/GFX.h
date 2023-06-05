#pragma once

#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"

class GFX {
public: 
	bool Initialize(HWND hwnd, INT width, INT height);
	void RenderFrame();

private:
	bool InitializeDirectX11(HWND hwnd, INT width, INT height);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	
	VertexShader vertexShader;
	PixelShader pixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	Microsoft::WRL::ComPtr <ID3D11RasterizerState> resterazerState = nullptr;
};