#include "pch.h"
#include "extern.h"

D3D11_INPUT_ELEMENT_DESC g_layout[3] =
{
	D3D11_INPUT_ELEMENT_DESC {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA, 0},

};

Transform g_transform = {};