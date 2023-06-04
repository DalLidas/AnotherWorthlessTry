#include "GFX.h"

BOOL GFX::Initialization(HWND hwnd, INT width, INT height)
{
	if (!InitializeDirectX11(hwnd, width, height)) {
		ExceptionLoger::ExceptionCall(GetLastError(), "Initialization DirectX11 exception");
		return false;
	}

	return true;
}

BOOL GFX::InitializeDirectX11(HWND hwnd, INT width, INT height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapterData();

	return true;
}
