#include "pch.h"
#include "imgui.h"


Vec2::operator ImVec2() const 
{
	return ImVec2(x, y);
}
