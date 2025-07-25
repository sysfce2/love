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

#ifndef LOVE_KEYBOARD_SDL_KEYBOARD_H
#define LOVE_KEYBOARD_SDL_KEYBOARD_H

// LOVE
#include "keyboard/Keyboard.h"
#include "common/EnumMap.h"

// SDL
#include <SDL3/SDL_keyboard.h>

#include <map>

namespace love
{
namespace keyboard
{
namespace sdl
{

class Keyboard : public love::keyboard::Keyboard
{
public:

	Keyboard();

	void setKeyRepeat(bool enable);
	bool hasKeyRepeat() const;
	bool isDown(const std::vector<Key> &keylist) const;
	bool isScancodeDown(const std::vector<Scancode> &scancodelist) const;
	bool isModifierActive(ModifierKey key) const;

	Key getKeyFromScancode(Scancode scancode) const;
	Scancode getScancodeFromKey(Key key) const;

	void setTextInput(bool enable);
	void setTextInput(bool enable, double x, double y, double w, double h);
	bool hasTextInput() const;
	bool hasScreenKeyboard() const;
	bool isScreenKeyboardVisible() const;

	static bool getConstant(Key in, SDL_Keycode &out);
	static bool getConstant(SDL_Keycode in, Key &out);

	static bool getConstant(Scancode in, SDL_Scancode &out);
	static bool getConstant(SDL_Scancode in, Scancode &out);

private:

	// Whether holding down a key triggers repeated key press events.
	// The real implementation is in love::event::sdl::Event::Convert.
	bool key_repeat;

	static std::map<Key, SDL_Keycode> keyToSDLKey;
	static std::map<SDL_Keycode, Key> sdlKeyToKey;

	static EnumMap<Scancode, SDL_Scancode, SDL_SCANCODE_COUNT>::Entry scancodeEntries[];
	static EnumMap<Scancode, SDL_Scancode, SDL_SCANCODE_COUNT> scancodes;

}; // Keyboard

} // sdl
} // keyboard
} // love

#endif // LOVE_KEYBOARD_SDL_KEYBOARD_H
