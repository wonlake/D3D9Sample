#include <windows.h>
#include <Wincodec.h>
#include <wrl.h> 
#include "Utility.h"
#include <array>

#pragma comment(lib, "Windowscodecs.lib" )

HRESULT GetPixelDataFromFile(LPCWCHAR szFileName, 
    BYTE** ppData, UINT& dataSize, UINT& width, UINT& height)
{
    Microsoft::WRL::ComPtr<IWICImagingFactory> piFactory; 
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> piDecoder;
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> piBitmapFrameIn;
	Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        (LPVOID*)&piFactory);

    if (FAILED(hr))
        return hr;

	hr = piFactory->CreateDecoderFromFilename(szFileName, NULL,
		GENERIC_READ, WICDecodeMetadataCacheOnLoad, &piDecoder);

    if (FAILED(hr))
        return hr;

	hr = piDecoder->GetFrame(0, &piBitmapFrameIn);
    
    auto refCount = piBitmapFrameIn->AddRef();
    refCount = piBitmapFrameIn->Release(); 

    if (FAILED(hr))
        return hr;

	hr = piBitmapFrameIn->GetSize(&width, &height);
    if (FAILED(hr))
        return hr;
	
    WICPixelFormatGUID format;
	hr = piBitmapFrameIn->GetPixelFormat(&format);

    if (FAILED(hr))
        return hr;
	
    if (!IsEqualGUID(format, GUID_WICPixelFormat32bppBGRA))
    {
        // 创建Format Converter
        hr = piFactory->CreateFormatConverter(&pConverter);
        if (FAILED(hr))
            return hr;

        // 初始化Format Converter
        hr = pConverter->Initialize(piBitmapFrameIn.Get(),
            GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
        if (FAILED(hr))
            return hr;
    }

    // 获取像素数据
    UINT stride = width * 4;
    dataSize = stride * height;
    BYTE* pixels = new BYTE[dataSize];

    if (pConverter == nullptr)
        hr = piBitmapFrameIn->CopyPixels(nullptr, stride, dataSize, pixels);
    else
        hr = pConverter->CopyPixels(nullptr, stride, dataSize, pixels);
	*ppData = pixels;

    CoUninitialize();

    return hr;
}

HRESULT GetPixelDataFromBuffer(BYTE rawData[], UINT rawDataSize,
    BYTE** ppData, UINT& dataSize, UINT& width, UINT& height)
{
    Microsoft::WRL::ComPtr<IWICImagingFactory> piFactory;
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> piDecoder;
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> piBitmapFrameIn;
    Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        (LPVOID*)&piFactory);

    if (FAILED(hr))
        return hr;

    Microsoft::WRL::ComPtr<IWICStream> pStreamIn;
    hr = piFactory->CreateStream(&pStreamIn);
    if (FAILED(hr))
        return hr;

    hr = pStreamIn->InitializeFromMemory(rawData, rawDataSize);
    if (FAILED(hr))
        return hr;

    hr = piFactory->CreateDecoderFromStream(pStreamIn.Get(), NULL,
        WICDecodeMetadataCacheOnLoad, &piDecoder);

    if (FAILED(hr))
        return hr;

    hr = piDecoder->GetFrame(0, &piBitmapFrameIn);

    if (FAILED(hr))
        return hr;

    hr = piBitmapFrameIn->GetSize(&width, &height);
    if (FAILED(hr))
        return hr;

    WICPixelFormatGUID format;
    hr = piBitmapFrameIn->GetPixelFormat(&format);

    if (FAILED(hr))
        return hr;

    if (!IsEqualGUID(format, GUID_WICPixelFormat32bppBGRA))
    {
        // 创建Format Converter
        hr = piFactory->CreateFormatConverter(&pConverter);
        if (FAILED(hr))
            return hr;

        // 初始化Format Converter
        hr = pConverter->Initialize(piBitmapFrameIn.Get(),
            GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
        if (FAILED(hr))
            return hr;
    }

    // 获取像素数据
    UINT stride = width * 4;
    dataSize = stride * height;
    BYTE* pixels = new BYTE[dataSize];

    if (pConverter == nullptr)
        hr = piBitmapFrameIn->CopyPixels(nullptr, stride, dataSize, pixels);
    else
        hr = pConverter->CopyPixels(nullptr, stride, dataSize, pixels);
    *ppData = pixels;

    CoUninitialize();

    return hr;
}

