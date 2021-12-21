#include "pch.h"
#include "CScriptManager.h"

#include "CBulletScript.h"
#include "CMonsterHpBarScript.h"
#include "CMonsterScript.h"
#include "CMoveScript.h"
#include "CPlayerScript.h"

void CScriptManager::GetScriptInfo(vector<wstring>& vec)
{
	vec.push_back(L"CBulletScript");
	vec.push_back(L"CMonsterHpBarScript");
	vec.push_back(L"CMonsterScript");
	vec.push_back(L"CMoveScript");
	vec.push_back(L"CPlayerScript");
}

CScript * CScriptManager::GetScript(const wstring& strScriptName)
{
	if (L"CBulletScript" == strScriptName)
		return new CBulletScript;
	if (L"CMonsterHpBarScript" == strScriptName)
		return new CMonsterHpBarScript;
	if (L"CMonsterScript" == strScriptName)
		return new CMonsterScript;
	if (L"CMoveScript" == strScriptName)
		return new CMoveScript;
	if (L"CPlayerScript" == strScriptName)
		return new CPlayerScript;
	return nullptr;
}

CScript * CScriptManager::GetScript(UINT iScriptType)
{
	switch (iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERHPBARSCRIPT:
		return new CMonsterHpBarScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MOVESCRIPT:
		return new CMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptManager::GetScriptName(CScript *pScript)
{
	switch ((SCRIPT_TYPE)pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::MONSTERHPBARSCRIPT:
		return L"CMonsterHpBarScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MOVESCRIPT:
		return L"CMoveScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	}
	return nullptr;
}