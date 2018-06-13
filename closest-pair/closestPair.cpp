//Will Spencer, Period 4, 11/12/17
//Program that calculates the closest pair of points either with a semi recursive method or a fully recursive method, expected to complete in nlogn time
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <limits>
#include <ctime>

using namespace std;

//Point with x and y coordinates
class Dot{
	public:
		float x;
		float y;
};

//Smallest class, index1 and index2 are indices of the closest pair, dist is the distance between them
struct Smallest
{
	int index1;
	int index2;
	float dist;
};

//sorts points by x coordinate
bool xSort(const Dot & i, const Dot & j)
{
	return i.x < j.x;
}

//sorts points by y coordinate
bool ySort(const Dot & i, const Dot & j)
{
	return i.y < j.y;
}

//calculates distance between two points
float calcDist(Dot p1, Dot p2)
{
	return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y, 2));
}

//Builds smallest class from inputs
Smallest createSmallest(int i, int j, float d)
{
	Smallest s = {i, j, d};
	return s;
}

Smallest recur(Dot arr[], int min, int max)
{
	//only two points left
	if(max - min == 1)
	{
		return createSmallest(min, max, calcDist(arr[min], arr[max]));
	}

	//three points left, finds closest pair of those 3
	else if(max - min == 2)
	{
		float minD;
		int in;
		float d1 = calcDist(arr[min], arr[min+1]);
		float d2 = calcDist(arr[min+1], arr[min+2]);
		if(d2 < d1)
		{
			minD = d2;
			in = min+1;
		}
		else
		{
			minD = d1;
			in = min;
		}
		float d3 = calcDist(arr[min+2], arr[min]);
		if(d3 < minD)
			return createSmallest(min, min+2, d3);
		else
			return createSmallest(in, in+1, minD);
	}

	//finds min distance of the sides
	int mid = (max + min) / 2;

	Smallest small1 = recur(arr, min, mid);
	Smallest small2 = recur(arr, mid + 1, max);
	Smallest currSmall;
	if(small1.dist < small2.dist)
		currSmall = small1;
	else
		currSmall = small2;
	float minD = currSmall.dist;
		
	//middle Line
	float lineX;
	if((max+min) %2 == 0)
		lineX = (arr[mid].x + arr[mid].x + 1)/2;
	else
		lineX = arr[mid].x;
		
	float lowbound = lineX - minD;
	float highbound = lineX + minD;
	
	for(int i = min; i <max+1; i++) 
	{
		for(int j=i+1; j<max+1; j++)
		{	
			//check if both points within the thing (x value - d and + d)
			if (arr[i].x > lowbound && arr[i].x < highbound && arr[j].x > lowbound && arr[j].x < highbound)
			{
				//check if opposite sides
				if((arr[i].x < lineX && arr[j].x > lineX) || (arr[i].x > lineX && arr[j].x < lineX))
				{
					float newD = calcDist(arr[i], arr[j]);
					if(newD < minD)
					{
						currSmall = createSmallest(i, j, newD);
						minD = newD;
					}
				}
			}
		}	
	}
	
	return currSmall;
}

//Fully recursive method to find closest pair, expected to run in nlogn time
Smallest recurLog(Dot arr[], int min, int max, Dot arrY[])
{
	//two points remaining
	if(max - min == 1)
	{
		return createSmallest(min, max, calcDist(arr[min], arr[max]));
	}
	//three points remaining, finds closest pair of the three
	else if(max - min == 2)
	{
		float minD;
		int in;
		float d1 = calcDist(arr[min], arr[min+1]);
		float d2 = calcDist(arr[min+1], arr[min+2]);
		if(d2 < d1)
		{
			minD = d2;
			in = min+1;
		}
		else
		{
			minD = d1;
			in = min;
		}
		float d3 = calcDist(arr[min+2], arr[min]);
		if(d3 < minD)
			return createSmallest(min, min+2, d3);
		else
			return createSmallest(in, in+1, minD);
	}
	//finds min distance of the sides
	int mid = (max + min) / 2;

	//code to split arrY
	int l1 = (mid+1) - min;
	int l2 = max-mid;
	Dot splitY1[l1];
	Dot splitY2[l2];
	
	int yCount1 = 0;
	int yCount2 = 0;
	int ySize = (max - min) + 1;
	
	for(int i = 0; i < ySize; i++)
	{
		if(arrY[i].x <= arr[mid].x)
		{
			splitY1[yCount1] = arrY[i];
			yCount1++;
		}
		else
		{
			splitY2[yCount2] = arrY[i];
			yCount2++;
		}
	}
	Smallest small1 = recurLog(arr, min, mid, splitY1);
	Smallest small2 = recurLog(arr, mid + 1, max, splitY2);
	Smallest currSmall;
	if(small1.dist < small2.dist)

		currSmall = small1;
	else
		currSmall = small2;
	float minD = currSmall.dist;
		
	//middle Line
	float lineX;
	if((max+min) %2 == 0)
		lineX = (arr[mid].x + arr[mid].x + 1)/2;
	else
		lineX = arr[mid].x;
		
	float lowbound = lineX - minD;
	float highbound = lineX + minD;
	
	//checks each point in arrY with 7 below it if it fits conditions
	for(int i = 0; i < ySize; i++) 
	{
		for(int j = i+1; j < std::min(ySize, i+8); j++) //other point
		{
			if (arrY[i].x > lowbound && arrY[i].x < highbound && arrY[j].x > lowbound && arrY[j].x < highbound) //points within band
			{
				if((arrY[i].x < lineX && arrY[j].x > lineX) || (arrY[i].x > lineX && arrY[j].x < lineX))
				{
					float newD = calcDist(arrY[i], arrY[j]);
					if(newD < minD)
					{
						currSmall = createSmallest(i, j, newD);
						minD = newD;
					}
				}
			}
		}	
	}
	
	return currSmall;
}

//Setup for the recursive helper method
double findDist(int numPoints)
{
	clock_t begin = clock();
	
	int num = numPoints;
	Dot arr[num];
	Dot arrY[num];
	int index1 = -1;
	int index2 = -1;
	float dist = numeric_limits<float>::infinity();

	for(int x = 0; x < num; x++)
	{
		Dot dot;
		dot.x = ((float) rand() / (RAND_MAX));
		dot.y = ((float) rand() / (RAND_MAX));
		arr[x] = dot;
	}
	
	//sorts points by xcoordinate in arr and ycoordinate in arrY
	std::sort(arr, arr+num, xSort);
	std::copy(arr, arr+num, arrY);
	std::sort(arrY, arrY+num, ySort);
	
	Smallest small = recurLog(arr, 0, num-1, arrY);
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	return elapsed_secs;
	
	printf("Smallest Distance: %f\n", small.dist);
}

//outputs time into "graphLog.txt"
int main()
{
	ofstream file;
	file.open("graphLog.txt");
	int count = 0;
	srand(time(NULL));
	int trialNum = 100;
	for(int numPoints = 100; numPoints <= 10000; numPoints += 100)
	{
		double secs = 0;
		for(int trial = 0; trial < trialNum; trial++)
		{	
			secs += findDist(numPoints);
		}
		secs = secs / trialNum;
		file << secs << endl;
		count++;
	}
	file.close();
}
