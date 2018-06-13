#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
double magM [3024][4032];
double angleM [3024][4032];
double isEdge [3024][4032];
double AccumulatorM[3024][4032];

//sets RGB values of a pixel to a set color
void setColor( Mat image, Vec3b pixel, uchar color, int x, int y)
{
	pixel[0] = color;
	pixel[1] = color;
	pixel[2] = color;
	image.at<Vec3b>(Point(x,y)) = pixel;
}

void drawCircle(Mat image, int x, int y, int r)
{
	for (double angle=0; angle<=2*M_PI; angle+=0.001)
	{
		int xVal = (int) x + r * cos(angle);
		int yVal = (int) y + r * sin(angle);

		//if (yVal < 700 and yVal >= 0 and xVal * 3 + 2 < 2100 and xVal >= 0)
		//{
		Vec3b pixel = image.at<Vec3b>(Point(xVal,yVal));
		pixel[0] = 150;
		image.at<Vec3b>(Point(xVal,yVal)) = pixel;
		//}
	}

}

//blurs a single ixel by dividing by factor
int blur(Mat image, int x, int y, int factor)
{
	Vec3b pixel = image.at<Vec3b>(Point(x,y));
	return pixel[0] / factor;
}

//grayscales the image
void grayScale(Mat image)
{

    for(int y = 0; y < image.rows; y++)
    {
		for(int x = 0; x < image.cols; x++)
		{
			Vec3b pixel = image.at<Vec3b>(Point(x,y));// get pixel
			
			//makes color gray
			uchar gray = (pixel[0] + pixel[1] + pixel[2]) / 3;
			setColor(image, pixel, gray, x, y);
		}
	}
}

