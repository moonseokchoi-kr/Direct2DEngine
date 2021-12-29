#pragma once

#include <Engine/CTexture.h>
#include "ComboWidget.h"

class DataInputWidget
{
public:
	static bool DataInputInt(wstring name, int* data);
	static bool DataInputFloat(wstring name, float* data);
	static bool DataInputVec2(wstring name, Vec2& data);
	static bool DataInputVec3(wstring name, Vec3& data);
	static bool DataInputVec4(wstring name, Vec4& data);
	static void DataInputTexture(wstring name, Ptr<CTexture> texture, DWORD_PTR second, COMBO_CALLBACK callback, Widget* inst);
};

