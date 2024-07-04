//
// Copyright (C) Mei Jun 2011
//

#include "Direct3D.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment(lib, "d3dcompiler.lib")

CDirect3D::CDirect3D()
{
	m_lpDevice			 = NULL;
	m_pLoadResources	 = NULL;
	m_pReleaseResources  = NULL;

	m_bWindowed			 = FALSE;
	m_bLoadResources     = FALSE;

	m_fRatio             = -1.0f;
}

CDirect3D::~CDirect3D()
{
	ReleaseResources();
	if( m_lpDevice!= NULL )
	{
		m_lpDevice->Release();
		m_lpDevice = NULL;
	}
}

BOOL CDirect3D::Init3D( HWND hWnd,
						DWORD dwWidth, DWORD dwHeight, 
						BOOL bWindowed, 
						D3DDEVTYPE DeviceType )
{
	HRESULT hr = 0;
	
	LPDIRECT3D9       lpD3D = NULL;

	lpD3D = Direct3DCreate9( D3D_SDK_VERSION );

	if( !lpD3D )
	{
		::MessageBox( NULL, TEXT("创建Direct3D对象失败！"), NULL, MB_OK );
		return FALSE;
	}

	D3DCAPS9 caps;
	lpD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, DeviceType, &caps );
	int vp = 0;
	if( caps.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
	d3dpp.BackBufferWidth			 = dwWidth;
	d3dpp.BackBufferHeight			 = dwHeight;
	d3dpp.BackBufferFormat			 = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount			 = 1;
	d3dpp.MultiSampleType			 = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		 = 0;
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				 = hWnd;
	d3dpp.Windowed					 = bWindowed;
	d3dpp.EnableAutoDepthStencil	 = TRUE;
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D24S8;
	d3dpp.Flags						 = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = lpD3D->CreateDevice( D3DADAPTER_DEFAULT,
						      DeviceType,
							  hWnd,
							  vp,
							  &d3dpp,
							  &m_lpDevice );

	if( FAILED(hr) )
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = lpD3D->CreateDevice( D3DADAPTER_DEFAULT,
								  DeviceType,
								  hWnd,
								  vp,
								  &d3dpp,
								  &m_lpDevice );
		if( FAILED(hr) )
		{
			lpD3D->Release();
			::MessageBox( NULL, TEXT("创建Direct3D设备失败！"), NULL, MB_OK );
			return FALSE;
		}
	}
	lpD3D->Release();
	
	return TRUE;
};

BOOL CDirect3D::SetupSceneMatrix( FLOAT fRatio )
{
	if( m_fRatio != fRatio )
		m_fRatio = fRatio;

	if( m_lpDevice == NULL )
		return FALSE;
	
	DirectX::XMMATRIX projMatrix =
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI * 0.5f,
								640.0f/480.0f,
								1.0f,
								1000.0f );
	//projMatrix = DirectX::XMMatrixTranspose(projMatrix);
	D3DMATRIX proj = *(D3DMATRIX*)&projMatrix;
	m_lpDevice->SetTransform( D3DTS_PROJECTION, &proj);
	return TRUE;
}

BOOL CDirect3D::SetupResourcesCallback( BOOL (*pLoadResources)( LPDIRECT3DDEVICE9 lpDevice ),
									    BOOL (*pReleaseResources)() )
{
	if( pLoadResources != NULL )
		m_pLoadResources = pLoadResources;
	if( pReleaseResources != NULL )
		m_pReleaseResources = pReleaseResources;
	return TRUE;
};

BOOL CDirect3D::Render( VOID (*pDrawScene)( LPDIRECT3DDEVICE9 lpDevice ) )
{
	if( m_lpDevice == NULL )
		return FALSE;

	if( !m_bLoadResources )
		if( !LoadResources() )
			return FALSE;

	m_lpDevice->Clear( 0, NULL, 
					 D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					 D3DCOLOR_XRGB(0, 0, 0),
					 1.0f, 0 );
	m_lpDevice->BeginScene();
	if( pDrawScene != NULL )
		pDrawScene( m_lpDevice );
	m_lpDevice->EndScene();
	m_lpDevice->Present( NULL, NULL, NULL, NULL );
	return TRUE;
}

BOOL CDirect3D::LoadResources( VOID )
{
	if( m_pLoadResources != NULL )
	{
		m_bLoadResources = m_pLoadResources( m_lpDevice );
		return m_bLoadResources;
	}
	m_bLoadResources = TRUE;	
	return TRUE;
}

BOOL CDirect3D::ReleaseResources( VOID )
{
	m_bLoadResources = FALSE;
	if( m_pReleaseResources != NULL )
		return m_pReleaseResources();
	return TRUE;
}

