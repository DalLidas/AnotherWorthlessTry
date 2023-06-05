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

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer1 = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer2 = nullptr;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;

	Microsoft::WRL::ComPtr <ID3D11RasterizerState> resterazerState = nullptr;

	INT windowWidth = 0;
	INT windowHeight = 0;
};