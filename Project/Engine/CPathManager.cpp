#include "pch.h"
#include "CPathManager.h"


#include "CCore.h"

CPathManager::CPathManager()
	: content_path_{}
{}

CPathManager::~CPathManager()
{
}


void CPathManager::Init()
{
	GetCurrentDirectory(255, content_path_);

	int iLen = (int)wcslen(content_path_);

	// 상위폴더로 
	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == content_path_[i])
		{
			content_path_[i] = '\0';
			break;
		}
	}

	wcscat_s(content_path_, 255, L"\\bin\\content\\");
	

	//SetWindowText(CCore::GetInst()->GetMainHwnd(), content_path_);
}

wstring CPathManager::GetRelativePath(const wchar_t* strFullPath)
{
	//strFullPath.substr();
	wstring strPath = strFullPath;

	size_t absLen = wcslen(content_path_);
	size_t fullLen = strPath.length();

	wstring strRelativePath = strPath.substr(absLen, fullLen - absLen);
	return strRelativePath;
}
