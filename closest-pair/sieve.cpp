//Will Spencer, Period 4, 11/12/17
//Algorithm to determine the closest pair of points in a set of points in expected linear time
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <limits>
#include <ctime>
#include <vector>
#include <map>

using namespace std;

int xMulti = 100000;

//point class, x and y coordinate
class Dot{
	public:
		float x;
		float y;
};

//calculates index of dictionary based on x and y values
int dictFunct(int x, int y)
{
	return (xMulti * x) + y;
}

//calculates distance between 2 points
float calcDist(Dot p1, Dot p2)
{
	return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y, 2));
}

//finds smallest distance from a point to the other points
float checkGrid(std::vector<Dot> currGrid, float minDist, Dot temp)
{
	for(int x = 0; x < currGrid.size(); x++)
	{
		float tempDist = calcDist(temp, currGrid[x]);
		if(tempDist < minDist && tempDist != 0)
		{
			minDist = tempDist;
		}
	}
	return minDist;
}

//builds map of the points based on distance b
std::map<int, std::vector<Dot> > buildMesh(std::vector<Dot> arr, int num, double b)
{
	std::map<int, std::vector<Dot> > dict;
		
	for(int x = 0; x < num; x++) 
	{
			
		int xVal = (int)(arr[x].x / b); //row
		int yVal = (int)(arr[x].y / b); //col
			
		int dictVal = dictFunct(xVal, yVal);
		
		if ( dict.find(dictVal) != dict.end() ) //point found
		{
			std::vector<Dot> tempArr(dict[dictVal]); //problem here
			tempArr.push_back(arr[x]);
			dict[dictVal] = tempArr;
		}
		
		else //point not found
		{
			std::vector<Dot> tempArr;
			tempArr.push_back(arr[x]);
			dict[dictVal] = tempArr;
		}
		
	}
	
	return dict;
}

//expected linear algorithm to find distance between closest pair of points in arr
double filtering(int length, std::vector<Dot> arr)
{
	int temp2 = 0;
	
	clock_t begin = clock();
	int num = length;
	std::vector<Dot> finalArray (length);

	finalArray = arr;
	float finalDist;
	
	while(num > 0) //points still left in the array
	{
		float minDist = numeric_limits<float>::infinity();
		int randI = rand() % num;
		Dot xi = arr[randI]; //random point from Si
		
		//minimum distance calculation
		for(int x = 0; x < num; x++)
		{
			float d = calcDist(xi, arr[x]);
			if(d < minDist && d != 0)
			{
				minDist = d;
			}
		}
		
		float b = minDist / 3;
		
		//determine it's neighborhood
		std::map<int, std::vector<Dot> > dict = buildMesh(arr, num, b);
		
		//if only point in neighborhood, remove it
		int newLength = 0;
		Dot tempArray[num];
		for(int x = 0; x < num; x++)
		{
			Dot temp = arr[x];
			int xVal = temp.x / b; //row
			int yVal = temp.y / b; //col
			int dictVal = dictFunct(xVal, yVal);
			
			bool keep = true;
			
			if(dict[dictVal].size() == 1) //checks if only point in own cell
			{
				
				//checks if neighboring cells exist (and therefore have points)
				if(dict.find(dictFunct(xVal+1, yVal)) != dict.end()) break;
				if(dict.find(dictFunct(xVal+1, yVal+1)) != dict.end()) break;
				if(dict.find(dictFunct(xVal, yVal+1)) != dict.end()) break;
				if(dict.find(dictFunct(xVal-1, yVal+1)) != dict.end()) break;
				if(dict.find(dictFunct(xVal-1, yVal)) != dict.end()) break;
				if(dict.find(dictFunct(xVal-1, yVal-1)) != dict.end()) break;
				if(dict.find(dictFunct(xVal, yVal-1)) != dict.end()) break;
				if(dict.find(dictFunct(xVal+1, yVal-1)) != dict.end()) break;
						
				keep = false;
			}
			
			if(keep == true)
			{
				tempArray[newLength] = temp; //adds to new Array
				newLength++; //new size
			}
		}
		
		if(newLength == 0)
		{
			finalDist = minDist;
			break;
		}
		
		std::vector<Dot> arrNew (newLength);
		for(int x = 0; x < newLength; x++) //new array
		{
			arrNew[x] = tempArray[x]; //gets rid of empty spaces at the end
		}
				
		arr = arrNew;
		num = newLength; //sets new length
		
		//if(newLength == 2) break;
		
	}
	
	//make another mesh and find closest pair using smallest distances in neighborhoods
	numeric_limits<float>::infinity();
	float d = finalDist; //should be right
	std::map<int, std::vector<Dot> > dict = buildMesh(finalArray, num, d);
	
	float minDist = numeric_limits<float>::infinity();
	
	for(int x = 0; x < length; x++)
	{
		Dot temp = finalArray[x];
		int xVal = temp.x / d;
		int yVal = temp.y / d;
		int dictVal = dictFunct(xVal, yVal);
		
		//checks all parts of neighborhood for one point
		minDist = checkGrid(dict[dictFunct(xVal, yVal)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal+1, yVal+1)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal+1, yVal)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal+1, yVal-1)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal-1, yVal+1)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal-1, yVal)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal-1, yVal-1)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal, yVal+1)], minDist, temp);
		minDist = checkGrid(dict[dictFunct(xVal, yVal-1)], minDist, temp);
	}
	//printf("Minimum Distance: %f\n", minDist);
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	return elapsed_secs;
}

//prints out elapsed time for different numbers of points
int main()
{
	srand(time(NULL));
	
	ofstream file;
	file.open("graphSieve.txt");
	
	int numMax = 50000;
	int trialMax = 50;
	int increment = 100;
	
	for(int numPoints = 100; numPoints <= numMax; numPoints += increment)
	{

		printf("Points %d\n", numPoints);
		double secs = 0;
		for(int trial = 0; trial < trialMax; trial++)
		{	
			
			std::vector<Dot> arr (numPoints);
			for(int x = 0; x < numPoints; x++)
			{
				Dot dot;
				float xPoint;
				float yPoint;

				xPoint = ((float) rand() / (RAND_MAX));
				yPoint = ((float) rand() / (RAND_MAX));
				dot.x = xPoint;
				dot.y = yPoint;
				arr[x] = dot;
			}
			
			secs += filtering(numPoints, arr);
		}
		secs = secs / trialMax;
		file << secs << endl;

	}

}

