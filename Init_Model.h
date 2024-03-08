using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT2 Tex;
};

XMFLOAT2 ScreenToCoord(int x, int y) { //window_Width
	XMFLOAT2 xmflt2;

	float window_Height_Half = window_Height / 2;
	float window_Width_Half = window_Width / 2;

	float relX = (x - window_Width_Half) / window_Width_Half;
	float relY = (window_Height_Half - y) / window_Height_Half;

	xmflt2.x = relX;
	xmflt2.y = relY;
	return xmflt2;
}

HRESULT Init_Model()
{
	HRESULT hr;
	// Create vertex buffer
	XMFLOAT2 xmflt1 = ScreenToCoord(125, 50 );
	XMFLOAT2 xmflt2 = ScreenToCoord(125+500, 50+500 );

	SimpleVertex vertices[] =
	{
		{XMFLOAT3(xmflt1.x, xmflt2.y, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(xmflt1.x, xmflt1.y, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(xmflt2.x, xmflt2.y, 0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(xmflt2.x, xmflt1.y, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
	};
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;


	WORD indices[] =
	{
		0,1,2,
		2,1,3
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	return S_OK;
}



