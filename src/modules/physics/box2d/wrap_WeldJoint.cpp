/**
 * Copyright (c) 2006-2025 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "wrap_WeldJoint.h"

namespace love
{
namespace physics
{
namespace box2d
{

WeldJoint *luax_checkweldjoint(lua_State *L, int idx)
{
	WeldJoint *j = luax_checktype<WeldJoint>(L, idx);
	if (!j->isValid())
		luaL_error(L, "Attempt to use destroyed joint.");
	return j;
}

int w_WeldJoint_setStiffness(lua_State *L)
{
	WeldJoint *t = luax_checkweldjoint(L, 1);
	float arg1 = (float)luaL_checknumber(L, 2);
	t->setStiffness(arg1);
	return 0;
}

int w_WeldJoint_getStiffness(lua_State *L)
{
	WeldJoint *t = luax_checkweldjoint(L, 1);
	lua_pushnumber(L, t->getStiffness());
	return 1;
}

int w_WeldJoint_setDamping(lua_State *L)
{
	WeldJoint *t = luax_checkweldjoint(L, 1);
	float arg1 = (float)luaL_checknumber(L, 2);
	t->setDamping(arg1);
	return 0;
}

int w_WeldJoint_getDamping(lua_State *L)
{
	WeldJoint *t = luax_checkweldjoint(L, 1);
	lua_pushnumber(L, t->getDamping());
	return 1;
}

int w_WeldJoint_getReferenceAngle(lua_State *L)
{
	WeldJoint *t = luax_checkweldjoint(L, 1);
	lua_pushnumber(L, t->getReferenceAngle());
	return 1;
}

static const luaL_Reg w_WeldJoint_functions[] =
{
	{ "setStiffness", w_WeldJoint_setStiffness },
	{ "getStiffness", w_WeldJoint_getStiffness },
	{ "setDamping", w_WeldJoint_setDamping },
	{ "getDamping", w_WeldJoint_getDamping },
	{ "getReferenceAngle", w_WeldJoint_getReferenceAngle },
	{ 0, 0 }
};

extern "C" int luaopen_weldjoint(lua_State *L)
{
	return luax_register_type(L, &WeldJoint::type, w_Joint_functions, w_WeldJoint_functions, nullptr);
}

} // box2d
} // physics
} // love
