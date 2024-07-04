//
// Copyright (C) Mei Jun 2011
//

#pragma once

#include <d3d9.h>
#include "ShaderUtility.h"
#include <DirectXMath.h>

class Plane
{
public:
	Plane();
	~Plane();

	BOOL CreatePlaneX( LPDIRECT3DDEVICE9 lpDevice, float yLength, float zLength,
		float xOffset = 0, float yOffset = 0, float zOffset = 0 );

	BOOL CreatePlaneY( LPDIRECT3DDEVICE9 lpDevice, float xLength, float zLength,
		float xOffset = 0, float yOffset = 0, float zOffset = 0 );

	BOOL CreatePlaneZ( LPDIRECT3DDEVICE9 lpDevice, float xLength, float yLength,
		float xOffset = 0, float yOffset = 0, float zOffset = 0 );

	BOOL CreateVertexDeclaration( LPDIRECT3DDEVICE9 lpDevice );

	VOID Render( LPDIRECT3DDEVICE9 lpDevice );

public:

	UINT							m_uiElementSize;
	UINT							m_uiNumIndices;
	UINT							m_uiNumVertices;

	LPDIRECT3DVERTEXDECLARATION9	m_lpVertexDeclaration;
	LPDIRECT3DVERTEXBUFFER9			m_lpVertexBuffer;
	LPDIRECT3DINDEXBUFFER9			m_lpIndexBuffer;

	struct CUSTOMVERTEX
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};
};

class Water : public Plane
{
public:
	class WaterShader : public ShaderHelper
	{
	public:
		BOOL CreateShaders( LPDIRECT3DDEVICE9 lpDevice );
	};

public:
	Water();
	~Water();

	BOOL Create( LPDIRECT3DDEVICE9 lpDevice, float xLength, float yLength,
		float xOffset = 0, float yOffset = 0, float zOffset = 0 );	

	BOOL CreateTextures( LPDIRECT3DDEVICE9 lpDevice );

	VOID RenderWater( LPDIRECT3DDEVICE9 lpDevice, 
		DirectX::XMMATRIX* pWorld, DirectX::XMMATRIX* pView, DirectX::XMMATRIX* pProj );

	LPDIRECT3DTEXTURE9		m_lpNormalMap;
	LPDIRECT3DCUBETEXTURE9  m_lpSceneMap;
	WaterShader*			m_pWaterShader;
	UINT					m_uiNormalMap;
	UINT					m_uiSceneMap;
};