HRESULT CreateTextureFromFile(LPCWCHAR szFileName, LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DTEXTURE9* ppTexture)
{ 
    BYTE* pData = nullptr;
    UINT dataSize = 0;
    UINT width = 0;
    UINT height = 0;

    auto hr = GetPixelDataFromFile(szFileName, &pData, dataSize, width, height);
    if (FAILED(hr))
        return hr;

	hr = lpDevice->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, ppTexture, NULL);
    if (FAILED(hr))
    {
        delete[] pData;
        return hr;
    }

    D3DLOCKED_RECT lockedRect;
    hr = (*ppTexture)->LockRect(0, &lockedRect, NULL, 0);
    if (FAILED(hr))
    {
        delete[] pData;
        (*ppTexture)->Release();
        return hr;
    }
    BYTE* pDest = (BYTE*)lockedRect.pBits;
    memcpy(pDest, pData, dataSize); // Assuming 32bpp format
    hr = (*ppTexture)->UnlockRect(0);
    if (FAILED(hr))
    {
        delete[] pData;
        (*ppTexture)->Release();
        return hr; 
    }

    delete[] pData;
    return hr;
}

HRESULT CreateCubeTextureFromFile(LPCWCHAR szFileName[6], LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DCUBETEXTURE9* ppTexture)
{
    UINT dataSize = 0;
    UINT width = 0;
    UINT height = 0;

    std::array<BYTE*, 6> cubeDatas = { NULL, NULL, NULL, NULL, NULL, NULL };
    auto i = 0;
    HRESULT hr = S_OK;
    for (auto& item : cubeDatas)
    {
		BYTE* pData = nullptr;
        hr = GetPixelDataFromFile(szFileName[i++], &pData, dataSize, width, height);
        if (FAILED(hr))
        {
            for (auto& item2 : cubeDatas)
                delete[] item2;
            return hr;
        }
        item = pData;
    }

    hr = lpDevice->CreateCubeTexture(width, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, ppTexture, NULL);

    D3DLOCKED_RECT lockedRect;
    for (int i = 0; i < cubeDatas.size(); ++i)
    {
        hr = (*ppTexture)->LockRect(D3DCUBEMAP_FACES(i), 0, &lockedRect, NULL, 0);
        if (FAILED(hr))
        {
            for (auto& item2 : cubeDatas)
                delete[] item2;
            (*ppTexture)->Release();
            return hr;
        }
        BYTE* pDest = (BYTE*)lockedRect.pBits;
        memcpy(pDest, cubeDatas[i], dataSize);
        hr = (*ppTexture)->UnlockRect(D3DCUBEMAP_FACES(i), 0);
        if (FAILED(hr))
        {
            for (auto& item2 : cubeDatas)
                delete[] item2;
            (*ppTexture)->Release();
            return hr;
        }
    }

    for (auto& item : cubeDatas)
    {
        delete[] item;
    }

    return hr;
}

HRESULT CreateTextureFromBuffer(BYTE rawData[], UINT rawDataSize, LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DTEXTURE9* ppTexture)
{
    BYTE* pData = nullptr;
    UINT dataSize = 0;
    UINT width = 0;
    UINT height = 0;

    auto hr = GetPixelDataFromBuffer(rawData, rawDataSize, &pData, dataSize, width, height);
    if (FAILED(hr))
        return hr;

    hr = lpDevice->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, ppTexture, NULL);
    if (FAILED(hr))
    {
        delete[] pData;
        return hr;
    }

    D3DLOCKED_RECT lockedRect;
    hr = (*ppTexture)->LockRect(0, &lockedRect, NULL, 0);
    if (FAILED(hr))
    {
        delete[] pData;
        (*ppTexture)->Release();
        return hr;
    }
    BYTE* pDest = (BYTE*)lockedRect.pBits;
    memcpy(pDest, pData, dataSize); // Assuming 32bpp format
    hr = (*ppTexture)->UnlockRect(0);
    if (FAILED(hr))
    {
        delete[] pData;
        (*ppTexture)->Release();
        return hr;
    }

    delete[] pData;
    return hr;
}

HRESULT CreateCubeTextureFromBuffer(BYTE rawData[], UINT rawDataSize, LPDIRECT3DDEVICE9 lpDevice, LPDIRECT3DCUBETEXTURE9* ppTexture)
{
    UINT dataSize = 0;
    UINT width = 0;
    UINT height = 0;

    HRESULT hr = S_OK;
        
    BYTE* pData = nullptr;
	hr = GetPixelDataFromBuffer(rawData, rawDataSize, &pData, dataSize, width, height);
	if (FAILED(hr))
		return hr;

    hr = lpDevice->CreateCubeTexture(height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, ppTexture, NULL);
    if (FAILED(hr))
        return hr;

    D3DLOCKED_RECT lockedRect;

    UINT cubeWidth = width / 6;

    for (int i = 0; i < 6; ++i)
    {
        hr = (*ppTexture)->LockRect(D3DCUBEMAP_FACES(i), 0, &lockedRect, NULL, 0);
        if (FAILED(hr))
        {
            delete[] pData;
            (*ppTexture)->Release();
            return hr;
        }
        BYTE* pDest = (BYTE*)lockedRect.pBits;
        for (UINT h = 0; h < height; ++h)
        {
            auto srcOffset = h * width + i * cubeWidth;
            auto dstOffset = h * cubeWidth;
            memcpy(pDest + dstOffset * 4, pData + srcOffset * 4, cubeWidth * 4);
        }
        hr = (*ppTexture)->UnlockRect(D3DCUBEMAP_FACES(i), 0);
        if (FAILED(hr))
        {
            delete[] pData;
            (*ppTexture)->Release();
            return hr;
        }
    }

    delete[] pData;
    return hr;
}

