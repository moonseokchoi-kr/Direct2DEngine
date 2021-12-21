#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	BULLETSCRIPT,
	MONSTERHPBARSCRIPT,
	MONSTERSCRIPT,
	MOVESCRIPT,
	PLAYERSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptManager
{
public:
	static void GetScriptInfo(vector<wstring>& vec);
	static CScript * GetScript(const wstring& strScriptName);
	static CScript * GetScript(UINT iScriptType);
	static const wchar_t * GetScriptName(CScript * pScript);
};
