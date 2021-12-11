#include "pch.h"
#include "imgui.h"
#include "client_func.h"


Vec2::operator ImVec2() const 
{
	return ImVec2(x, y);
}

string WStringToString(const wstring& destBuffer)
{
	string str;
	WideCharToMultiByte(CP_ACP, 0, destBuffer.c_str(), -1, str.data(), (int)destBuffer.size(), nullptr, nullptr);

	return str;
}

wstring StringToWString(const string& destBuffer)
{
	wstring w_str;
	MultiByteToWideChar(CP_ACP, 0, destBuffer.c_str(), (int)destBuffer.size(), w_str.data(), (int)w_str.size());
	return w_str;
}
