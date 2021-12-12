#pragma once
#include "CShader.h"

struct ShaderParameter
{
	SHADER_PARAM type;
	wstring		usage;
};


class CGraphicsShader :
	public CShader
{
public:
	CGraphicsShader();
	~CGraphicsShader();

public:
	

	HRESULT CreateVertexShader(const wstring& strFilePath, const char* functionName);
	HRESULT CreatePixelShader(const wstring& strFilePath, const char* functionName);
	HRESULT CreateGeometryShader(const wstring& strFilePath, const char* functionName);
	

	virtual void UpdateData();

public:
	RENDER_TIMING GetRenderTiming() { return render_timing_; }
	void SetRenderTiming(RENDER_TIMING timing) { render_timing_ = timing; }
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { topology_ = topology; }
	void SetRasterizerType(RASTERIZER_TYPE type) { rasterizer_type_ = type; }
	void SetBlendType(BLEND_TYPE type) { blend_type_ = type; }
	void SetDepthStencilType(DEPTH_STENCIL_TYPE type) { depth_stencil_type_ = type; }
	const vector<ShaderParameter>& GetShaderParam() { return shader_parameter_vector_; }
	void AddShaderParamInfo(const ShaderParameter& _info) { shader_parameter_vector_.push_back(_info); }
private:
	ComPtr<ID3D10Blob> vertex_shader_blob_;
	ComPtr<ID3D10Blob> hull_shader_blob_;
	ComPtr<ID3D10Blob> domain_shader_blob_;
	ComPtr<ID3D10Blob> geometry_shader_blob_;
	ComPtr<ID3D10Blob> pixel_shader_blob_;

	ComPtr<ID3D11VertexShader> vertex_shader_;
	ComPtr<ID3D11HullShader> hull_shader_;
	ComPtr<ID3D11DomainShader> domain_shader_;
	ComPtr<ID3D11GeometryShader> geometry_shader_;
	ComPtr<ID3D11PixelShader> pixel_shader_;

	ComPtr<ID3D11InputLayout> layout_;

	RASTERIZER_TYPE rasterizer_type_;
	BLEND_TYPE blend_type_;
	DEPTH_STENCIL_TYPE depth_stencil_type_;
	RENDER_TIMING render_timing_;

	vector<ShaderParameter> shader_parameter_vector_;

	D3D11_PRIMITIVE_TOPOLOGY topology_;
};

