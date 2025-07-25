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

#include "Thread.h"

namespace love
{
namespace thread
{
namespace sdl
{
Thread::Thread(Threadable *t)
	: t(t)
	, running(false)
	, thread(nullptr)
{
}

Thread::~Thread()
{
	// Clean up handle
	if (thread)
		SDL_DetachThread(thread);
}

bool Thread::start()
{
#if defined(LOVE_LINUX)
	// Temporarly block signals, as the thread inherits this mask
	love::thread::ScopedDisableSignals disableSignals;
#endif

	Lock l(mutex);

	if (running)
		return false;

	if (thread) // Clean old handle up
		SDL_WaitThread(thread, nullptr);

	// Keep the threadable around until the thread is done with it.
	// This is done before thread_runner executes because there can be a delay
	// between CreateThread and the start of the thread code's execution.
	t->retain();

	thread = SDL_CreateThread(thread_runner, t->getThreadName(), this);
	running = (thread != nullptr);

	if (!running)
		t->release(); // thread_runner is never called in this situation.

	return running;
}

void Thread::wait()
{
	{
		Lock l(mutex);
		if (!thread)
			return;
	}
	SDL_WaitThread(thread, nullptr);
	Lock l(mutex);
	running = false;
	thread = nullptr;
}

bool Thread::isRunning()
{
	Lock l(mutex);
	return running;
}

int Thread::thread_runner(void *data)
{
	Thread *self = (Thread *) data; // some compilers don't like 'this'

	self->t->threadFunction();

	{
		Lock l(self->mutex);
		self->running = false;
	}

	// This was retained in start().
	self->t->release();
	return 0;
}
} // sdl
} // thread
} // love
