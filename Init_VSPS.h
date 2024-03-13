
ID3D11VertexShader* g_pVertexShader = nullptr;//���ؽ����̴�
ID3D11InputLayout* g_pVertexLayout = nullptr;//���ؽ����̾ƿ�
ID3D11PixelShader* g_pPixelShader = nullptr;//�ȼ� ���̴�

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


HRESULT Init_VertexShader()
{
    HRESULT hr;

	// Compile the vertex shader
	//���̴��� ���α׷� �����Ҷ����� ������ �����ʰ� �ؿ� �Լ� ȣ�⶧ ������
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"Shader02.fx", "VS", "vs_4_0", &pVSBlob);//������ pVSBlob�� ���ؽ����̴������� �Է¼��� Shader02.fx �� ���⼭�� ��������
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);//������ 
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Init_PixelShader()
{
	HRESULT hr;

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"Shader02.fx", "PS", "ps_4_0", &pPSBlob);//������ pVSBlob�� �ȼ����̴������� �Է�
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;

}


void Render_Shader()
{
	// Set the input layout
	pd3dContext->IASetInputLayout(g_pVertexLayout);         //���̾ƿ� ��ǲ
	pd3dContext->VSSetShader(g_pVertexShader, nullptr, 0);  //���ؽ� ���̴� 
	pd3dContext->PSSetShader(g_pPixelShader, nullptr, 0);   //���ؽ� ���̴�
}
