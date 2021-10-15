#include "pch.h"


#include "CMaterial.h"
#include "CTexture.h"

#include "CConstBuffer.h"
#include "CDevice.h"



CMaterial::CMaterial()
	:data_{}
	,texture_array_{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetData(SHADER_PARAM param, void* data)
{
	switch (param)
	{
	case SHADER_PARAM::INT_0: 
	case SHADER_PARAM::INT_1: 
	case SHADER_PARAM::INT_2: 
	case SHADER_PARAM::INT_3:
		data_.int_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::INT_0)] = *static_cast<int*>(data);
		break;
	case SHADER_PARAM::FLOAT_0: 
	case SHADER_PARAM::FLOAT_1: 
	case SHADER_PARAM::FLOAT_2: 
	case SHADER_PARAM::FLOAT_3:
		data_.float_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::FLOAT_0)] = *static_cast<float*>(data);
		break;
	case SHADER_PARAM::VEC2_0: 
	case SHADER_PARAM::VEC2_1: 
	case SHADER_PARAM::VEC2_2: 
	case SHADER_PARAM::VEC2_3:
		data_.vec2_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::VEC2_0)] = *static_cast<Vec2*>(data);
		break;
	case SHADER_PARAM::VEC4_0: 
	case SHADER_PARAM::VEC4_1: 
	case SHADER_PARAM::VEC4_2: 
	case SHADER_PARAM::VEC4_3:
		data_.vec4_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::VEC4_0)] = *static_cast<Vec4*>(data);
		break;
	case SHADER_PARAM::MAT_0: 
	case SHADER_PARAM::MAT_1: 
	case SHADER_PARAM::MAT_2: 
	case SHADER_PARAM::MAT_3:
		data_.matrix_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::MAT_0)] = *static_cast<Matrix*>(data);
		break;
	case SHADER_PARAM::TEX_0: 
	case SHADER_PARAM::TEX_1: 
	case SHADER_PARAM::TEX_2: 
	case SHADER_PARAM::TEX_3: 
	case SHADER_PARAM::TEX_ARR_0: 
	case SHADER_PARAM::TEX_ARR_1: 
	case SHADER_PARAM::TEX_CUBE_0: 
	case SHADER_PARAM::TEX_CUBE_1: 
	case SHADER_PARAM::TEX_END:
		texture_array_[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::TEX_0)] = static_cast<CTexture*>(data);
	break;
	default: ;
	}
}

void CMaterial::UpdateData()
{
	shader_->UpdateData();

	static CConstBuffer* const_buffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	const_buffer->SetData(&data_, sizeof(MaterialParameter));
	const_buffer->SetPipelineStage(PIPELINE_STAGE::PS_ALL);
	const_buffer->UpdateData();

	for(size_t i =0; i<texture_array_.size(); ++i)
	{
		if (nullptr != texture_array_[i])
		{
			texture_array_[i]->SetPipelineStage(PIPELINE_STAGE::PS_ALL, i);
			texture_array_[i]->UpdateData();
		}
	}
}
