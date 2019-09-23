/*
Declarations of functions for particleDesricptors.cpp.

Author: Rasmus Vest Nielsen
*/

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv; // Tells the compiler that the subsequent code is making use
		    // of names in the specified namespace

// Set guard
#ifndef PARTICLEDESCRIPTORS_H
#define PARTICLEDESCRIPTORS_H

// Function declaration.

Mat printParticles(std::pair<std::vector<signed int>,
		std::vector<signed int>> particlePixels, int particleNumber);
double eccentricity( std::vector<Point> &contour );
void getMassCenter(std::vector<std::vector<Point>> contours, Mat imageClone);

#endif // End of guard
