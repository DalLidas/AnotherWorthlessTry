#include "GFX.h"

bool GFX::Initialize(HWND hwnd, INT width, INT height)
{
	windowWidth = width;
	windowHeight = height;

	if (!InitializeDirectX11(hwnd, width, height))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;


	return true;
}

void GFX::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->resterazerState.Get());


	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	//this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);


	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer2.GetAddressOf(), &stride, &offset);
	this->deviceContext->Draw(3, 0);

	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer1.GetAddressOf(), &stride, &offset);
	this->deviceContext->Draw(3, 0);

	

	this->swapChain->Present(1, NULL);
}

bool GFX::InitializeDirectX11(HWND hwnd, INT width, INT height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapterData();

	if (adapters.size() < 1)
	{
		ExceptionLoger::ExceptionCall("IDXGI lost Adapters exception");
		return false;
	}

	//
	//https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc?f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(DXGI%252FDXGI_SWAP_CHAIN_DESC)%3Bk(DXGI_SWAP_CHAIN_DESC)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	//
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//
	//https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain?f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(D3D11%252FD3D11CreateDeviceAndSwapChain)%3Bk(D3D11CreateDeviceAndSwapChain)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	//
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		adapters[0].pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL, 
		NULL, 
		NULL, 
		0, 
		D3D11_SDK_VERSION,
		&scd, 
		this->swapChain.GetAddressOf(), 
		this->device.GetAddressOf(), 
		NULL, 
		this->deviceContext.GetAddressOf());

	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Created device and swapchain exception");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) 
	{
		ExceptionLoger::ExceptionCall(hr, "Get buffer exception");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr)) 
	{
		ExceptionLoger::ExceptionCall(hr, "Created render target view exception");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->windowWidth;
	depthStencilDesc.Height = this->windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ExceptionLoger::ExceptionCall(hr, "Created depth stencil buffer exception");
		return false;
	}

	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ExceptionLoger::ExceptionCall(hr, "Created depth stencil view exception");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Created depth stencil state exception");
		return false;
	}

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC resterazerDesc;
	ZeroMemory(&resterazerDesc, sizeof(resterazerDesc));

	resterazerDesc.FillMode = D3D11_FILL_SOLID;
	resterazerDesc.CullMode = D3D11_CULL_BACK;

	hr = this->device->CreateRasterizerState(&resterazerDesc, this->resterazerState.GetAddressOf());
	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Created rasterizer state exception");
		return false;
	}

	return true;
}

bool GFX::InitializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexShader.Initialize(this->device, L"VertexShader.cso", layout, numElements))
		return false;

	
	if (!pixelShader.Initialize(this->device,  L"PixelShader.cso"))
		return false;
	

	return true;
}

bool GFX::InitializeScene()
{
	//trig1
	Vertex v1[] =
	{
		Vertex(-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f), //Bottom Left Red Point
		Vertex( 0.0f,  0.5f, 0.0f, 1.0f ,0.0f, 0.0f), //Top Middle Green Point
		Vertex( 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f), //Bottom Right Blue Point
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v1);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v1;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer1.GetAddressOf());
	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Created vertex buffer exception");
		return false;
	}

	//trig2
	Vertex v2[] =
	{
		Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f), //Bottom Left Red Point
		Vertex(0.0f,   1.0f, 1.0f, 0.0f, 1.0f, 0.0f), //Top Middle Green Point
		Vertex(1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 0.0f), //Bottom Right Blue Point
	};

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v2);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v2;

	hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer2.GetAddressOf());
	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Created vertex buffer exception");
		return false;
	}

	return true;
}

