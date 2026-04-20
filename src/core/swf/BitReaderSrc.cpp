#include "./parser.hpp"

BitReader::BitReader(const uint8_t *buf, size_t sz) : data(buf), size(sz), offsetByte(0), offsetBit(0) {}

uint32_t BitReader::readUB(uint8_t nBits)
{
	if(nBits == 0){return 0;}


	uint32_t result = 0;

	

	for (uint8_t i = 0; i < nBits; ++i)
	{

		if (offsetByte >= size)
		{
			std::cerr << "PARSING ERROR" << std::endl;
			return 0;
		}

		result = (result << 1) | (data[offsetByte] >> (7 - offsetBit)) & 1;
		offsetBit += 1;

		if (offsetBit >= 8)
		{
			offsetBit = 0;
			offsetByte += 1;
		}
	}

	return result;
}

int32_t BitReader::readSB(uint8_t nBits)
{
	uint32_t prevResult = readUB(nBits);

	if(nBits > 0 && (prevResult & (1 << (nBits - 1)))){
		prevResult = prevResult | (0xFFFFFFFF << nBits);
		return prevResult;
	} else {
		return prevResult;
	}

}

float BitReader::readFB(uint8_t nBits)
{

	if(nBits == 0) return 0.0f;

	FIXED val;

 	val.raw = readSB(nBits);

	return val.toFloat();
}

UI8 BitReader::readUI8()
{	
	align();
	UI8 result = data[offsetByte];
	offsetByte += 1;
	return result;
}

UI16 BitReader::readUI16()
{
	align();

	if(offsetByte + 2 > size){
		throw std::overflow_error("SWF Premature EOB");
	}


	UI8 b0 = data[offsetByte];
	UI8 b1 = data[offsetByte + 1];

	UI16 result = (b0) | (b1 << 8);
	offsetByte += 2;

	return result;
}

UI32 BitReader::readUI32()
{
	align();

	if(offsetByte + 4 > size){
		throw std::overflow_error("SWF premature EOB");
	}

	
	UI8 b0 = data[offsetByte];
	UI8 b1 = data[offsetByte + 1];
	UI8 b2 = data[offsetByte + 2];
	UI8 b3 = data[offsetByte + 3];

	UI32 result = (static_cast<UI32>(b0)) | 
	(static_cast<UI32> (b1) << 8)  | 
	(static_cast<UI32> (b2) << 16) | 
	(static_cast<UI32> (b3) << 24);
	offsetByte += 4;

	return result;
}

std::string BitReader::readString()
{
	align();
	std::string result;

	
	while(offsetByte < size)
	{

		char c = static_cast<char>(data[offsetByte]);
		offsetByte++;

		if(c == 0x00){
			break;
		}

		result += c;
	}

	return result;
}

void BitReader::align()
{
	if(offsetBit > 0)
	{
		offsetBit = 0;
		offsetByte++;
	}
}
