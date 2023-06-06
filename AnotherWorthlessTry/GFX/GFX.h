#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "..//Timer.h"

class GFX {
public: 
	bool Initialize(HWND hwnd, INT width, INT height);
	void RenderFrame();
	Camera camera;

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
	ConstantBuffer<CB_VS_vertexshader> constantBuffer;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indicesBuffer;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;

	Microsoft::WRL::ComPtr <ID3D11RasterizerState> resterazerState = nullptr;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	INT windowWidth = 0;
	INT windowHeight = 0;
	Timer fpsTimer;
};