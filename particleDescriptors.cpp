/*
Descriptors of shapes.

Author: Rasmus Vest Nielsen
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv; // Tells the compiler that the subsequent code is making use
		    // of names in the specified namespace
using namespace std;

// Global variables
int K = 0; // Contour counter. Needs to be global due to
	   // some code needs to rearranged!

/*
   Calculate the elongation (eccentricity) of a particle
   See: https://answers.opencv.org/question/44331/eccentricity-
   ielongation-of-contours/
*/
double eccentricity( vector<Point> &contour )
{
	RotatedRect ellipse = fitEllipse( contour );
	return ellipse.size.height / ellipse.size.width;
}

/*
Calculate mass centers from the spatial moments computated as:

mij = sum,xy (array(x, y)* x^j * y^i)

The mass center is given by x = m10 / m00, y = m01 / m00

m00 is the area (for binary images) or sum of grey level (for greytone
images).

See:
https://en.wikipedia.org/wiki/Image_moment
*/
void getMassCenter(vector<vector<Point>> contours, Mat imageClone)
{
	// Moments (double m00, double m10, double m01, double m20, double m11,
	//          double m02, double m30, double m21, double m12, double m03)

	// Calculate moments
	vector<Moments> mu( contours.size() );

	// Data container for center of mass
	vector<Point2f> mc( contours.size() );

	// Data container for approximated contours
	vector<vector<Point>> approxContours;
	approxContours.resize(contours.size()); // Resize number of contours

	// Data container for convex hulls
	vector<vector<Point>> convexHulls;
	convexHulls.resize(contours.size()); // Resize number of contours

	// Iterate through each contour and calculate center of mass
	for (int i = 0; i < contours.size(); i++)
	{
		// Get moments (struct)
		mu[i] = moments(contours[i], false);

		// Calculate center of mass
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

		// Calculate Feret diameters and aspect ratio
		Rect rect; // [x, y, w, h]
		rect = boundingRect(Mat(contours[i]));
		double ar = rect.width / rect.height;

		// Calculate convexity (area / convex hull area)
		convexHull(contours[i], convexHulls[i]);

		double convexity = mu[i].m00 / contourArea(convexHulls[i]);

		// Draw center of mass
		circle(imageClone, mc[i], 4, Scalar(128,255,255), -1, 8, 0);

		// Print information to terminal
		/*
		printf("Contour[%d] \n", i);
		printf("Area = %.2f \n", mu[i].m00);
		printf("Area OpenCV: %.2f \n", contourArea(contours[i]));
		printf("Perimeter or curve length: %.2f \n", arcLength(
					contours[i], true ));
		printf("Feret diameters: %.2f / %.2f \n", rect.x, rect.y);
		printf("Convexity: %.2f \n", convexity);
		printf("Aspect ratio: %.2f \n", ar);
		*/

		// Approximate minimum number of vertices to describe contours
		// using the Ramer-Dougles-Peucker algorithm. See
                // https://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93
		// Peucker_algorithm
		// approxPolyDP (inputArray, outputVector, epsilon, closed(is
		// the first and last vertices connected?))
		approxPolyDP( Mat(contours[i]), approxContours[i], 3, true);

		// Get elongation
		double elongation = 999; // No-value
		if (contours[i].size() >= 5) // Needs 5 points to calculate
		{			     // ellipse.
			// Calculate elongation
			elongation = eccentricity(contours[i]);
			//printf("Elongation: %.2f \n", elongation);
		}

		// Write shape decsriptors to .csv file
		std::ofstream file ("particle_shapes.csv", std::ios::app);
		if (file.is_open())
		{
			file << K << ",";
			file << i << ",";
			file << mu[i].m00 << ",";
			file << contourArea(contours[i]) << ",";
			file << arcLength(contours[i], true) << ",";
			file << rect.x << ",";
			file << rect.y << ",";
			file << convexity << ",";
			file << ar << ",";
			file << elongation << endl;
			file.close();
		}

		// Saving the detected contour due to multiple particles
		// might be located within the same image.
		Mat contourImage;
		contourImage = imageClone(rect);
		std::string s = "images/contour_" + std::to_string(K)
			+ ".png";
		const char* fileName = s.c_str();
		imwrite(fileName, contourImage);
		K++;
	}
}

