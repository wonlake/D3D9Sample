//
// Copyright (C) Mei Jun 2011
//

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "Direct3D.h"
#include "Camera.h"
#include "ShaderUtility.h"
#include "Water.h"

#include "resource.h"

//全局变量
CDirect3D			g_Direct3D;
CTrackBallCamera	g_Camera;
DWORD				g_dwWindowWidth	 = 640;
DWORD				g_dwWindowHeight = 480;
Water*				g_pWater		 = NULL;

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
HWND SetupWindow( HINSTANCE hInstance, DWORD dwWidth, DWORD dwHeight );
VOID SetupD3DFramework( HWND hWnd , CDirect3D* lpDirect3D, DWORD dwWidth, DWORD dwHeight );
INT  SetupMessageLoop( CDirect3D *lpDirect3D );
VOID DrawScene( LPDIRECT3DDEVICE9 lpDevice );
BOOL LoadResources( LPDIRECT3DDEVICE9 lpDevice );
BOOL ReleaseResources( VOID );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
{
	HWND hWnd = SetupWindow( hInstance, g_dwWindowWidth, g_dwWindowHeight );
	
	if( hWnd == NULL )
		return 0;

	SetupD3DFramework( hWnd, &g_Direct3D, g_dwWindowWidth, g_dwWindowHeight );
	
	return SetupMessageLoop( &g_Direct3D );					
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	g_Camera.HandleMessage( hWnd, uMsg, wParam, lParam );

	switch( uMsg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			DestroyWindow( hWnd );
		break;
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	return 0;
}

HWND SetupWindow( HINSTANCE hInstance, DWORD dwWidth, DWORD dwHeight )
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = (HCURSOR)LoadCursor( NULL, IDC_ARROW );
	wc.hIcon		 = (HICON)LoadIcon( hInstance, (LPCTSTR)IDI_ICON1 );
	wc.hbrBackground = NULL;
	wc.lpfnWndProc	 = (WNDPROC)WndProc;
	wc.lpszClassName = TEXT("Direct3D9");
	wc.lpszMenuName  = NULL;

	if( !RegisterClass( &wc ) )
	{
		MessageBox( NULL, TEXT("注册窗口失败！"), NULL, MB_OK );
		return NULL;
	}

	RECT rc = { 0 };
	SetRect( &rc, 0, 0, dwWidth, dwHeight );        
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, false );

	HWND hWnd = CreateWindow( TEXT("Direct3D9"), 
		TEXT("Direct3D9Framework"), 
		WS_OVERLAPPEDWINDOW,
		100, 100, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL );

	if( !hWnd )
	{
		MessageBox( NULL, TEXT("创建窗口失败！"), NULL, MB_OK );
		return NULL;
	}

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	return hWnd;
}

VOID SetupD3DFramework( HWND hWnd , CDirect3D* lpDirect3D, 
					    DWORD dwWidth, DWORD dwHeight )
{
	g_Camera.Init( dwWidth, dwHeight );
	g_Camera.SetWheelUnit( 10.0f );
	lpDirect3D->Init3D( hWnd, dwWidth, dwHeight, TRUE, D3DDEVTYPE_HAL );
	lpDirect3D->SetupSceneMatrix( (float)dwWidth / (float)dwHeight );
	lpDirect3D->SetupResourcesCallback( LoadResources, ReleaseResources );
}

INT SetupMessageLoop( CDirect3D *lpDirect3D )
{
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			lpDirect3D->Render( DrawScene );
			Sleep( 10 );
		}
	}
	return 0;
}


VOID DrawScene( LPDIRECT3DDEVICE9 lpDevice )
{
	static XMMATRIX matIdentity = XMMatrixIdentity();
	DirectX::XMMATRIX matWorld, matView, matProj;
	DirectX::XMMATRIX matTrans;

	matWorld = DirectX::XMMatrixIdentity();
	matTrans = DirectX::XMMatrixTranslation(0.0f, -20.0f, 25.0f );
	XMMATRIX matV = matIdentity;		
	g_Camera.GetViewMatrix( (XMMATRIX*)&matV );
	memcpy( &matView, &matV, sizeof(matView) );
	matView *= matTrans;
	
	lpDevice->GetTransform( D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&matProj));


	lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(125, 0, 125), 1.0f, 0 );

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_pWater->RenderWater( lpDevice, &matWorld, &matView, &matProj );
}


BOOL LoadResources( LPDIRECT3DDEVICE9 lpDevice )
{
	g_pWater = new Water();
	g_pWater->Create( lpDevice, 100.0f, 100.0f );
	
	return TRUE;
}

BOOL ReleaseResources( VOID )
{		
	SAFE_DELETE( g_pWater );

	return TRUE;
}


