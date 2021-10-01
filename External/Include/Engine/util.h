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
#define SafeDelete(x) {delete x; x=0;}
