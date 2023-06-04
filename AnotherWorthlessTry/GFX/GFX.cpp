#include "GFX.h"

BOOL GFX::Initialization(HWND hwnd, INT width, INT height)
{
	if (!InitializeDirectX11(hwnd, width, height)) {
		ExceptionLoger::ExceptionCall(GetLastError(), "Initialization DirectX11 exception");
		return false;
	}

	return true;
}

void GFX::RenderFrame()
{
	float bgcolor[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->swapChain->Present(1, 0);
}

BOOL GFX::InitializeDirectX11(HWND hwnd, INT width, INT height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapterData();

	if (adapters.size() < 1) {
		ExceptionLoger::ExceptionCall("Lost adapter exception");
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
		this->deviceContext.GetAddressOf()
	);

	if (FAILED(hr)) {
		ExceptionLoger::ExceptionCall(hr, "Swap chain description exception");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr)) //If error occurred
	{
		ExceptionLoger::ExceptionCall(hr, "Create backBuffer exception");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());

	if (FAILED(hr)) {
		ExceptionLoger::ExceptionCall(hr, "Create target view exception");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);

	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = static_cast<FLOAT>(width);
	viewPort.Height = static_cast<FLOAT>(height);

	this->deviceContext->RSSetViewports(1, &viewPort);

	return true;
}

BOOL GFX::InitializeShaders()
{
	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Release\\";
#endif
#endif
	}
#pragma endregion

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexShader.Initialize(this->device, shaderfolder + L"vertexShader.cso", layout, numElements))
		return false;

	
	if (!pixelShader.Initialize(this->device, shaderfolder + L"pixelShader.cso"))
		return false;
	


	return true;
}
