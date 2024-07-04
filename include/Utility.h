//
// Copyright (C) Mei Jun 2011
//

#pragma once

#define SAFE_RELEASE(p) if(p != NULL) { (p)->Release(); (p) = NULL; };
#define SAFE_DELETE(p) if(p != NULL) { delete (p); (p) = NULL; };
#define SAFE_DELETEARRAY(p) if(p != NULL) { delete[] (p); (p) = NULL; };

#include <vector>
#include <string>
#include <cstdint>
#include <array>
#include <d3d9.h>

enum EREGISTER_SET
{
    RS_BOOL,
    RS_INT4,
    RS_FLOAT4,
    RS_SAMPLER
};

struct ConstantDesc
{
    std::string Name;
    EREGISTER_SET RegisterSet;
    int RegisterIndex;
    int RegisterCount;
    int Rows;
    int Columns;
    int Elements;
    int StructMembers;
    size_t Bytes;
};

class ConstantTable
{
public:
    bool Create(const void* data);

    size_t GetConstantCount() const { return m_constants.size(); }
    const std::string& GetCreator() const { return m_creator; }

    const ConstantDesc* GetConstantByIndex(size_t i) const { return &m_constants[i]; }
    const ConstantDesc* GetConstantByName(const std::string& name) const;

private:
    std::vector<ConstantDesc> m_constants;
    std::string m_creator;
};

HRESULT CreateTextureFromFile(LPCWCHAR szFileName, LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT CreateCubeTextureFromFile(LPCWCHAR szFileName[6], LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DCUBETEXTURE9* ppTexture);

HRESULT CreateTextureFromBuffer(BYTE data[], UINT dataSize, LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DTEXTURE9* ppTexture);
HRESULT CreateCubeTextureFromBuffer(BYTE rawData[], UINT rawDataSize, LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DCUBETEXTURE9* ppTexture);
