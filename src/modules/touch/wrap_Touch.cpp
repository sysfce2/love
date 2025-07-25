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

#include "common/config.h"

// LOVE
#include "wrap_Touch.h"

#include "sdl/Touch.h"
#include "common/Optional.h"

namespace love
{
namespace touch
{

#define instance() (Module::getInstance<Touch>(Module::M_TOUCH))

int64 luax_checktouchid(lua_State *L, int idx)
{
	if (!lua_islightuserdata(L, idx))
		return luax_typerror(L, idx, "touch id");

	return (int64) (intptr_t) lua_touserdata(L, 1);
}

int w_getTouches(lua_State *L)
{
	Optional<Touch::DeviceType> typefilter;
	if (!lua_isnoneornil(L, 1))
	{
		const char *typestr = luaL_checkstring(L, 1);
		if (!Touch::getConstant(typestr, typefilter.value))
			return luax_enumerror(L, "touch device type", Touch::getConstants(typefilter.value), typestr);
		typefilter.hasValue = true;
	}

	const std::vector<Touch::TouchInfo> &touches = instance()->getTouches();

	lua_createtable(L, (int) touches.size(), 0);

	int filteredindex = 1;
	for (const Touch::TouchInfo &touch : touches)
	{
		if (!typefilter.hasValue || typefilter.value == touch.deviceType)
		{
			// This is a bit hackish and we lose the higher 32 bits of the id on
			// 32-bit systems, but SDL only ever gives id's that at most use as many
			// bits as can fit in a pointer (for now.)
			// We use lightuserdata instead of a lua_Number (double) because doubles
			// can't represent all possible id values on 64-bit systems.
			lua_pushlightuserdata(L, (void *)(intptr_t)touch.id);
			lua_rawseti(L, -2, filteredindex);
			filteredindex++;
		}
	}

	return 1;
}

int w_getPosition(lua_State *L)
{
	int64 id = luax_checktouchid(L, 1);

	Touch::TouchInfo touch = {};
	luax_catchexcept(L, [&]() { touch = instance()->getTouch(id); });

	lua_pushnumber(L, touch.x);
	lua_pushnumber(L, touch.y);

	return 2;
}

int w_getPressure(lua_State *L)
{
	int64 id = luax_checktouchid(L, 1);

	Touch::TouchInfo touch = {};
	luax_catchexcept(L, [&](){ touch = instance()->getTouch(id); });

	lua_pushnumber(L, touch.pressure);
	return 1;
}

int w_getDeviceType(lua_State *L)
{
	int64 id = luax_checktouchid(L, 1);

	Touch::TouchInfo touch = {};
	luax_catchexcept(L, [&]() { touch = instance()->getTouch(id); });

	const char *typestr = nullptr;
	if (!Touch::getConstant(touch.deviceType, typestr))
		return luaL_error(L, "Unknown touch device type.");

	lua_pushstring(L, typestr);
	return 1;
}

int w_isMouse(lua_State *L)
{
	int64 id = luax_checktouchid(L, 1);

	Touch::TouchInfo touch = {};
	luax_catchexcept(L, [&]() { touch = instance()->getTouch(id); });

	luax_pushboolean(L, touch.mouse);
	return 1;
}

static const luaL_Reg functions[] =
{
	{ "getTouches", w_getTouches },
	{ "getPosition", w_getPosition },
	{ "getPressure", w_getPressure },
	{ "getDeviceType", w_getDeviceType },
	{ "isMouse", w_isMouse },
	{ 0, 0 }
};

extern "C" int luaopen_love_touch(lua_State *L)
{
	Touch *instance = instance();
	if (instance == nullptr)
	{
		luax_catchexcept(L, [&](){ instance = new love::touch::sdl::Touch(); });
	}
	else
		instance->retain();

	WrappedModule w;
	w.module = instance;
	w.name = "touch";
	w.type = &Module::type;
	w.functions = functions;
	w.types = nullptr;

	return luax_register_module(L, w);
}

} // touch
} // love
