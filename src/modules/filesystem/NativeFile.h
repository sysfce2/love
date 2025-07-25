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

// LOVE
#include "common/config.h"
#include "File.h"

// C
#include <cstdio>

typedef struct SDL_IOStream SDL_IOStream;

namespace love
{
namespace filesystem
{

/**
 * File which uses C's stdio. Filenames are system-dependent full paths.
 **/
class NativeFile : public File
{
public:

	NativeFile(const std::string &filename, Mode mode);
	virtual ~NativeFile();

	// Implements Stream.
	NativeFile *clone() override;
	int64 read(void* dst, int64 size) override;
	bool write(const void* data, int64 size) override;
	bool flush() override;
	int64 getSize() override;
	int64 tell() override;
	bool seek(int64 pos, SeekOrigin origin) override;

	// Implements File.
	using File::read;
	using File::write;
	bool open(Mode mode) override;
	bool close() override;
	bool isOpen() const override;
	bool isEOF() override;
	bool setBuffer(BufferMode bufmode, int64 size) override;
	BufferMode getBuffer(int64 &size) const override;
	Mode getMode() const override;
	const std::string &getFilename() const override;

private:

	NativeFile(const NativeFile &other);
	bool setupBuffering(BufferMode mode, int64 bufferSize);
	int64 bufferedRead(void* dst, int64 size);
	bool bufferedWrite(const void* data, int64 size);

	static const char *getModeString(Mode mode);

	std::string filename;

	SDL_IOStream *file;

	Mode mode;

	int8 *buffer;
	BufferMode bufferMode;
	int64 bufferSize;
	int64 bufferUsed;

}; // NativeFile

} // filesystem
} // love
