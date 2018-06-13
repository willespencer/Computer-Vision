//Will Spencer, Period 4
//Coin Detection using OpenCV

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat countCoins(Mat image)
{
	Mat gray_image, blur_image, hough_image; //Mat images used in method
	
	//variables changed to optimize coins counted
	
	int lowThreshold = 30; //lowest Threshold for Canny
	int ratio = 2; //ratio to find maxThreshold
	int kernelSize = 3; //kernel size for Canny
	int minRad = 85; //min radius to detect circles
	int maxRad = 600; //max radius to detect circles

	cvtColor( image, gray_image, CV_BGR2GRAY ); //grayscales image and saves as gray_image

	GaussianBlur( gray_image, blur_image, Size(9, 9), 2, 2 ); //blurs image and saves as blur_image

	Canny(blur_image, blur_image, lowThreshold, lowThreshold*ratio, kernelSize ); //Canny Edge detection, replaces blur_image

	vector<Vec3f> circles; //list of all circles detected
	int rows = blur_image.rows;

	HoughCircles(blur_image, circles, CV_HOUGH_GRADIENT, 1, rows/8, 200, 100, minRad, maxRad); //fills circles list from blur_image

	int nickels = 0, dimes = 0, quarters = 0, pennies = 0, dollars = 0;
	
	Mat rgb_image(gray_image.size(), CV_8UC3); //image with red circles on it
	cv::cvtColor(gray_image, rgb_image, CV_GRAY2RGB); //converts from grayscale to RGB

	for( size_t i = 0; i < circles.size(); i++ ) //loops through circles
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		// draw the circle outline in red
		circle(rgb_image, center, radius, Scalar(0, 0, 255), 2, 8, 0);

		//count money
		if(radius <= 120) //size threshold for dimes
			dimes++;
			
		else if(radius <= 140) // size threshold for pennies
			pennies++;

		else if(radius <= 190) //threshold for nickels/dollars
		{
			Vec3b pixel = image.at<Vec3b>(center);// get pixel
			
			if((pixel[0] + pixel[1] + pixel[2]) / 3 < 150) //darker color = dollar
				dollars++;
			
			else //lighter color = nickel
				nickels++;
		}
		
		else //largest coins are quarters
			quarters++;
	}
	
	double money = dollars + (.25 * quarters) + (.1 * dimes) + (.05 * nickels) + (.01 * pennies);
	printf("Total money counted: %f\n", money); //outputs calculated money

	//returns grayscaled image with detected coints in red
	return rgb_image;
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }


    image = countCoins(image);

    namedWindow("Display Image", 0 );
    resizeWindow("Display Image", 600, 500); //makes window smaller

    //namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}