//gaussian blur, overlays matrix onto image
void gaussianBlur(Mat image)
{
	int blurMatrix[3][3] = {{16, 8, 16}, 
                       {8, 4, 8}, 
                       {16, 8, 16}};
                       
	for(int y = 1; y < image.rows - 1; y++) //avoids edges
    {
		for(int x = 1; x < image.cols - 1; x++)
		{
			//sum is the weighted sum of pixels by gaussian matrix
			int sum = 0;
			for(int i = -1; i <= 1; i++)
			{
				for(int j = -1; j <= 1; j++)
				{
					sum += blur(image, x+i, y+j, blurMatrix[i+1][j+1]);
				}
			}
			
			Vec3b pixel = image.at<Vec3b>(Point(x,y));
			setColor(image, pixel, sum, x, y); //sets color to sum
		}
	}
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
    
	grayScale(image);
	gaussianBlur(image);
	
	
	double xKern[3][3] = {{-1, 0, 1}, 
                       {-2, 0, 2}, 
                       {-1, 0, 1}};
    double yKern[3][3] = {{-1, -2, -1}, 
                        {0,  0,  0}, 
                        {1,  2,  1}};
    
    
	for(int y = 1; y < image.rows - 1; y++)
    {
		for(int x = 1; x < image.cols - 1; x++)
		{
			Vec3b pixel = image.at<Vec3b>(Point(x,y)); 
			double magX = 0;
			double magY = 0;
			
			for(int i = -1; i <= 1; i++)
			{
				for(int j = -1; j <= 1; j++)
				{
					int xn = x + i;
					int yn = y + j;
					Vec3b currPix = image.at<Vec3b>(Point(xn,yn)); 
					
					magX += currPix[0] * xKern[i+1][j+1]; //starts at 0, not -1
					magY += currPix[0] * yKern[i+1][j+1]; 
				}
			}
			
			magM[y][x] = sqrt(pow(magX, 2) + pow(magY, 2));
			angleM[y][x] = atan(magY / magX); //arctan
		}
	}

	//Canny algorithm, sets edges to white pixels, rest to black
	for(int y = 1; y < image.rows - 1; y++)
    {
		for(int x = 1; x < image.cols - 1; x++)
		{
			Vec3b pixel = image.at<Vec3b>(Point(x,y)); 
			double angle = angleM[y][x] * (180 / M_PI);
			double mag = magM[y][x];
			
			if(angle > 22.5 && angle <= 67.5) //diagonal
			{
				if(magM[y-1][x-1] < mag && magM[y+1][x+1] < mag) //edge
					setColor(image, pixel, 255, x, y);
				else
					setColor(image, pixel, 0, x, y); //not edge
			}
			else if(angle > -22.5 && angle <= 22.5) //horizontal
			{
				if(magM[y][x-1] < mag && magM[y][x+1] < mag) //edge
					setColor(image, pixel, 255, x, y);
				else
					setColor(image, pixel, 0, x, y); //not edge
			}
			else if(angle > -67.5 && angle <= -22.5) //diagonal
			{
				if(magM[y-1][x+1] < mag && magM[y+1][x-1] < mag) //edge
					setColor(image, pixel, 255, x, y);
				else
					setColor(image, pixel, 0, x, y); //not edge
			}
			else  //vertical
			{
				if(magM[y+1][x] < mag && magM[y-1][x] < mag) //edge
					setColor(image, pixel, 255, x, y);
				else
					setColor(image, pixel, 0, x, y); //not edge
			}
		}
	}
	
	//Thresholds
	double maxThresh = 140;
	double minThresh = 60;
	
	for(int y = 1; y < image.rows - 1; y++)
    {
		for(int x = 1; x < image.cols - 1; x++)
		{
			Vec3b pixel = image.at<Vec3b>(Point(x,y)); 
			if(magM[y][x]  > maxThresh) //edge
			{
				setColor(image, pixel, 255, x, y);
				isEdge[y][x] = 1;
			}
			else if(magM[y][x] < minThresh) //not edge
			{
				setColor(image, pixel, 0, x, y);
				isEdge[y][x] = 0;
			}
			else //might change
			{
				bool sureEdge = false;
				for(int i = -1; i <= 1; i++)
				{
					for(int j = -1; j <= 1; j++)
					{
						if(!sureEdge)
						{
							int xn = x + i;
							int yn = y + j;
							
							if(magM[yn][xn] > maxThresh) //sure-edge
								sureEdge = true;
						}
					}
				}
				if(sureEdge)	
				{
					setColor(image, pixel, 255, x, y);
					isEdge[y][x] = 1;
				}
				else //not edge
				{
					setColor(image, pixel, 0, x, y);
					isEdge[y][x] = 0;
				}
			}
		}
	}
	
	int minRad = 1;
	int maxRad = 100;
	
	//Hough transform algorithm to detect circles in image
	/*for(int y = 0; y < image.rows; y++)
    {
		for(int x = 0; x < image.cols; x++)
		{
			if(isEdge[y][x] == 1) //only applies to edge points
			{
				for(int r = 1; r < maxRad; r++)
				{
					for(int theta = 0; theta < 360; theta++)
					{
						int b = x - r * cos(theta * M_PI / 180);
						int a = y - r * sin(theta * M_PI / 180);
						AccumulatorM[a][b][r] += 1;
					}
				}
			}
		}
	}*/
	
	//finds local maxima
	/*for(int a = 0; a < image.rows; a++)
    {
		for(int b = 0; b < image.cols; b++)
		{
			for(int r = 1; r < maxRad; r++)
			{
				int v = AccumulatorM[a][b][r];
				if(v > AccumulatorM[a-1][b][r] && v > AccumulatorM[a][b-1][r] && v > AccumulatorM[a][b][r-1]
				&& v > AccumulatorM[a-1][b-1][r] && v > AccumulatorM[a][b-1][r-1] && v > AccumulatorM[a-1][b][r-1] 
				&& v > AccumulatorM[a-1][b-1][r-1] && v > AccumulatorM[a+1][b][r] && v > AccumulatorM[a][b+1][r] 
				&& v > AccumulatorM[a][b][r+1] && v > AccumulatorM[a+1][b+1][r] && v > AccumulatorM[a][b+1][r+1] 
				&& v > AccumulatorM[a+1][b][r+1] && v > AccumulatorM[a+1][b+1][r+1])
				{
					drawCircle(image, b, a, r);
				}
			}
		}
	}*/

    namedWindow("Display Image", 0 );
    //namedWindow("Display Image", WINDOW_AUTOSIZE );
    resizeWindow("Display Image", 600, 500);
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

