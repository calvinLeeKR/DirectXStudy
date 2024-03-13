#include"DDSTextureLoader.h"
#include"tex\WICTextureLoader.h"
using namespace DirectX;

ID3D11ShaderResourceView* g_pTextureRV = nullptr;
ID3D11SamplerState* g_pSamplerLinear = nullptr;

HRESULT Init_Texture()
{
	HRESULT hr;

	//ID3D11ShaderResourceView* G_pTextureRV
	//hr = CreateWICTextureFromFile(pd3dDevice, L"2.png", nullptr, &g_pTextureRV);
	hr = CreateDDSTextureFromFile(pd3dDevice, L"seafloor.dds", nullptr, &g_pTextureRV);
	if (FAILED(hr))
		return hr;
	ID3D11Resource* pResource = nullptr;
	g_pTextureRV->GetResource(&pResource);
	{
		D3D11_TEXTURE2D_DESC desc;
		((ID3D11Texture2D*)pResource)->GetDesc(&desc);
		img_Width = desc.Width;
		img_Height = desc.Height;
	}

	pResource->Release();


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


HRESULT Init_BlendState()

{

	D3D11_BLEND_DESC desc = {};

	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//합성

	//알파값 정의
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	//
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	//
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = pd3dDevice->CreateBlendState(&desc, &g_pBlendState);

	return hr;

}

void Render_Texture()
{
	// Texture
	pd3dContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	pd3dContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
}