#include "pch.h"
#include "func.h"

void SaveWStringToFile(const wstring& str, FILE* file)
{
	size_t len = str.length();
	fwrite(&len, sizeof(size_t), 1, file);
	fwrite(str.c_str(), sizeof(wchar_t), len, file);
}

void LoadWStringFromFile(wstring& str, FILE* file)
{
	size_t len =0;
	fread(&len, sizeof(size_t), 1, file);

	wchar_t szbuffer[255] = L"";
	fread(szbuffer, sizeof(wchar_t), len, file);
	str = szbuffer;
}

wstring ToWString(string var)
{
	array<wchar_t, 256> str = { 0, };
	MultiByteToWideChar(CP_ACP, 0, var.c_str(), (int)var.length(), str.data(), (int)var.length());
	return wstring(str.data());
}
