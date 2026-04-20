#include <stdint.h>
#include <vector>
#include <string>
#include <cstring>
#include <zlib.h>
#include <iostream>
#include <ostream>


class BitReader;

#ifndef TYPES_HPP
#define TYPES_HPP

// Primitive Integer Types (Unsigned)
using UI8 = uint8_t;
using UI16 = uint16_t;
using UI32 = uint32_t;
using TWIP = int32_t;

inline float twipToPixel(TWIP v)
{
	return v / 20.0f;
}

inline TWIP pixelToTwip(float px)
{
	return static_cast<TWIP>(px * 20.0f);
}

// Primitive Integer Types (Signed)
using SI8 = int8_t;
using SI16 = int16_t;
using SI32 = int32_t;

inline uint32_t GetEncodeU32(unsigned char *&pos)
{
	uint32_t result = pos[0];
	if (!(result & 0x00000080))
	{
		pos++;
		return result;
	}

	result = (result & 0x0000007f) | pos[1] << 7;
	if (!(result & 0x00004000))
	{
		pos += 2;
		return result;
	}

	result = (result & 0x00003fff) | pos[2] << 14;
	if (!(result & 0x00200000))
	{
		pos += 3;
		return result;
	}

	result = (result & 0x001fffff) | pos[3] << 21;
	if (!(result & 0x10000000))
	{
		pos += 4;
		return result;
	}

	result = (result & 0x0fffffff) | pos[4] << 28;
	pos += 5;
	return result;
}

// Primitive Fixed-Point Types
struct FIXED
{
	int32_t raw = 0;
	float toFloat() const { return raw / 65536.0f; }
};

struct FIXED8
{
	int16_t raw = 0;
	float toFloat() const { return raw / 256.0f; }
};

// floating point data types
using FLOAT16 = _Float16;
using FLOAT = float;
using DOUBLE = double;

// Color Data types
struct RGB
{
	uint8_t r, g, b;

	void read(BitReader &reader);
};

struct RGBA
{
	uint8_t r, g, b, a;
	void read(BitReader &reader);
};

struct ARGB
{
	uint8_t a, r, g, b;
	void read(BitReader &reader);
};

struct LanguageCode
{
	std::string Language;
	UI8 LanguageCode;

	void read(BitReader &reader);

	std::string getLanguage();
};

// Main Data Types
struct RECT
{
	TWIP xmin, xmax, ymin, ymax;

	void read(BitReader &reader);
	float getWidth();
	float getHeight();
};

struct MATRIX
{
	bool HasScale, HasRotate;
	float ScaleX = 1.0f, ScaleY = 1.0f, RotateSkew0 = 0.0f, RotateSkew1 = 0.0f;
	TWIP TranslateX = 0, TranslateY = 0;

	void read(BitReader &reader);
	float getTranslateX();
	float getTranslateY();
};

struct CXFORM
{
	float RedMultTerm = 1.0f, GreenMultTerm = 1.0f, BlueMultTerm = 1.0f;
	float RedAddTerm = 0.0f, GreenAddTerm = 0.0f, BlueAddTerm = 0.0f;
	bool HasAddTerms, HasMultTerms;
	void read(BitReader &reader);
};

struct CXFORMALPHA
{
	float RedMultTerm = 1.0f, GreenMultTerm = 1.0f, BlueMultTerm = 1.0f, AlphaMultTerm = 1.0f;
	float RedAddTerm = 0.0f, GreenAddTerm = 0.0f, BlueAddTerm = 0.0f, AlphaAddTerm = 0.0f;
	bool HasAddTerms, HasMultTerms;
	void read(BitReader &reader);
};

struct SWFHeader
{
	std::string prefix;
	UI8 version;
	UI32 FileLenght;
	RECT FrameSize;
	UI16 FrameRate;
	UI16 FrameCount;
	std::vector<UI8> fullData;

	float getFPS() const;
	int dataChecking();
	int CWSDecompression(std::vector<UI8> &compressedBuf);
	void readRemaining(BitReader &reader);
};

#endif