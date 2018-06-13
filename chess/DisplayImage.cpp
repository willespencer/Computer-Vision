//Will Spencer, Period 4
//Chessboard Detection using OpenCV

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

//Method to draw cube at the center of the chessboard in frame, based on points
//Drawn in color of scal and with thickness thick

void drawCube(Mat frame, vector<Point2f> points, Scalar scal, int thick)
{			
	//Points(in order) (0,0,0) (0,0,-2) (0,2,0) (0,2,-2) (2,0,0) (2,0,-2) (2,2,0) (2,2,-2)
	//lines are from points (0, 2) (0, 4) (0, 1) (1, 3) (1, 5) (2, 6) (2, 3) (3, 7) (4, 6) (4, 5) (5, 7) (6, 7)
	
	line(frame, points[0], points[2], scal, thick);
	line(frame, points[0], points[4], scal, thick);
	line(frame, points[0], points[1], scal, thick);
	line(frame, points[1], points[3], scal, thick);
	line(frame, points[1], points[5], scal, thick);
	line(frame, points[2], points[6], scal, thick);
	line(frame, points[2], points[3], scal, thick);
	line(frame, points[3], points[7], scal, thick);
	line(frame, points[4], points[6], scal, thick);
	line(frame, points[4], points[5], scal, thick);
	line(frame, points[5], points[7], scal, thick);
	line(frame, points[6], points[7], scal, thick);
}

//Method that outputs a video with a box near the center of the chessboard given Video cap
//Currently outputs frame by frame during the run and outputs output.mov
void checkVideo(VideoCapture cap)
{	
	int chessSize = 7;
	Size patternSize(chessSize, chessSize); //Number of inner corners per a chessboard row and column
	
	vector<Point3f> obj; //set of 3d corners

	//populates obj with simple 3d coordinates of chess corners
	for(int x = 0; x < chessSize; x++) 
	{
		for(int y = 0; y < chessSize; y++)
		{
			Point3f point = Point3f(x, y, 0);
			obj.push_back(point);
		}
	}
	

	//Code to instantiate an output video of all the frames that will be drawn on	
	double frames = cap.get(CV_CAP_PROP_FPS);
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	Size imgSize = Size(width, height);
	VideoWriter output("output.mov", CV_FOURCC('m','p', '4', 'v'), frames, imgSize);
	
	int count = 0;
	
	while(1)
	{
		Mat frame;
		cap >> frame; //grabs a frame from input video
		
		if(frame.empty()) //break if done looping thorugh fromes
		{ 
			break;
		}
		
		vector<Point2f> corners; //2d points on image
		
		//finds chessboard corners and outputs them to vector corners
		//Adaptive_Thresh converts to black and white, fast_check runs fast if no chessboard detected
		bool found = findChessboardCorners( frame, patternSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
        + CALIB_CB_FAST_CHECK);

		if(found) //if the chessboard was found in the image
		{
			Mat gray_image;
			cvtColor(frame, gray_image, CV_BGR2GRAY); //creates grayscale image
			
			//more accurately finds corner positions
			cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1),
				TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
				
			vector<vector<Point2f> > imgCorners; //list of sets of corners (2d)
			vector<vector<Point3f> > objCorners; //list of sets of corners (3d)
			
			imgCorners.push_back(corners); //adds corners to list of sets of corners
			objCorners.push_back(obj); //adds matching 3d vector
			
			Mat camM; //3x3 camera matrix
			Mat distCoeffs; //distortion coefficients
			vector<Mat> rVects; //vector of rotation vectors
			vector<Mat> tVects; //vector of translation vectors
			
			//calibrates camera based off of objCorners and imgCorners, calibrates camM and distCoeffs
			calibrateCamera(objCorners, imgCorners, imgSize, camM, distCoeffs, rVects, tVects); 
			
			Mat rVect; //translation vector
			Mat tVect; //rotation vector
		
			//outputs rotationVector and translationVector
			solvePnPRansac(obj, corners, camM, distCoeffs, rVect, tVect);
			
			//temporary code to keep track of how far the code has progressed
			count++;
			if(count % 20 == 0)
				printf("Frame Count: %d \n", count);
			
			//initializes 3d axis points of the cube: x/y either 0 or 2, z either 0 or -2
			vector<Point3f> axis;
			for(int x = 0; x <= 1; x++)
			{
				for(int y = 0; y <= 1; y++)
				{
					for(int z = 0; z <= 1; z++)
					{
						Point3f point = Point3f((x * 2) + 3, (y * 2) + 3, (z * -2));
						axis.push_back(point);
					}
				}
			}
			
			vector<Point2f> projectPoints;
		
			//projects 3d axis points to 2d projectPoints vector
			cv::projectPoints(axis, rVect, tVect, camM, distCoeffs, projectPoints);
     
			//puts circle corners onto the board
			for(int i=0; i < corners.size(); i++)
			{
				circle(frame, corners[i], 3, Scalar(0,0,255), -1);
			}
			
			//draws the Cube in blue, with thickness 3
			drawCube(frame, projectPoints, Scalar(255,0,0), 3); 
		}
		
		Mat flipped;
		flip(frame, flipped, 0); //flips frame over x axis
		
		imshow("Frame", flipped); //displays image (flipped)
		
		char c=(char)waitKey(25);
		if(c==27) // Press  ESC on keyboard to exit
			break;

		output << flipped; //outputs flipped frame to output video
	}
	
	output.release(); //stops writing
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
