//
// Copyright (C) Mei Jun 2011
//

#include "Water.h"
#include "Utility.h"
#include "data.h"

Plane::Plane()
{
	m_uiElementSize = 0;
	m_uiNumIndices  = 0;
	m_uiNumVertices = 0;

	m_lpVertexBuffer		= NULL;
	m_lpIndexBuffer			= NULL;
	m_lpVertexDeclaration	= NULL;
}

Plane::~Plane()
{
	SAFE_RELEASE( m_lpIndexBuffer );
	SAFE_RELEASE( m_lpVertexBuffer );
	SAFE_RELEASE( m_lpVertexDeclaration );
}

BOOL Plane::CreatePlaneX( LPDIRECT3DDEVICE9 lpDevice, float yLength, float zLength,
	float xOffset, float yOffset, float zOffset )
{
	if( lpDevice == NULL )
		return FALSE;

	m_uiElementSize = sizeof( CUSTOMVERTEX );
	m_uiNumIndices	= 6;
	m_uiNumVertices = 4;

	CUSTOMVERTEX vertices[4] =
	{
		{ 0.0f,  yLength / 2.0f,  zLength / 2.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0 },
		{ 0.0f,  yLength / 2.0f, -zLength / 2.0f, 1.0f, 0.0f, 0.0f, 1.0, 0.0 },
		{ 0.0f, -yLength / 2.0f,  zLength / 2.0f, 1.0f, 0.0f, 0.0f, 0.0, 1.0 },
		{ 0.0f, -yLength / 2.0f, -zLength / 2.0f, 1.0f, 0.0f, 0.0f, 1.0, 1.0 },
	};

	for( int i = 0; i < 4; ++i )
	{
		vertices[i].x += xOffset;
		vertices[i].y += yOffset;
		vertices[i].z += zOffset;
	}

	HRESULT hr = lpDevice->CreateVertexBuffer( m_uiElementSize * m_uiNumVertices, 0, 0, 
		D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL );
	if( FAILED(hr) )
		return FALSE;

	LPVOID pVertexData = NULL;
	m_lpVertexBuffer->Lock( 0, m_uiElementSize * m_uiNumVertices, &pVertexData, 0 );
	memcpy( pVertexData, vertices, sizeof(vertices) );
	m_lpVertexBuffer->Unlock();

	USHORT indices[6] =
	{
		0, 1, 2,
		2, 1, 3,
	};

	hr = lpDevice->CreateIndexBuffer( sizeof(indices), 0, D3DFMT_INDEX16, 
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL );
	if( FAILED(hr) )
		return FALSE;

	LPVOID pIndexData = NULL;
	m_lpIndexBuffer->Lock( 0, sizeof(indices), &pIndexData, 0 );
	memcpy( pIndexData, indices, sizeof(indices) );
	m_lpIndexBuffer->Unlock();

	return TRUE;
}

BOOL Plane::CreatePlaneY( LPDIRECT3DDEVICE9 lpDevice, float xLength, float zLength,
	float xOffset, float yOffset, float zOffset )
{
	if( lpDevice == NULL )
		return FALSE;

	m_uiElementSize = sizeof( CUSTOMVERTEX );
	m_uiNumIndices	= 6;
	m_uiNumVertices = 4;

	CUSTOMVERTEX vertices[4] =
	{
		{ -xLength / 2.0f, 0.0f,  zLength / 2.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.0 },
		{  xLength / 2.0f, 0.0f,  zLength / 2.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0 },
		{ -xLength / 2.0f, 0.0f, -zLength / 2.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0 },
		{  xLength / 2.0f, 0.0f, -zLength / 2.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0 },
	};

	for( int i = 0; i < 4; ++i )
	{
		vertices[i].x += xOffset;
		vertices[i].y += yOffset;
		vertices[i].z += zOffset;
	}

	HRESULT hr = lpDevice->CreateVertexBuffer( m_uiElementSize * m_uiNumVertices, 0, 0, 
		D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL );
	if( FAILED(hr) )
		return FALSE;

	LPVOID pVertexData = NULL;
	m_lpVertexBuffer->Lock( 0, m_uiElementSize * m_uiNumVertices, &pVertexData, 0 );
	memcpy( pVertexData, vertices, sizeof(vertices) );
	m_lpVertexBuffer->Unlock();

	USHORT indices[6] =
	{
		0, 1, 2,
		2, 1, 3,
	};

	hr = lpDevice->CreateIndexBuffer( sizeof(indices), 0, D3DFMT_INDEX16, 
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL );
	if( FAILED(hr) )
		return FALSE;

	LPVOID pIndexData = NULL;
	m_lpIndexBuffer->Lock( 0, sizeof(indices), &pIndexData, 0 );
	memcpy( pIndexData, indices, sizeof(indices) );
	m_lpIndexBuffer->Unlock();

	return TRUE;
}

