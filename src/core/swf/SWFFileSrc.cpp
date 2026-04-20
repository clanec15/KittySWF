#include "./parser.hpp"

bool SWFFile::loadFile(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	std::vector<UI8> compressedBuffer;
	int size = file.tellg();



	file.seekg(0, std::ios::beg);
	char prefix[3];
	SI8 verRaw;
	SI32 fileLen;	
	file.read(prefix, 3);
	Header.prefix = std::string(prefix, sizeof(prefix));

	file.read(reinterpret_cast<char *>(&verRaw), 1);
	file.read(reinterpret_cast<char *>(&fileLen), 4);

	Header.version = verRaw;
	Header.FileLenght = fileLen;

	int dataStatus = Header.dataChecking();
	

	switch (dataStatus)
	{
	case 0:
		Header.fullData.resize(Header.FileLenght);
		std::memcpy(&Header.fullData[0], prefix, 3);
		std::memcpy(&Header.fullData[3], &verRaw, 1);
		std::memcpy(&Header.fullData[4], &fileLen, 4);
		file.read(reinterpret_cast<char *>(&Header.fullData[8]), Header.FileLenght - 8);

		break;
	
	case 1:
		compressedBuffer.resize(size - 8);
		Header.fullData.resize(Header.FileLenght);
		std::memcpy(&Header.fullData[0], prefix, 3);
		std::memcpy(&Header.fullData[3], &verRaw, 1);
		std::memcpy(&Header.fullData[4], &fileLen, 4);
		file.read(reinterpret_cast<char *>(compressedBuffer.data()), size - 8);
		int decompStatus = Header.CWSDecompression(compressedBuffer);

		if(decompStatus == Z_OK)
		{
			std::cout << "Decompression Successful..." << std::endl;
		} else if (decompStatus == Z_DATA_ERROR) {
			std::cerr << "Corrupted File!!" << std::endl;
		} else if (decompStatus == Z_BUF_ERROR) {
			std::cerr << "Buffer Overflow!!" << std::endl;
		}

		Header.fullData[0] = 'F';

		break;
	}

	BitReader reader(Header.fullData.data(), Header.fullData.size());
	for(int i = 0; i < 8; i++) reader.readUI8();
	Header.readRemaining(reader);
	return true;
}