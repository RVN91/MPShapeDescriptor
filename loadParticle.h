/*
Declarations of functions for loadParticle.cpp.

Author: Rasmus Vest Nielsen
*/

#include <iostream>
#include <vector>

// Set guard
#ifndef LOADPARTICLE_H
#define LOADPARTICLE_H

// Function declaration.
char readU8(std::istream& file, unsigned long & bytePosition);
unsigned int readU32(std::istream& file, unsigned long & bytePosition);
signed int read32(std::istream& file, unsigned long & bytePosition);
float read32float(std::istream& file, unsigned long & bytePosition);
void endianTest();
std::vector<std::pair<std::vector<signed int>, std::vector<signed int>>>
getParticlePixels(const char * fileName);

#endif // End of guard
