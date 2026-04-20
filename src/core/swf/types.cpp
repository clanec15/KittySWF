#include "types.hpp"
#include "parser.hpp"
#include <zlib.h>

void RGB::read(BitReader &reader)
{
	r = reader.readUI8();
	g = reader.readUI8();
	b = reader.readUI8();
}

void RGBA::read(BitReader &reader)
{

	r = reader.readUI8();
	g = reader.readUI8();
	b = reader.readUI8();
	a = reader.readUI8();
}

void ARGB::read(BitReader &reader)
{
	a = reader.readUI8();
	r = reader.readUI8();
	g = reader.readUI8();
	b = reader.readUI8();
}

void LanguageCode::read(BitReader &reader)
{
	LanguageCode = reader.readUI8();
	switch (LanguageCode)
	{
	case 1:
		Language = "Latin";
		break; // English, French, German, etc
	case 2:
		Language = "Japanese";
		break;
	case 3:
		Language = "Korean";
		break;
	case 4:
		Language = "Simplified Chinese";
		break;
	case 5:
		Language = "Traditional Chinese";
		break;
	default:
		Language = "None Indicated";
		break;
	}
}

std::string LanguageCode::getLanguage()
{
	return Language;
}

void RECT::read(BitReader &reader)
{
	UI8 nBits = reader.readUB(5); // RECT size
	xmin = reader.readSB(nBits);
	xmax = reader.readSB(nBits);
	ymin = reader.readSB(nBits);
	ymax = reader.readSB(nBits);

	reader.align();
}

float RECT::getWidth()
{
	return twipToPixel((xmax - xmin));
}

float RECT::getHeight()
{
	return twipToPixel((ymax - ymin));
}

void MATRIX::read(BitReader &reader)
{
	HasScale = reader.readUB(1);
	if (HasScale)
	{
		UI8 NScaleBits = reader.readUB(5);
		ScaleX = reader.readFB(NScaleBits);
		ScaleY = reader.readFB(NScaleBits);
	}
	HasRotate = reader.readUB(1);
	if (HasRotate)
	{
		UI8 NRotateBits = reader.readUB(5);
		RotateSkew0 = reader.readFB(NRotateBits);
		RotateSkew1 = reader.readFB(NRotateBits);
	}

	UI8 NTranslateBits = reader.readUB(5);
	TranslateX = reader.readSB(NTranslateBits);
	TranslateY = reader.readSB(NTranslateBits);
	reader.align();
}

float MATRIX::getTranslateX()
{
	return twipToPixel(TranslateX);
}

float MATRIX::getTranslateY()
{
	return twipToPixel(TranslateY);
}

void CXFORM::read(BitReader &reader)
{
	HasAddTerms = reader.readUB(1);
	HasMultTerms = reader.readUB(1);
	uint8_t Nbits = reader.readUB(4);

	if (HasMultTerms)
	{
		RedMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
		GreenMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
		BlueMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
	}

	if (HasAddTerms)
	{
		RedAddTerm = static_cast<float>(reader.readSB(Nbits));
		GreenAddTerm = static_cast<float>(reader.readSB(Nbits));
		BlueAddTerm = static_cast<float>(reader.readSB(Nbits));
	}

	reader.align();
}

void CXFORMALPHA::read(BitReader &reader)
{
	HasAddTerms = reader.readUB(1);
	HasMultTerms = reader.readUB(1);
	uint8_t Nbits = reader.readUB(4);

	if (HasMultTerms)
	{
		RedMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
		GreenMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
		BlueMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
		AlphaMultTerm = static_cast<float>(reader.readSB(Nbits)) / 256.0f;
	}

	if (HasAddTerms)
	{
		RedAddTerm = static_cast<float>(reader.readSB(Nbits));
		GreenAddTerm = static_cast<float>(reader.readSB(Nbits));
		BlueAddTerm = static_cast<float>(reader.readSB(Nbits));
		AlphaAddTerm = static_cast<float>(reader.readSB(Nbits));
	}

	reader.align();
}

float SWFHeader::getFPS() const
{
	return static_cast<float>(FrameRate) / 256.0f;
}

int SWFHeader::dataChecking()
{
	if (prefix == "FWS")
	{
		std::cout << "Uncompressed File Detected, reading..." << std::endl;
		return 0;
	}
	else if (prefix == "CWS")
	{
		std::cout << "Compressed File Detected using the ZLIB standard, decompressing..." << std::endl;
		return 1;
	}
	else if (prefix == "ZWS")
	{
		std::cout << "Compressed File Detected using the LZMA standard, decompressing..." << std::endl;
		return 2;
	}

	return -1;
}

int SWFHeader::CWSDecompression(std::vector<UI8> &compressedBuf)
{
	uLongf payloadSize = FileLenght - 8;
	return uncompress(&fullData[8], &payloadSize, compressedBuf.data(), compressedBuf.size());
}

void SWFHeader::readRemaining(BitReader &reader)
{
	FrameSize.read(reader);
	FrameRate = reader.readUI16();
	FrameCount = reader.readUI16();
}
