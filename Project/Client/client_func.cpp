#include "pch.h"
#include "imgui.h"
#include "client_func.h"


Vec2::operator ImVec2() const 
{
	return ImVec2(x, y);
}

string WStringToString(const wstring& destBuffer)
{
	array<char, 256> str = { 0, };
	WideCharToMultiByte(CP_ACP, 0, destBuffer.c_str(), (int)destBuffer.size(), str.data(), (int)str.size(), nullptr, nullptr);

	return string(str.data());
}

wstring StringToWString(const string& destBuffer)
{
	array<wchar_t, 256> str = { 0, };
	MultiByteToWideChar(CP_ACP, 0, destBuffer.c_str(), (int)destBuffer.size(), str.data(), (int)destBuffer.size());
	return wstring(str.data());
}
