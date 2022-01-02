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
	static bool DataInputText(wstring name, wstring& str);


	static bool DataDragInputInt(wstring name, int* data, bool is_normal = false);
	static bool DataDragInputFloat(wstring name, float* data, bool is_normal= false);
	static bool DataDragInputVec2(wstring name, Vec2& data, bool is_normal = false);
	static bool DataDragInputVec3(wstring name, Vec3& data, bool is_normal = false);
	static bool DataDragInputVec4(wstring name, Vec4& data, bool is_normal = false);


	static void DataInputObejct(wstring name, wstring object);
};

