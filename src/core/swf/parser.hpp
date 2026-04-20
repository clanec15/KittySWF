#include "types.hpp"
#include <cstddef>
#include <iostream>
#include <cmath>
#include <filesystem>
#include <sstream>
#include <vector>
#include <fstream>

#ifndef PARSER_HPP
#define PARSER_HPP

namespace fs = std::filesystem;

class BitReader
{
	const uint8_t *data;
	size_t size;
	size_t offsetByte;
	uint8_t offsetBit;

public:
	BitReader(const uint8_t *buf, size_t sz);
	uint32_t readUB(uint8_t nBits);
	int32_t readSB(uint8_t nBits);
	float readFB(uint8_t nBits);
	UI8 readUI8();
	UI16 readUI16();
	UI32 readUI32();
	std::string readString();
	void align();
};

class SWFFile
{
	public:
	SWFHeader Header;
	bool loadFile(const std::string& filePath);

	private:

};

#endif