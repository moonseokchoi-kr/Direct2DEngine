#include "pch.h"
#include "CScriptManager.h"

#include "CBulletScript.h"
#include "CMonsterHpBar.h"
#include "CMonsterScript.h"
#include "CMoveScript.h"
#include "CPlayerScript.h"

void CScriptManager::GetScriptInfo(vector<wstring>& vec)
{
	vec.push_back(L"CBulletScript");
	vec.push_back(L"CMonsterHpBar");
	vec.push_back(L"CMonsterScript");
	vec.push_back(L"CMoveScript");
	vec.push_back(L"CPlayerScript");
}

CScript * CScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CMonsterHpBar" == _strScriptName)
		return new CMonsterHpBar;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMoveScript" == _strScriptName)
		return new CMoveScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	return nullptr;
}

CScript * CScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERHPBAR:
		return new CMonsterHpBar;
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

const wchar_t * CScriptManager::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::MONSTERHPBAR:
		return L"CMonsterHpBar";
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