/*
Extracts particles from binary sIMPLE files.
*/
Mat printParticles(std::pair<std::vector<signed int>,
		std::vector<signed int>> particlePixels, int particleNumber)
{
	// Extract particle vectors
	std::vector<int> xPixels = particlePixels.first;
	std::vector<int> yPixels = particlePixels.second;
	int nPixels = particlePixels.first.size();

	//printf("n pixels: %d\n", nPixels);

	// Find minimum and maximum values.
	auto xMin = std::min_element(std::begin(xPixels),
			std::end(xPixels));
	auto xMax = std::max_element(std::begin(xPixels),
			std::end(xPixels));
	auto yMin = std::min_element(std::begin(yPixels),
			std::end(yPixels));
	auto yMax = std::max_element(std::begin(yPixels),
			std::end(yPixels));

	// Offset pixel values
	std::vector<int>  xPixelsOffset (nPixels);
	std::vector<int>  yPixelsOffset (nPixels);

	for(int j = 0; j <= nPixels - 1; j++)
	{
		xPixelsOffset[j] = xPixels[j] - * xMin + 5;
	}

	for(int j = 0; j <= nPixels - 1; j++)
	{
		yPixelsOffset[j] = yPixels[j] - * yMin + 5;
	}

	// Free memory.
	xPixels.clear();
	yPixels.clear();

	// Find minimum and maximum values.
	auto xMinOffset = std::min_element(std::begin(xPixelsOffset),
			std::end(xPixelsOffset));
	auto xMaxOffset = std::max_element(std::begin(xPixelsOffset),
			std::end(xPixelsOffset));
	auto yMinOffset = std::min_element(std::begin(yPixelsOffset),
			std::end(yPixelsOffset));
	auto yMaxOffset = std::max_element(std::begin(yPixelsOffset),
			std::end(yPixelsOffset));

	/*
	// Original.
	printf("x min: %d\n", * xMin);
	printf("y min: %d\n", * yMin);
	printf("x max: %d\n", * xMax);
	printf("y max: %d\n", * yMax);

	// Offsets.
	printf("xOffset min: %d\n", * xMinOffset);
	printf("yOffset min: %d\n", * yMinOffset);
	printf("xOffset max: %d\n", * xMaxOffset);
	printf("yOffset max: %d\n", * yMaxOffset);
	*/
	// Initialize zero matrix and populate the particle coordinates
	// with ones. (REMEMBER TO ADD + 1 FOR BOUNDARIES!)
	int boundarySizeX = * xMaxOffset - * xMinOffset + 10;
	int boundarySizeY = * yMaxOffset - * yMinOffset + 10;

	//std::cout << i << std::endl;

	//printf("Matrix boundary X: %d\n", boundarySizeX);
	//printf("Matrix boundary Y: %d\n", boundarySizeY);

	// The reverse order of x and y is due to images are defined
	// with x as rows and y as columns.
	Mat image;
	image = Mat::zeros(boundarySizeY, boundarySizeX, CV_8UC1);

	// Set pixel values in matrix.
	for(int k = 0; k <= nPixels - 1; k++)
	{
		//cout << xPixelsOffset[k] << ", " << yPixelsOffset[k]
		//<< endl;
		//cout << i << endl;
		image.at<unsigned char>(yPixelsOffset[k],
				xPixelsOffset[k]) = 255;
		//printf("%d\n", image.at<unsigned char>(yPixels[i],
		//xPixels[i]));
	}

	std::string s = "images/particle_" + std::to_string(particleNumber)
		+ ".png";
	const char* fileName = s.c_str();
	imwrite(fileName, image);

	xPixelsOffset.clear();
	yPixelsOffset.clear();

	return image;
}
