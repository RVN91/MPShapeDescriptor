/*
Load and assign particle information to variables from sIMPLE binary
file.

Author: Rasmus Vest Nielsen
*/

#include <iostream>
#include <vector>
#include <fstream>

/*
Safely reads and casts 8-bit (1 byte) to char from file stream assuming
little endianness. Reading 1 byte memmory blocks are chosen to ensure
independence from system types.
*/
char readU8(std::istream& file, unsigned long & bytePosition)
{
	file.seekg(bytePosition);
	char * memmoryBlock;
	int nBytes = 1;
	memmoryBlock = new char[nBytes];
	file.read(memmoryBlock, nBytes);

	bytePosition += 1;

	return * memmoryBlock;
}

/*
Safely reads and casts unsigned 32-bit (4 bytes) to integer from file stream
assuming little endianness. Reading 1 byte memmory blocks are chosen to ensure
independence from system types.
*/
unsigned int readU32(std::istream& file, unsigned long & bytePosition)
{
	file.seekg(bytePosition);
	char * memmoryBlock;
	int nBytes = 4;
	memmoryBlock = new char [nBytes];
	file.read(memmoryBlock, nBytes);
	unsigned int * val = (unsigned int *) memmoryBlock;

	bytePosition += 4;

	return *val;
}

/*
Safely reads and casts signed 32-bit (4 bytes) to integer from file stream
assuming little endianness. Reading 1 byte memmory blocks are chosen to ensure
independence from system types.
*/
signed int read32(std::istream& file, unsigned long & bytePosition)
{
	file.seekg(bytePosition);
	char * memmoryBlock;
	int nBytes = 4;
	memmoryBlock = new char [nBytes];
	file.read(memmoryBlock, nBytes);
	signed int * val = (signed int *) memmoryBlock;

	bytePosition += 4;

	return *val;
}

/*
Safely reads and casts sined 32-bit (4 bytes) to float from file stream
assuming little endianness. Reading 1 byte memmory blocks are chosen to ensure
independence from system types.
*/
float read32float(std::istream& file, unsigned long & bytePosition)
{
	file.seekg(bytePosition);
	char * memmoryBlock;
	int nBytes = 4;
	memmoryBlock = new char [nBytes];
	file.read(memmoryBlock, nBytes);

	//bitset<32> x(*memmoryBlock);
	//cout << x << endl;

	float * val = (float*) memmoryBlock;

	bytePosition += 4;

	return *val;
}

/*
Checks if local machine is small or big endian. Most modern CPU's use small
endian so it might be a waste of time.
*/
void endianTest()
{
	unsigned char test[2] = {1,0};
	short x;
	x = *(short*) test;

	if(x == 1)
	{
		std::cout << "System is little endian!" << std::endl;
	}
	else
	{
		std::cout << "System is big endian!" << std::endl;
	}
}

/*
Loads pixels from binary file into vectors of vectors.
*/
std::vector<std::pair<std::vector<signed int>, std::vector<signed int>>> getParticlePixels(const char * fileName)
{
	std::ifstream particleFile;
	particleFile.open(fileName, std::ios::binary);
	if(!particleFile.is_open())
	{
		std::cout << "Cannot open file!" << std::endl;
		//return 1;
	}

	unsigned long bytePosition = 0;

	unsigned int nParticles = readU32(particleFile, bytePosition);
	std::cout << "Number of particles: " << nParticles << std::endl;

	// Vector of pairs of vectors
	std::vector<std::pair<std::vector<signed int>, std::vector<signed int>>> particlePixelVectors;
	//std::vector<std::vector<signed int>> yParticlePixelVectors;

	for(int i = 0; i <= nParticles -1; i++)
	{
		unsigned int rsC = readU32(particleFile, bytePosition);
		char booleanChar = readU8(particleFile, bytePosition);
		unsigned int nPixels = readU32(particleFile, bytePosition);

		// x and y pixels cooridnates.
		//signed int xPixels[nPixels + 1];
		//signed int yPixels[nPixels + 1];
		std::vector<signed int> xPixels;
		std::vector<signed int> yPixels;

		for(int j = 0; j <= nPixels; j++)
		{
			//xPixels[j] = read32(particleFile, bytePosition);
			xPixels.push_back(read32(particleFile, bytePosition));
		}

		for(int j = 0; j <= nPixels; j++)
		{
			//yPixels[j] = read32(particleFile, bytePosition);
			yPixels.push_back(read32(particleFile, bytePosition));
		}

		// Populate the outer vector by vector elements.
		particlePixelVectors.push_back(std::make_pair(xPixels, yPixels));
		//particlePixelVectors.push_back(yPixels);

		unsigned int xMean = readU32(particleFile, bytePosition);
		unsigned int yMean = readU32(particleFile, bytePosition);
		float majorDim = read32float(particleFile, bytePosition);
		float minorDim = read32float(particleFile, bytePosition);
		float volume = read32float(particleFile, bytePosition);
		float mass = read32float(particleFile, bytePosition);
		float pno = readU32(particleFile, bytePosition);
		float mpno = readU32(particleFile, bytePosition);

		// Delphi strings are not fixed in size but a comes with a byte
		// length defined in the first byte of the string.
		char strLength = readU8(particleFile, bytePosition);
		//printf("Str. length: %d \n", strLength)
		bytePosition += strLength;
	}

	particleFile.close();

	return particlePixelVectors;
}