// Structs
struct CTHeader
{
	uint32_t Size;
	uint32_t Creator;
	uint32_t Version;
	uint32_t Constants;
	uint32_t ConstantInfo;
	uint32_t Flags;
	uint32_t Target;
};

struct CTInfo
{
	uint32_t Name;
	uint16_t RegisterSet;
	uint16_t RegisterIndex;
	uint16_t RegisterCount;
	uint16_t Reserved;
	uint32_t TypeInfo;
	uint32_t DefaultValue;
};

struct CTType
{
	uint16_t Class;
	uint16_t Type;
	uint16_t Rows;
	uint16_t Columns;
	uint16_t Elements;
	uint16_t StructMembers;
	uint32_t StructMemberInfo;
};

// Shader instruction opcodes
const uint32_t SIO_COMMENT = 0x0000FFFE;
const uint32_t SIO_END = 0x0000FFFF;
const uint32_t SI_OPCODE_MASK = 0x0000FFFF;
const uint32_t SI_COMMENTSIZE_MASK = 0x7FFF0000;
const uint32_t CTAB_CONSTANT = 0x42415443;

// Member functions
bool ConstantTable::Create(const void* data)
{
	const uint32_t* ptr = static_cast<const uint32_t*>(data);
	while (*++ptr != SIO_END)
	{
		if ((*ptr & SI_OPCODE_MASK) == SIO_COMMENT)
		{
			// Check for CTAB comment
			uint32_t comment_size = (*ptr & SI_COMMENTSIZE_MASK) >> 16;
			if (*(ptr + 1) != CTAB_CONSTANT)
			{
				ptr += comment_size;
				continue;
			}

			// Read header
			const char* ctab = reinterpret_cast<const char*>(ptr + 2);
			size_t ctab_size = (comment_size - 1) * 4;

			const CTHeader* header = reinterpret_cast<const CTHeader*>(ctab);
			if (ctab_size < sizeof(*header) || header->Size != sizeof(*header))
				return false;
			m_creator = ctab + header->Creator;

			// Read constants
			m_constants.reserve(header->Constants);
			const CTInfo* info = reinterpret_cast<const CTInfo*>(ctab + header->ConstantInfo);
			for (uint32_t i = 0; i < header->Constants; ++i)
			{
				const CTType* type = reinterpret_cast<const CTType*>(ctab + info[i].TypeInfo);

				// Fill struct
				ConstantDesc desc;
				desc.Name = ctab + info[i].Name;
				desc.RegisterSet = static_cast<EREGISTER_SET>(info[i].RegisterSet);
				desc.RegisterIndex = info[i].RegisterIndex;
				desc.RegisterCount = info[i].RegisterCount;
				desc.Rows = type->Rows;
				desc.Columns = type->Columns;
				desc.Elements = type->Elements;
				desc.StructMembers = type->StructMembers;
				desc.Bytes = 4 * desc.Elements * desc.Rows * desc.Columns;
				m_constants.push_back(desc);
			}
			return true;
		}
	}
	return false;
}

const ConstantDesc* ConstantTable::GetConstantByName(const std::string& name) const
{
	std::vector<ConstantDesc>::const_iterator it;
	for (it = m_constants.begin(); it != m_constants.end(); ++it)
	{
		if (it->Name == name)
			return &(*it);
	}
	return NULL;
}

//bool IsRGBA(const std::wstring& strFileName)
//{
//    unsigned char buf[4] = { 0 };
//    std::fstream fs(strFileName, std::ios::in | std::ios::binary);
//    fs.seekg(4, SEEK_SET);
//    fs.read((char*)&buf, 4);
//    int pos = (buf[3] << 24) + (buf[2] << 16) + (buf[1] << 8) + buf[0];
//    while (pos > 0)
//    {
//        fs.seekg(pos, SEEK_SET);
//        fs.read((char*)&buf, 2);
//        int numEntry = (buf[1] << 8) + buf[0];
//        fs.seekg(12 * numEntry, SEEK_CUR);
//        fs.read((char*)&buf, 4);
//        pos = (buf[3] << 24) + (buf[2] << 16) + (buf[1] << 8) + buf[0];
//    }
//    fs.seekg(15, SEEK_CUR);
//    fs.read((char*)&buf, 1);
//    if (buf[0] == 0x0F)
//        return true;
//    else
//        return false;
//}