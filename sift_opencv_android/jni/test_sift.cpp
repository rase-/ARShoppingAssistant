// A simple SIFT detection program using nonfree module in OpenCV4Android.
// Created by Guohui Wang 
// Email: robertwgh_at_gmail_com
// Data: 5/22/2013

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	if( argc != 3)
	{
		cout <<" Usage: sift input_image output_image" << endl;
		return -1;
	}
	
	//cv::initModule_nonfree();
	//cout <<"initModule_nonfree() called" << endl;

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR); 
	if(! image.data )
	{
		cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	vector<KeyPoint> keypoints;
	Mat descriptors;

	// Create a SIFT keypoint detector.
	SiftFeatureDetector detector;
	detector.detect(image, keypoints);
	cout << "Detected " << (int) keypoints.size() << " keypoints" <<endl;

	// Compute feature description.
	detector.compute(image,keypoints, descriptors);
	cout << "Computed feature."<<endl;

	// Store description to "descriptors.des".
	FileStorage fs;
	fs.open("descriptors.des", FileStorage::WRITE);
	cout << "Opened file to store the features."<<endl;
	fs << "descriptors" << descriptors;
	cout << "Finished writing file."<<endl;
	fs.release();
	cout << "Released file."<<endl;

	// Show keypoints in the output image.
	Mat outputImg;
	Scalar keypointColor = Scalar(255, 0, 0);
	drawKeypoints(image, keypoints, outputImg, keypointColor, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cout << "Drew keypoints in output image file."<<endl;

#ifdef WIN32
	namedWindow("Output image", CV_WINDOW_AUTOSIZE );
	imshow("Output image", outputImg);
	waitKey(0);
#endif
	
	cout << "Generate the output image."<<endl;
	imwrite(argv[2], outputImg);

	cout << "Done."<<endl;
	return 0;
}