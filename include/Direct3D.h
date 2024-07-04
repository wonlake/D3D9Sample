//
// Copyright (C) Mei Jun 2011
//

#pragma once

#include <d3d9.h>
#include <DirectXMath.h>

class CDirect3D
{
public:
	CDirect3D();
	virtual ~CDirect3D();

public:
    BOOL Init3D( HWND hWnd, 
				 DWORD dwWidth, DWORD dwHeight,  
				 BOOL bWindowed, 
				 D3DDEVTYPE DeviceType );
	BOOL SetupSceneMatrix( FLOAT fRatio );
	BOOL SetupResourcesCallback( BOOL (*pLoadResources)( LPDIRECT3DDEVICE9 lpDevice ),
						 		 BOOL (*pReleaseResources)() );
	BOOL Render( VOID (*pDrawScene)( LPDIRECT3DDEVICE9 lpDevice ) );

private:
	BOOL LoadResources( VOID );
	BOOL ReleaseResources( VOID );

private:
	LPDIRECT3DDEVICE9 m_lpDevice;
	BOOL             (*m_pLoadResources)( LPDIRECT3DDEVICE9 lpDevice );
	BOOL             (*m_pReleaseResources)();

	BOOL  m_bWindowed;
	BOOL  m_bLoadResources;
	FLOAT m_fRatio;
};