BOOL Plane::CreatePlaneZ( LPDIRECT3DDEVICE9 lpDevice, float xLength, float yLength,
	float xOffset, float yOffset, float zOffset )
{
	if( lpDevice == NULL )
		return FALSE;

	m_uiElementSize = sizeof( CUSTOMVERTEX );
	m_uiNumIndices	= 6;
	m_uiNumVertices = 4;

	CUSTOMVERTEX vertices[4] =
	{
		{ -xLength / 2.0f,  yLength / 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 0.0 },
		{  xLength / 2.0f,  yLength / 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 0.0 },
		{ -xLength / 2.0f, -yLength / 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 1.0 },
		{  xLength / 2.0f, -yLength / 2.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 1.0 },
	};

	for( int i = 0; i < 4; ++i )
	{
		vertices[i].x += xOffset;
		vertices[i].y += yOffset;
		vertices[i].z += zOffset;
	}

	HRESULT hr = lpDevice->CreateVertexBuffer( m_uiElementSize * m_uiNumVertices, 0, 0, 
		D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL );
	if( FAILED(hr) )
		return FALSE;

	LPVOID pVertexData = NULL;
	m_lpVertexBuffer->Lock( 0, m_uiElementSize * m_uiNumVertices, &pVertexData, 0 );
	memcpy( pVertexData, vertices, sizeof(vertices) );
	m_lpVertexBuffer->Unlock();

	USHORT indices[6] =
	{
		0, 1, 2,
		2, 1, 3,
	};

	hr = lpDevice->CreateIndexBuffer( sizeof(indices), 0, D3DFMT_INDEX16, 
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL );
	if( FAILED(hr) )
		return FALSE;

	LPVOID pIndexData = NULL;
	m_lpIndexBuffer->Lock( 0, sizeof(indices), &pIndexData, 0 );
	memcpy( pIndexData, indices, sizeof(indices) );
	m_lpIndexBuffer->Unlock();

	return TRUE;
}

BOOL Plane::CreateVertexDeclaration( LPDIRECT3DDEVICE9 lpDevice )
{
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0	},
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	HRESULT hr = lpDevice->CreateVertexDeclaration( decl, &m_lpVertexDeclaration );

	if( FAILED(hr) )
		return FALSE;
	else
		return TRUE;
}

VOID Plane::Render( LPDIRECT3DDEVICE9 lpDevice )
{
	if( m_lpVertexDeclaration )
	{
		lpDevice->SetVertexDeclaration( m_lpVertexDeclaration );
		lpDevice->SetIndices( m_lpIndexBuffer );
		lpDevice->SetStreamSource( 0, m_lpVertexBuffer, 0, m_uiElementSize );
		lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_uiNumVertices, 0, 
			m_uiNumIndices / 3 );
	}
	else
	{
		CreateVertexDeclaration( lpDevice );
	}
}

Water::Water()
{
	m_lpNormalMap	= NULL;
	m_lpSceneMap	= NULL;
	m_pWaterShader	= NULL;
	m_uiNormalMap	= -1;
	m_uiSceneMap	= -1;
}

Water::~Water()
{
	SAFE_RELEASE( m_lpNormalMap );
	SAFE_RELEASE( m_lpSceneMap );
	SAFE_DELETE( m_pWaterShader );
}

