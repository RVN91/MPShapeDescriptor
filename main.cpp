#include <stdio.h>
#include "loadParticle.h"
#include "particleDescriptors.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace cv; // Tells the compiler that the subsequent code is making use
		    // of names in the specified namespace
using namespace std;

int main ()
{
	// Remove previously output files
	remove("particle_shapes.csv");

	// Write headers to .csv file
	std::ofstream file ("particle_shapes.csv", std::ios::app);
	if (file.is_open())
	{
		file << "particle_number" << ",";
		file << "contour_number" << ",";
		file << "area_from_moment" << ",";
		file << "fancy_area" << ",";
		file << "arch_length" << ",";
		file << "feret_x" << ",";
		file << "feret_y" << ",";
		file << "convexity" << ",";
		file << "aspect_ratio" << ",";
		file << "elongation" << endl;
		file.close();
	}

	const char * fileName = "T1_1.pmp";
	std::cout << "Name of input file: " << fileName << std::endl;

	std::vector<std::pair<std::vector<signed int>, std::vector<signed int>>>
		particlePixels = getParticlePixels(fileName);

	int nParticles = particlePixels.size();

	for(int particleNumber = 0; particleNumber <= nParticles - 1;
			particleNumber++)
	{
		Mat image = printParticles(particlePixels[particleNumber],
				particleNumber);
		// Apply threshold
		// threshold(InputArray, OutputArray, lower, max, type)
		Mat thresholdImage, imageClone;
		threshold(image, thresholdImage, 1,
				255, 0);
		// Find and extract contours
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(thresholdImage, contours, hierarchy, RETR_TREE,
				CHAIN_APPROX_SIMPLE);

		// Clone original image and draw upon clone
		imageClone = image.clone();
		drawContours(imageClone, contours, -1, Scalar(128,255,255), 3,
				LINE_AA);
		// Calculate mass centers
		getMassCenter(contours, imageClone);

		//imshow("threshold", thresholdImage);
		//imshow("Contours", imageClone);
		//waitKey(0);
	}

	return 0;
}
