#pragma once
class DataInputWidget
{
public:
	static bool DataInputInt(wstring name, int* data);
	static bool DataInputFloat(wstring name, float* data);
	static bool DataInputVec2(wstring name, Vec2& data);
	static bool DataInputVec4(wstring name, Vec4& data);
};

