//Will Spencer, Period 4
//Table Detection using OpenCV

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include "opencv2/features2d/features2d.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/legacy/legacy.hpp>
//#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"


using namespace cv;

//Method that outputs a video with a box near the center of the chessboard given Video cap
//Currently outputs frame by frame during the run and outputs output.mov
void checkVideo(VideoCapture cap)
{	
	
	//Code to instantiate an output video of all the frames that will be drawn on	
	/*double frames = cap.get(CV_CAP_PROP_FPS);
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	Size imgSize = Size(width, height);
	VideoWriter output("output.mov", CV_FOURCC('m','p', '4', 'v'), frames, imgSize);*/
	
	while(1)
	{
		Mat frame1, frame2;
		cap >> frame1; //grabs a frame from input video
		cap >> frame2;
		
		if(frame1.empty() || frame2.empty()) //break if done looping thorugh fromes
		{ 
			break;
		}
		
		int minHessian = 400;
		
		SurfFeatureDetector detector(minHessian);
		vector<KeyPoint> keys1;
		vector<KeyPoint> keys2;
		
		//detect keypoints using SURF Detector
		detector.detect(frame1, keys1); 
		detector.detect(frame2, keys2);
		
		//Mat keypointImg;
		//drawKeypoints(frame, keys, keypointImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT); //draws keyPoints
		
		SurfDescriptorExtractor extractor;
		Mat descriptors1, descriptors1;
		
		//calculates feature vectors from keyPoints
		extractor.compute(frame1, keys1, descriptors1); 
		extractor.compute(frame2, keys2, descriptors2); 
		
		BFMatcher matcher(NORM_L2);
		vector< DMatch > matches;
		matcher.match(descriptors1, descriptors2, matches); //Brute force, finds matches between images
		
		Mat matchImg;
		drawMatches(frame1, keys1, frame2, keys2, matches, matchImg); 
		
		//Mat flipped;
		//flip(frame, flipped, 0); //flips frame over x axis
		
		imshow("Frame", matchImg); //displays image (flipped)
		
		/*char c=(char)waitKey(25);
		if(c==27) // Press  ESC on keyboard to exit
			break;*/

		//output << keypointImg; //outputs flipped frame to output video
	}
	
	//output.release(); //stops writing
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

	VideoCapture cap(argv[1]);

    checkVideo(cap);

    waitKey(0);

    return 0;
}
