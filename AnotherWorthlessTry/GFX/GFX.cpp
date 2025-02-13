#include "GFX.h"

bool GFX::Initialize(HWND hwnd, INT width, INT height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->fpsTimer.Start();

	if (!InitializeDirectX11(hwnd))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void GFX::RenderFrame(const std::vector<Point>& points, BindMSG& imGuiMsg)
{
	static float bgcolor[] = { 76.0f / 255.0f, 86.0f / 255.0f, 106.0f / 255.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->resterazerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	

	//Draw SkyBox
	skyBox.SetPosition(imGuiMsg.sceneBorder.x / 2.0f, imGuiMsg.sceneBorder.y / 2.0f, imGuiMsg.sceneBorder.z / 2.0f);
	skyBox.SetScale(imGuiMsg.sceneBorder);


	skyBox.Draw(this->camera.GetViewMatrix() * this->camera.GetProjectionMatrix());

	////Draw Corners
	//for (int x = 0; x < 2; ++x) {
	//	for (int y = 0; y < 2; ++y) {
	//		for (int z = 0; z < 2; ++z) {
	//			cubeObject.SetPosition(x*border.x, y * border.y, z * border.z);
	//			cubeObject.Draw(this->camera.GetViewMatrix() * this->camera.GetProjectionMatrix());
	//		}
	//	}
	//}

	//Draw PointOfGod
	sphereObject.SetPosition(imGuiMsg.pointOfGod);
	sphereObject.SetScale(0.5f);
	sphereObject.Draw(camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::DarkSlateGray);

	//Draw Object
	for (auto point = points.begin(); point != points.end(); ++point) {
		//tileObject.Draw(this->camera.GetViewMatrix() * this->camera.GetProjectionMatrix
		sphereObject.SetPosition(point->pointPos);
		sphereObject.SetScale(point->radius);

		sphereObject.Draw(camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::White);
	}
	

	//Draw Text
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create ImGui Test Window
	bool my_tool_active = true;
	ImGui::Begin("Menu", &my_tool_active, ImGuiWindowFlags_None);

	//Set background color
	ImGui::ColorEdit3("Background color", bgcolor);

	//Set scene settings
	if (ImGui::CollapsingHeader("Scene settings"))
	{
		static float* pog[]{ &imGuiMsg.pointOfGod.x, &imGuiMsg.pointOfGod.y, &imGuiMsg.pointOfGod.z };
		ImGui::SliderFloat3("Position of spawn point", pog[0], 0.0f, imGuiMsg.sceneBorder.x);

		static float* vel[]{ &imGuiMsg.point.velosity.x, &imGuiMsg.point.velosity.y, &imGuiMsg.point.velosity.z };
		ImGui::SliderFloat3("Velosity of point", vel[0], -100.0f, 100.0f,"%.3f", 1.0f);

		static float* acc[]{ &imGuiMsg.point.acceleration.x, &imGuiMsg.point.acceleration.y, &imGuiMsg.point.acceleration.z };
		ImGui::SliderFloat3("Acceleration of point", acc[0], -100.0f, 100.0f, "%.3f", 1.0f);

		ImGui::InputFloat("Radius", &imGuiMsg.point.radius, 0.1f, 1.0f, 1);

		ImGui::Checkbox("Moveable", &imGuiMsg.point.moveState);
		ImGui::Checkbox("Create gravity", &imGuiMsg.point.createGravityState);

		if (imGuiMsg.createState) imGuiMsg.createState = false;
		if (ImGui::Button("Create point"))
			imGuiMsg.createState = true;

		if (imGuiMsg.createRandomState) imGuiMsg.createRandomState = false;
		if (ImGui::Button("Create random point"))
			imGuiMsg.createRandomState = true;

		if (imGuiMsg.createState) imGuiMsg.destroyState = false;
		if (ImGui::Button("Delete all points"))
			imGuiMsg.destroyState = true;

		static float* border[]{ &imGuiMsg.sceneBorder.x, &imGuiMsg.sceneBorder.y, &imGuiMsg.sceneBorder.z };
		ImGui::SliderFloat3("Scene border", border[0], 0.0f, 100.0f);
	}

	//Set scphysics settings
	if (ImGui::CollapsingHeader("Physics settings"))
	{
		ImGui::Checkbox("Physics state", &imGuiMsg.phsicsState);
		ImGui::Checkbox("Gravity state", &imGuiMsg.gAccelerationState);

		ImGui::Checkbox("Colide dicrement state", &imGuiMsg.bounceDicrimentState);
		ImGui::SliderFloat("dicrement", &imGuiMsg.bounceDicrement, 0.0f, 100.0f);

		ImGui::Checkbox("Air resistance state", &imGuiMsg.airResistanceState);
		ImGui::SliderFloat("Air resistance dicrement", &imGuiMsg.airResistanceDicrement, 0.0f, 100.0f);

		ImGui::InputFloat("Movement multeplier", &imGuiMsg.movMultiplier, 0.1f, 1.0f, 1);
		ImGui::InputFloat("Acceleration multeplier", &imGuiMsg.accMultiplier, 0.1f, 1.0f, 1); 
		ImGui::InputFloat("Gravity multeplier", &imGuiMsg.CreatedAccMultiplier, 0.1f, 1.0f, 1);
	}
	
	ImGui::End();
	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	this->swapChain->Present(0, NULL);
}

bool GFX::InitializeDirectX11(HWND hwnd)
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

	scd.BufferDesc.Width = this->windowWidth;
	scd.BufferDesc.Height = this->windowHeight;
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
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

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
	viewport.Width = static_cast<FLOAT>(this->windowWidth);
	viewport.Height = static_cast<FLOAT>(this->windowHeight);
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

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Resource/Font/comic_sans_ms_16.spritefont");

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

	//tileObject.Initialize(this->device.Get(), this->deviceContext.Get(), this->constantBuffer);
	//rhombObject.Initialize(this->device.Get(), this->deviceContext.Get(), this->constantBuffer);
	//cubeObject.Initialize(this->device.Get(), this->deviceContext.Get(), this->constantBuffer);
	skyBox.Initialize(this->device.Get(), this->deviceContext.Get(), this->constantBuffer);

	sphereObject.Initialize(this->deviceContext.Get());


	//Initialize Constant Buffer(s)
	HRESULT hr = this->constantBuffer.Initialize(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Failed to initialize constant buffer.");
		return false;
	}

	camera.SetPosition(0.0f, 0.0f, -2.0f);
	camera.SetProjectionValues(90.0, static_cast<FLOAT>(this->windowWidth) / static_cast<FLOAT>(this->windowHeight), 0.1f, 1000.0f);

	return true;
}

