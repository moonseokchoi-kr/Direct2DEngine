#include "pch.h"
#include "CResource.h"

CResource::CResource(RESOURCE_TYPE type)
	:
	resource_type_(type)
	,reference_count_(0)
{
}

CResource::~CResource()
{
}
