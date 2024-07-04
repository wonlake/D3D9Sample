//
// Copyright (C) Mei Jun 2011
//

#pragma once

#include <d3d9.h>
#include <d3dcompiler.h>
#include "Utility.h"

class ShaderHelper
{
public:
	
	ConstantTable VsConstantTable;
	ConstantTable PsConstantTable;
	LPDIRECT3DVERTEXSHADER9 lpVertexShader;
	LPDIRECT3DPIXELSHADER9	lpPixelShader;

	ShaderHelper()
	{
		lpVertexShader	  = NULL;
		lpPixelShader	  = NULL;
	}

	~ShaderHelper()
	{
		SAFE_RELEASE( lpVertexShader );
		SAFE_RELEASE( lpPixelShader );
	}
};

static ShaderHelper* CreateShaderFromFile( LPDIRECT3DDEVICE9 lpDevice, 
	LPCWSTR FilenameVS, LPCWSTR FileNamePS )
{
	if( FilenameVS == NULL && FileNamePS == NULL )
		return NULL;

	ShaderHelper* pShaderHelper = new ShaderHelper();
	ID3DBlob* lpShaderBuffer = NULL;
	ID3DBlob* lpErrorBuffer = NULL;

	if( FilenameVS )
	{
		D3DCompileFromFile( FilenameVS, 0, 0, "main", "vs_3_0", 0, 0, 
			&lpShaderBuffer, &lpErrorBuffer ); 

		if(lpErrorBuffer != NULL)
		{
			MessageBoxA( NULL, (char*)lpErrorBuffer->GetBufferPointer(), NULL, MB_OK);
			lpErrorBuffer->Release();
			lpErrorBuffer = NULL;
			return NULL;
		} 

		pShaderHelper->VsConstantTable.Create(lpShaderBuffer->GetBufferPointer());

		if( lpShaderBuffer != NULL)
		{
			lpShaderBuffer->Release();
			lpShaderBuffer = NULL;
		}
	}

	if( FileNamePS )
	{
		D3DCompileFromFile( FileNamePS, 0, 0, "main", "ps_3_0", 0, 0,
			&lpShaderBuffer, &lpErrorBuffer);

		if(lpErrorBuffer != NULL)
		{
			MessageBoxA( NULL, (CHAR*)(lpErrorBuffer->GetBufferPointer()), NULL, NULL );
			lpErrorBuffer->Release();
			lpErrorBuffer = NULL;
			return NULL;
		}

		pShaderHelper->PsConstantTable.Create(lpShaderBuffer->GetBufferPointer());

		lpDevice->CreatePixelShader(
			(DWORD*)lpShaderBuffer->GetBufferPointer(), &pShaderHelper->lpPixelShader );

		if( lpShaderBuffer != NULL)
		{		
			lpShaderBuffer->Release();
			lpShaderBuffer = NULL;
		}
	}

	return pShaderHelper;
}