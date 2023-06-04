#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "../ExceptionLoger.h"

struct AdapterData {
	AdapterData(IDXGIAdapter* pAdapter);

	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC description;
};

class AdapterReader {
public:
	static std::vector<AdapterData> GetAdapterData();

private:
	static std::vector<AdapterData> adapters;
};