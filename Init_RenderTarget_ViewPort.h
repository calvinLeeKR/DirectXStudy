


HRESULT Init_RenderTarget_ViewPort()
{
	HRESULT hr = S_OK;

	// render target
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	pd3dContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);




    // Setup the viewport
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 RECT rc;
     GetClientRect(gWnd, &rc);
     UINT width = rc.right - rc.left;
     UINT height = rc.bottom - rc.top;

	 D3D11_VIEWPORT vp;
	 vp.TopLeftX = 0;
	 vp.TopLeftY = 0;
	 vp.Width = (FLOAT)width;
	 vp.Height = (FLOAT)height;
	 vp.MinDepth = 0.0f;//zÃà
	 vp.MaxDepth = 1.0f;//zÃà
	 pd3dContext->RSSetViewports(1, &vp);




     return S_OK;
}

