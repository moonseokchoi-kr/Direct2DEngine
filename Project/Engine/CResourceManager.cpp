#include "pch.h"
#include "CResourceManager.h"

CResourceManager::CResourceManager()
{

}

CResourceManager::~CResourceManager()
{
	for(size_t i =0; i<resources_.size(); ++i)
	{
		Safe_Delete_Map(resources_[i]);
	}
}
