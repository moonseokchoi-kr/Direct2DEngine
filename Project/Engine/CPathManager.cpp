#include "pch.h"
#include "CPathManager.h"


#include "CCore.h"

CPathManager::CPathManager()
	: m_szContentPath{}
{}

CPathManager::~CPathManager()
{
}


void CPathManager::Init()
{
	GetCurrentDirectory(255, m_szContentPath);

	int iLen = (int)wcslen(m_szContentPath);

	// 상위폴더로 
	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");
	

	//SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);
}

wstring CPathManager::GetRelativePath(const wchar_t* _strFullPath)
{
	//_strFullPath.substr();
	wstring strPath = _strFullPath;

	size_t absLen = wcslen(m_szContentPath);
	size_t fullLen = strPath.length();

	wstring strRelativePath = strPath.substr(absLen, fullLen - absLen);
	return strRelativePath;
}
