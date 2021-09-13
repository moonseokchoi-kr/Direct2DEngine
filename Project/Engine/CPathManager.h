#pragma once

class CPathManager
	:public CSingleton<CPathManager>
{
	SINGLE(CPathManager);
private:
	wchar_t		m_szContentPath[255];
	
public:
	void Init();
	const wchar_t* GetContentPath() { return m_szContentPath; }
	wstring GetRelativePath(const wchar_t* _strFullPath);
};

