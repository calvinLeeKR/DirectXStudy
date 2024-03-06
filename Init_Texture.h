#include "tex\DDSTextureLoader.h"
#include "tex\WICTextureLoader.h"
using namespace DirectX;



HRESULT Init_Texture()
{
	HRESULT hr;

	hr = CreateWICTextureFromFile(pd3dDevice, L"1.bmp", nullptr, &g_pTextureRV);
	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

