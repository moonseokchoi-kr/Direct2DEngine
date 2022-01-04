#include "pch.h"

#include "CMaterial.h"

#include "CConstBuffer.h"
#include "CDevice.h"

#include "CSceneManager.h"


CMaterial::CMaterial()
	:CResource(RESOURCE_TYPE::MATERIAL)
	,data_{}
	,texture_array_{}
	,is_default(false)
{
}

CMaterial::CMaterial(bool _defualt)
	:CResource(RESOURCE_TYPE::MATERIAL)
	,data_{}
	,texture_array_{}
	,is_default(_defualt)
{
}

CMaterial::CMaterial(const CMaterial& origin)
	:CResource(origin)
	,data_(origin.data_)
	,texture_array_(origin.texture_array_)
	,is_default(true)
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
		texture_array_[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::TEX_0)] = static_cast<CTexture*>(data);
		break;
	default: 
		break;
	}
	if (!is_default && SCENE_MODE::STOP == CSceneManager::GetInst()->GetSceneMode())
	{
		Save(GetRelativePath());
	}
}

void CMaterial::GetData(SHADER_PARAM param, void* data)
{
	switch (param)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		*((int*)data)=data_.int_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::INT_0)];
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		*((float*)data) = data_.float_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::FLOAT_0)];
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		*((Vec2*)data) = data_.vec2_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::VEC2_0)];
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		*((Vec4*)data) = data_.vec4_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::VEC4_0)];
		break;
	case SHADER_PARAM::MAT_0:
	case SHADER_PARAM::MAT_1:
	case SHADER_PARAM::MAT_2:
	case SHADER_PARAM::MAT_3:
		*((Matrix*)data) = data_.matrix_arr[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::MAT_0)];
		break;
	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
		*((Ptr<CTexture>*)data) = texture_array_[static_cast<UINT>(param) - static_cast<UINT>(SHADER_PARAM::TEX_0)];
		break;
	case SHADER_PARAM::TEX_ARR_0:
	case SHADER_PARAM::TEX_ARR_1:
	case SHADER_PARAM::TEX_CUBE_0:
	case SHADER_PARAM::TEX_CUBE_1:

	default:
		break;
	}
}

void CMaterial::UpdateData()
{
	shader_->UpdateData();

	static CConstBuffer* const_buffer = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::MATERIAL_CONST);
	const_buffer->SetData(&data_, sizeof(MaterialParameter));
	const_buffer->SetPipelineStage(PIPELINE_STAGE::PS_ALL);
	const_buffer->UpdateData();

	for(UINT i =0; i<(UINT)texture_array_.size(); ++i)
	{

		if (nullptr != texture_array_[i])
		{
			texture_array_[i]->SetPipelineStage(PIPELINE_STAGE::PS_ALL, i);
			texture_array_[i]->UpdateData();
		}
	}
}

HRESULT CMaterial::Save(const wstring& relativePath)
{

	wstring contentPath = CPathManager::GetInst()->GetContentPath();
	contentPath += relativePath;

	SetRelativePath(relativePath);

	FILE* file = nullptr;
	_wfopen_s(&file, contentPath.c_str(), L"wb");

	if (nullptr == file)
		return E_FAIL;

	// Key, 경로
	SaveWStringToFile(GetKey(), file);
	SaveWStringToFile(GetRelativePath(), file);

	// 상수 데이터
	fwrite(&data_, sizeof(MaterialParameter), 1, file);

	// 텍스쳐 데이터
	for (int i = 0; i < texture_array_.size(); ++i)
	{
		SaveResReference(texture_array_[i], file);
	}

	// 참조 쉐이더
	SaveResReference(shader_, file);

	// Default
	fwrite(&is_default, sizeof(bool), 1, file);

	fclose(file);

	return S_OK;
}

HRESULT CMaterial::Load(const wstring& filePath)
{

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");

	if (nullptr == file)
		return E_FAIL;

	// Key, 경로
	wstring key, relativePath;
	LoadWStringFromFile(key, file);
	LoadWStringFromFile(relativePath, file);

	// 상수 데이터
	fread(&data_, sizeof(MaterialParameter), 1, file);

	// 텍스쳐 데이터
	for (int i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		LoadResReference(texture_array_[i], file);
	}

	// 참조 쉐이더
	LoadResReference(shader_, file);

	// Default
	fread(&is_default, sizeof(bool), 1, file);

	fclose(file);

	return S_OK;
}