BOOL Water::Create( LPDIRECT3DDEVICE9 lpDevice, float xLength, float yLength,
	float xOffset, float yOffset, float zOffset )
{
	if( lpDevice == NULL )
		return FALSE;

	DWORD height	= 100;
	DWORD width		= 100;
	m_uiNumVertices = height * width;
	m_uiElementSize = sizeof( CUSTOMVERTEX );
	m_uiNumIndices	= (height - 1) * (width - 1) * 6;

	CUSTOMVERTEX* pVertex = new CUSTOMVERTEX[m_uiNumVertices];
	USHORT* pIndices	 = new USHORT[m_uiNumIndices];
	USHORT *p			 = pIndices; 

	for( UINT i = 0; i < height; ++i )
	{
		for( UINT j = 0; j < width; ++j )
		{
			pVertex[i * width + j].x = 1.0f * (j - width / 2.0f);
			pVertex[i * width + j].y = 0.0f;
			pVertex[i * width + j].z = 1.0f * (width - i - 1);
			pVertex[i * width + j].u = 1.0f - (float)j / (float)height;
			pVertex[i * width + j].v = (float)i / (float)width;
		}
	}

	for( UINT i = 0; i < height - 1; ++i )
	{
		for( UINT j = 0; j < width - 1; ++j )
		{
			*p++ = (USHORT)(i*width + j);
			*p++ = (USHORT)(i*width + j + 1); 
			*p++ = (USHORT)((i + 1)*width + j);
			*p++ = (USHORT)((i + 1)*width + j);
			*p++ = (USHORT)(i*width + j + 1); 
			*p++ = (USHORT)((i + 1)*width + j + 1);
		}
	}


	for( UINT i = 0; i < m_uiNumVertices; ++i )
	{
		pVertex[i].x += xOffset;
		pVertex[i].y += yOffset;
		pVertex[i].z += zOffset;
	}

	HRESULT hr = lpDevice->CreateVertexBuffer( m_uiElementSize * m_uiNumVertices, 0, 0, 
		D3DPOOL_MANAGED, &m_lpVertexBuffer, NULL );
	if( FAILED(hr) )
	{
		SAFE_DELETEARRAY(pVertex);
		SAFE_DELETEARRAY(pIndices);
		return FALSE;
	}
	LPVOID pVertexData = NULL;
	m_lpVertexBuffer->Lock( 0, m_uiElementSize * m_uiNumVertices, &pVertexData, 0 );
	memcpy( pVertexData, pVertex, m_uiElementSize * m_uiNumVertices );
	m_lpVertexBuffer->Unlock();

	hr = lpDevice->CreateIndexBuffer( sizeof(USHORT) * m_uiNumIndices, 0, D3DFMT_INDEX16, 
		D3DPOOL_MANAGED, &m_lpIndexBuffer, NULL );
	if( FAILED(hr) )
	{
		SAFE_DELETEARRAY(pVertex);
		SAFE_DELETEARRAY(pIndices);
		return FALSE;
	}

	LPVOID pIndexData = NULL;
	m_lpIndexBuffer->Lock( 0, sizeof(USHORT) * m_uiNumIndices, &pIndexData, 0 );
	memcpy( pIndexData, pIndices, sizeof(USHORT) * m_uiNumIndices );
	m_lpIndexBuffer->Unlock();

	SAFE_DELETEARRAY(pVertex);
	SAFE_DELETEARRAY(pIndices);

	if( m_pWaterShader )
		return TRUE;
	else
	{
		UINT				count = 0;

		m_pWaterShader = new WaterShader();
		m_pWaterShader->CreateShaders( lpDevice );
		
		m_uiNormalMap = m_pWaterShader->PsConstantTable.GetConstantByName("NormalMap")->RegisterIndex;
		auto con = m_pWaterShader->PsConstantTable.GetConstantByName("SceneMap");
		if(con != nullptr)
			m_uiSceneMap = con->RegisterIndex;

		CreateTextures( lpDevice );
	}
	return TRUE;
}

BOOL Water::CreateTextures( LPDIRECT3DDEVICE9 lpDevice )
{
	auto hr = CreateTextureFromBuffer(g_NormalData, sizeof(g_NormalData), lpDevice, &m_lpNormalMap); 
	if (FAILED(hr))
		MessageBox(NULL, TEXT("Create texture error"), TEXT("Error"), NULL);
	hr = CreateCubeTextureFromBuffer(g_SceneData, sizeof(g_SceneData), lpDevice, &m_lpSceneMap);
	if (FAILED(hr))
		MessageBox(NULL, TEXT("Create cube texture error"), TEXT("Error"), NULL);

	// 纹理创建失败，进行错误处理
	return TRUE;
}

