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
#include "..//Point.h"

#include "DrawableObjects/TileObject.h"
#include "DrawableObjects/RhombObject.h"
#include "DrawableObjects/CubeObject.h"
#include "DrawableObjects/SphereObject.h"
#include "DrawableObjects/SkyBox.h"



class GFX {
public: 
	bool Initialize(HWND hwnd, INT width, INT height);
	void RenderFrame(const std::vector<Point>& points, const XMFLOAT3& border);
	Camera camera;

private:
	bool InitializeDirectX11(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	
	VertexShader vertexShader;
	PixelShader pixelShader;
	ConstantBuffer<CB_VS_vertexshader> constantBuffer;

	TileObject tileObject;
	RhombObject rhombObject;
	CubeObject cubeObject;
	SphereObject sphereObject;
	SkyBox skyBox;

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