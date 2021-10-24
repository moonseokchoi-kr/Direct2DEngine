// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

//Window Lib
#include <windows.h>
#include <stdlib.h>

//std and stl
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <array>
#include <cmath>
#include <assert.h>
#include <crtdbg.h>

using std::wstring;
using std::vector;
using std::list;
using std::unordered_map;
using std::make_pair;
using std::array;
using std::to_wstring;

#include "singleton.h"

//DirectX 11 Header

#include <wrl/client.h>
#include <d3d11.h>
#include <FX11/d3dx11effect.h>
#include <dxgi.h>
#include <dxerr/DxErr.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;


//DirectX11 Library
#ifdef _DEBUG
#pragma comment(lib, "FX11/Effects11_debug")
#else
#pragma comment(lib, "FX11/Effects11")
#endif
#pragma comment(lib, "d3d11")
#pragma comment(lib,"d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "dxerr/DxErr")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "legacy_stdio_definitions.lib")
//custom header

#include "SimpleMath.h"
using namespace SimpleMath;
#include "define.h"
#include "common.h"
#include "struct.h"
#include "func.h"
#include "util.h"
#include "extern.h"

#endif; //PCH_H
