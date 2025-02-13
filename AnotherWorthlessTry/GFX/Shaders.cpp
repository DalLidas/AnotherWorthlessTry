#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob((L"ShaderBin\\" + shaderpath).c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += shaderpath;
		ExceptionLoger::ExceptionCall(hr, errorMsg);
		return false;
	}

	hr = device->CreateVertexShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create vertex shader: ";
		errorMsg += shaderpath;
		ExceptionLoger::ExceptionCall(hr, errorMsg);
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numElements, this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		ExceptionLoger::ExceptionCall(hr, "Failed to create input layout.");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{
	HRESULT hr = D3DReadFileToBlob((L"ShaderBin\\" + shaderpath).c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += shaderpath;
		ExceptionLoger::ExceptionCall(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(this->shaderBuffer.Get()->GetBufferPointer(), this->shaderBuffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create pixel shader: ";
		errorMsg += shaderpath;
		ExceptionLoger::ExceptionCall(hr, errorMsg);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->shaderBuffer.Get();
}