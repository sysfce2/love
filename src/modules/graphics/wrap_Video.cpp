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

#include "wrap_Video.h"

// Shove the wrap_Video.lua code directly into a raw string literal.
static const char video_lua[] =
#include "wrap_Video.lua"
;

namespace love
{
namespace graphics
{

Video *luax_checkvideo(lua_State *L, int idx)
{
	return luax_checktype<Video>(L, idx);
}

int w_Video_getStream(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	luax_pushtype(L, video->getStream());
	return 1;
}

int w_Video_getSource(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	auto source = video->getSource();
	if (source)
		luax_pushtype(L, video->getSource());
	else
		lua_pushnil(L);
	return 1;
}

int w_Video_setSource(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	if (lua_isnoneornil(L, 2))
		video->setSource(nullptr);
	else
	{
		auto source = luax_checktype<love::audio::Source>(L, 2);
		video->setSource(source);
	}
	return 0;
}

int w_Video_getWidth(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	lua_pushnumber(L, video->getWidth());
	return 1;
}

int w_Video_getHeight(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	lua_pushnumber(L, video->getHeight());
	return 1;
}

int w_Video_getDimensions(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	lua_pushnumber(L, video->getWidth());
	lua_pushnumber(L, video->getHeight());
	return 2;
}

int w_Video_getPixelWidth(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	lua_pushnumber(L, video->getPixelWidth());
	return 1;
}

int w_Video_getPixelHeight(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	lua_pushnumber(L, video->getPixelHeight());
	return 1;
}

int w_Video_getPixelDimensions(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	lua_pushnumber(L, video->getPixelWidth());
	lua_pushnumber(L, video->getPixelHeight());
	return 2;
}

int w_Video_setFilter(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	SamplerState s = video->getSamplerState();

	const char *minstr = luaL_checkstring(L, 2);
	const char *magstr = luaL_optstring(L, 3, minstr);

	if (!SamplerState::getConstant(minstr, s.minFilter))
		return luax_enumerror(L, "filter mode", SamplerState::getConstants(s.minFilter), minstr);
	if (!SamplerState::getConstant(magstr, s.magFilter))
		return luax_enumerror(L, "filter mode", SamplerState::getConstants(s.magFilter), magstr);

	s.maxAnisotropy = std::min(std::max(1, (int) luaL_optnumber(L, 4, 1.0)), LOVE_UINT8_MAX);

	luax_catchexcept(L, [&](){ video->setSamplerState(s); });
	return 0;
}

int w_Video_getFilter(lua_State *L)
{
	Video *video = luax_checkvideo(L, 1);
	const SamplerState &s = video->getSamplerState();

	const char *minstr = nullptr;
	const char *magstr = nullptr;

	if (!SamplerState::getConstant(s.minFilter, minstr))
		return luaL_error(L, "Unknown filter mode.");
	if (!SamplerState::getConstant(s.magFilter, magstr))
		return luaL_error(L, "Unknown filter mode.");

	lua_pushstring(L, minstr);
	lua_pushstring(L, magstr);
	lua_pushnumber(L, s.maxAnisotropy);
	return 3;
}

static const luaL_Reg functions[] =
{
	{ "getStream", w_Video_getStream },
	{ "getSource", w_Video_getSource },
	{ "_setSource", w_Video_setSource },
	{ "getWidth", w_Video_getWidth },
	{ "getHeight", w_Video_getHeight },
	{ "getDimensions", w_Video_getDimensions },
	{ "getPixelWidth", w_Video_getPixelWidth },
	{ "getPixelHeight", w_Video_getPixelHeight },
	{ "getPixelDimensions", w_Video_getPixelDimensions },
	{ "setFilter", w_Video_setFilter },
	{ "getFilter", w_Video_getFilter },
	{ 0, 0 }
};

int luaopen_video(lua_State *L)
{
	int ret = luax_register_type(L, &Video::type, functions, nullptr);

	luaL_loadbuffer(L, video_lua, sizeof(video_lua), "=[love \"Video.lua\"]");
	luax_gettypemetatable(L, Video::type);
	lua_call(L, 1, 0);

	return ret;
}

} // graphics
} // love
