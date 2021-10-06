#pragma once

class CPathManager
	:public CSingleton<CPathManager>
{
	SINGLE(CPathManager);
private:
	wchar_t		content_path_[255];
	
public:
	void Init();
	const wchar_t* GetContentPath() { return content_path_; }
	wstring GetRelativePath(const wchar_t* strFullPath);
};

