#pragma once

#define SINGLE(type) private:\
						type();\
						~type();\
						friend class CSingleton<type>;