VOID Water::RenderWater( LPDIRECT3DDEVICE9 lpDevice,
	DirectX::XMMATRIX* pWorld, DirectX::XMMATRIX* pView, DirectX::XMMATRIX* pProj )
{
	if( m_pWaterShader )
	{
		WaterShader* pShader = m_pWaterShader;
		DirectX::XMMATRIX matWorldView = *pWorld * *pView; 
		DirectX::XMMATRIX matWorldViewProj = matWorldView * *pProj;
		DirectX::XMMATRIX matViewInv = DirectX::XMMatrixInverse(NULL, *pView);

		auto hWorldViewProj = pShader->VsConstantTable.GetConstantByName("matWorldViewProj")->RegisterIndex;
		lpDevice->SetVertexShaderConstantF(hWorldViewProj, (float*)(&matWorldViewProj), 4);

		auto hViewInverse = pShader->VsConstantTable.GetConstantByName( "matViewInverse" )->RegisterIndex;
		auto wvTranspose = DirectX::XMMatrixTranspose(matViewInv);
		lpDevice->SetVertexShaderConstantF(hViewInverse, (float*)&wvTranspose, 4);

		auto hTime = pShader->VsConstantTable.GetConstantByName("time")->RegisterIndex;

		static float time[4] = { 0 };
		time[0] += 0.1f;
		if( time[0] > 100.0f)
			time[0] -= 100.0f;
		lpDevice->SetVertexShaderConstantF( hTime, time, 1);

		lpDevice->SetVertexShader( pShader->lpVertexShader );
		lpDevice->SetPixelShader( pShader->lpPixelShader );

		if (m_uiNormalMap != -1)
		{
			lpDevice->SetTexture(m_uiNormalMap, m_lpNormalMap);
			lpDevice->SetSamplerState(m_uiNormalMap, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			lpDevice->SetSamplerState(m_uiNormalMap, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			lpDevice->SetSamplerState(m_uiNormalMap, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}

		if (m_uiSceneMap != -1)
		{
			lpDevice->SetTexture( m_uiSceneMap, m_lpSceneMap );
			lpDevice->SetSamplerState(m_uiSceneMap, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			lpDevice->SetSamplerState(m_uiSceneMap, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			lpDevice->SetSamplerState(m_uiSceneMap, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			lpDevice->SetSamplerState(m_uiSceneMap, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			lpDevice->SetSamplerState(m_uiSceneMap, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}
	}

	Render( lpDevice );
}

BOOL Water::WaterShader::CreateShaders( LPDIRECT3DDEVICE9 lpDevice )
{
	static CHAR strVS[] =
	{
		"uniform float4x4 matWorldViewProj;\n"
		"uniform float4x4 matViewInverse;\n"
		"uniform float4x4 matWorldView;\n"
		"uniform float time;\n"
		"\n"
		"struct Wave {\n"
		"  float freq;  // 2*PI / wavelength\n"
		"  float amp;   // amplitude\n"
		"  float phase; // speed * 2*PI / wavelength\n"
		"  float2 dir;\n"
		"};\n"
		"\n"
		"#define NWAVES 2\n"
		"static Wave wave[NWAVES] = {\n"
		"	{ 1.0, 1.0, 0.5, float2(1, 0) },\n"
		"	{ 2.0, 0.5, 1.3, float2(0.7, 0.7) }	\n"
		"};\n"
		"\n"
		"struct VS_INPUT\n"
		"{\n"
		"	float4 position : POSITION;\n"
		"	float3 normal	: NORMAL;\n"
		"	float2 texcoord : TEXCOORD;\n"
		"};\n"
		"\n"
		"struct VS_OUTPUT\n"
		"{\n"
		"	float4 position : POSITION;\n"
		"	float2 texcoord : TEXCOORD;\n"
		"	float3 TexCoord1 : TEXCOORD1; \n"
		"	float3 TexCoord2 : TEXCOORD2; \n"
		"	float3 TexCoord3 : TEXCOORD3; \n"
		"\n"
		"	float2 bumpCoord0 : TEXCOORD4;\n"
		"	float2 bumpCoord1 : TEXCOORD5;\n"
		"	float2 bumpCoord2 : TEXCOORD6;\n"
		"	float3 eyeVector  : TEXCOORD7;\n"
		"};\n"
		"\n"
		"float evaluateWave(Wave w, float2 pos, float t)\n"
		"{\n"
		"  return w.amp * sin( dot(w.dir, pos)*w.freq + t*w.phase);\n"
		"}\n"
		"\n"
		"// derivative of wave function\n"
		"float evaluateWaveDeriv(Wave w, float2 pos, float t)\n"
		"{\n"
		"  return w.freq*w.amp * cos( dot(w.dir, pos)*w.freq + t*w.phase);\n"
		"}\n"
		"\n"
		"// sharp wave functions\n"
		"float evaluateWaveSharp(Wave w, float2 pos, float t, float k)\n"
		"{\n"
		"  return w.amp * pow(sin( dot(w.dir, pos)*w.freq + t*w.phase)* 0.5 + 0.5 , k);\n"
		"}\n"
		"\n"
		"float evaluateWaveDerivSharp(Wave w, float2 pos, float t, float k)\n"
		"{\n"
		"  return k*w.freq*w.amp * pow(sin( dot(w.dir, pos)*w.freq + t*w.phase)* 0.5 + 0.5 , k - 1) * cos( dot(w.dir, pos)*w.freq + t*w.phase);\n"
		"}\n"
		"\n"
		"void main( in  VS_INPUT vert,\n"
		"	   out VS_OUTPUT o )\n"
		"{	\n"
		"	float waveFreq = 0.108;\n"
		"	float waveAmp = 1.0;\n"
		"    	wave[0].freq = waveFreq;\n"
		"   	wave[0].amp = waveAmp;\n"
		"\n"
		"    	wave[1].freq = waveFreq*2.0;\n"
		"    	wave[1].amp = waveAmp*0.5;\n"
		"\n"
		"    	float4 P = vert.position;\n"
		"\n"
		"	// sum waves	\n"
		"	P.y = 0.0;\n"
		"	float ddx = 0.0, ddy = 0.0;\n"
		"	float deriv = 0.0;\n"
		"	\n"
		"	for(int i = 0; i < NWAVES;)\n"
		"	{\n"
		"		P.y += evaluateWave(wave[i], P.xz, time);\n"
		"    		deriv = evaluateWaveDeriv(wave[i], P.xz, time);\n"
		"    		ddx += deriv * wave[i].dir.x;\n"
		"    		ddy += deriv * wave[i].dir.y; \n"
		"		i++;	   		\n"
		"   	}\n"
		"\n"
		"    	float3 B = float3(1, ddx, 0);\n"
		"    	float3 T = float3(0, ddy, 1);\n"
		"    	float3 N = float3(-ddx, 1, -ddy);\n"
		"\n"
		"    	o.position = mul( matWorldViewProj, P );\n"
		"	float2 textureScale = float2(8.0, 4.0);\n"
		"	o.texcoord = vert.texcoord * textureScale;\n"
		"	\n"
		"	float _time = fmod(time, 100.0);\n"
		"	float2 bumpSpeed = float2(-0.05, 0.0);\n"
		"	o.bumpCoord0.xy = vert.texcoord*textureScale + _time*bumpSpeed;\n"
		"	o.bumpCoord1.xy = vert.texcoord*textureScale*2.0 + _time*bumpSpeed*4.0;\n"
		"	o.bumpCoord2.xy = vert.texcoord*textureScale*4.0 + _time*bumpSpeed*8.0;\n"
		"\n"
		"	float3x3 objToTangentSpace;\n"
		"	float BumpScale = 0.2;\n"
		"	// first rows are the tangent and binormal scaled by the bump scale\n"
		"	objToTangentSpace[0] = BumpScale * normalize(T);\n"
		"	objToTangentSpace[1] = BumpScale * normalize(B);\n"
		"	objToTangentSpace[2] = normalize(N);\n"
		"\n"
		"	o.TexCoord1.xyz = mul(objToTangentSpace, float3(1, 0, 0));\n"
		"	o.TexCoord2.xyz = mul(objToTangentSpace, float3(0, 1, 0));\n"
		"	o.TexCoord3.xyz = mul(objToTangentSpace, float3(0, 0, 1));\n"
		"\n"
		"	o.eyeVector = (float3)(matViewInverse[3] - vert.position);\n"
		"}\n"
	};

	static CHAR strPS[] = 	
	{
		"sampler2D	 NormalMap;\n"
		"samplerCUBE SceneMap;\n"
		"\n"
		"struct PS_INPUT\n"
		"{\n"
		"	float4 position : POSITION;\n"
		"	float2 texcoord : TEXCOORD;\n"
		"	float3 TexCoord1 : TEXCOORD1; \n"
		"	float3 TexCoord2 : TEXCOORD2; \n"
		"	float3 TexCoord3 : TEXCOORD3; \n"
		"\n"
		"	float2 bumpCoord0 : TEXCOORD4;\n"
		"	float2 bumpCoord1 : TEXCOORD5;\n"
		"	float2 bumpCoord2 : TEXCOORD6;\n"
		"	float3 eyeVector  : TEXCOORD7;\n"
		"};\n"
		"\n"
		"float4 main( in PS_INPUT vert ) : COLOR\n"
		"{\n"
		"	float4 N = tex2D(NormalMap, vert.texcoord)*2.0 - 1.0;\n"
		"	\n"
		"	float3x3 m; // tangent to world matrix\n"
		"	m[0] = vert.TexCoord1;\n"
		"	m[1] = vert.TexCoord2;\n"
		"	m[2] = vert.TexCoord3;\n"
		"	float3 Nw = mul(m, N.xyz);\n"
		"	\n"
		"//	float3 E = float3(vert.TexCoord1.w, vert.TexCoord2.w, vert.TexCoord3.w);\n"
		"	float3 E = vert.eyeVector;\n"
		"    float3 R = reflect(-E, Nw);\n"
		"	float4 color = texCUBE(SceneMap, R);\n"
		"	//color.a = 1.0;\n"
		"	return color;\n"
		"}\n"
		"//float4 main( in PS_INPUT vert ) : COLOR\n"
		"//{\n"
		"//  float4 finalColor = tex2D( samLinear, vert.texcoord );\n"
		"//  float4 color = texCUBE( samCubeLinear, vert.texRef );\n"
		"//  color = finalColor * 0.1 + color;\n"
		"//  color.a = 1.0;\n"
		"//  return color;\n"
		"//}\n"

	};
	
	static CHAR strPS1[] =
		"texture2D texNormal;\n"
		"sampler2D NormalMap\n"
		"{\n"
		"	Texture = <texNormal>;\n"
		"   MinFilter = linear;\n"
		"	MagFilter = linear;\n"
		"	MipFilter = linear;\n"
		"	AddressU = wrap;\n"
		"	AddressV = wrap;\n"
		"};\n"

		"struct PS_INPUT\n"
		"{\n"
		"	float4 position : POSITION;\n"
		"	float2 texcoord : TEXCOORD;\n"
		"};\n"

		"float4 main(in PS_INPUT vert) : COLOR\n"
		"{\n"
		"	float4 finalColor = tex2D(NormalMap, vert.texcoord);\n"
		"	//float3 tex = float3(-0.5, 0.5 - vert.texcoord.y, vert.texcoord.x - 0.5);\n"
		"	//float4 color = texCUBE(samCubeLinear, tex);\n"
		"	//color = finalColor * 0.1 + color;\n"
		"	//color.a = 1.0;\n"
		"	return finalColor;\n"
		"}\n";

	ID3DBlob* lpShaderBuffer = NULL;
	ID3DBlob* lpErrorBuffer = NULL;

	{
		auto hr = D3DCompile( strVS, lstrlenA( strVS ) + 1, NULL, NULL, NULL, "main", "vs_3_0", 0, 0, 
			&lpShaderBuffer, &lpErrorBuffer);

		if(lpErrorBuffer != NULL)
		{
			MessageBoxA( NULL, (char*)(lpErrorBuffer->GetBufferPointer()), "WaterShader VertexShader", MB_OK );
			lpErrorBuffer->Release();
			lpErrorBuffer = NULL;
			return FALSE;
		}

		VsConstantTable.Create(lpShaderBuffer->GetBufferPointer());
		lpDevice->CreateVertexShader(
			(DWORD*)lpShaderBuffer->GetBufferPointer(), &lpVertexShader );

		if( lpShaderBuffer != NULL)
		{
			lpShaderBuffer->Release();
			lpShaderBuffer = NULL;
		}
	}

	{
		auto hr = D3DCompile(strPS, lstrlenA( strPS ) + 1, NULL, NULL, NULL, "main", "ps_3_0", 0, 0,
			&lpShaderBuffer, &lpErrorBuffer);

		if(lpErrorBuffer != NULL)
		{
			MessageBoxA( NULL, (CHAR*)(lpErrorBuffer->GetBufferPointer()), 
				"WaterShader PixelShader", NULL );
			lpErrorBuffer->Release();
			lpErrorBuffer = NULL;
			return FALSE;
		}

		PsConstantTable.Create(lpShaderBuffer->GetBufferPointer());

		lpDevice->CreatePixelShader(
			(DWORD*)lpShaderBuffer->GetBufferPointer(), &lpPixelShader );

		if( lpShaderBuffer != NULL)
		{		
			lpShaderBuffer->Release();
			lpShaderBuffer = NULL;
		}
	}

	return TRUE;
}