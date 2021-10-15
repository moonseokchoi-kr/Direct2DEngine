#include "pch.h"
#include "CResourceManager.h"

CResourceManager::CResourceManager()
{

}

CResourceManager::~CResourceManager()
{
	for(size_t i =0; i<resrouce_array_.size(); ++i)
	{
		Safe_Delete_Map(resrouce_array_[i]);
	}
}
