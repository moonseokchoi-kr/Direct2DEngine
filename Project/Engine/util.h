#pragma once


#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#endif

///d3d error checker
/// 
#if defined(_DEBUG)
	#ifndef HR
	#define HR(x)											\
	{														\
		HRESULT hr = (x);									\
		if (FAILED(hr))										\
		{													\
			DXTrace(__FILE__,(DWORD)__LINE__,hr, L#x, true);\
			return E_FAIL;									\
		}													\
	}													
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif	
#endif

#define ReleaseCOM(x) {if(x){x->Release(); x=0;}}
#define SafeDelete(x) {if(x){delete x; x=0;}}


class GrapicsAdapterUtil
{
public:

	static ComPtr<IDXGIAdapter>EnumratesAdapters(const wchar_t* _name);
	static DXGI_MODE_DESC* EnumratesOutputs(ComPtr<IDXGIAdapter> _adapter);

private:
	static ComPtr<IDXGIAdapter>SelectAdapters(vector<ComPtr<IDXGIAdapter>> _adapters , const wchar_t* _name);
};
