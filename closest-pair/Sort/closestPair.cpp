//Will Spencer, Period 4
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <limits>
#include <time.h>
#include <algorithm>

using namespace std;

class Dot{
	public:
		float x;
		float y;
};

bool xSort(const Dot & i, const Dot & j)
{
	return i.x < j.x;
}

bool ySort(const Dot & i, const Dot & j)
{
	return i.y < j.y;
}

float calcDist(float x1, float x2, float y1, float y2)
{
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

void nSquared(int m[][900])
{
	srand(time(NULL));

	int num = 30;
	Dot arr[num];
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
	
	std::sort(arr, arr+30, xSort);
	int r = (int)(arr[0].y * 300);
	int c = (int)(arr[0].x * 900);
	while(c%3 != 0)
	{
		c--;
	}

	m[r+2][c+1] = 0;
	m[r+2][c+4] = 0;
	m[r+2][c+7] = 0;
	m[r+1][c+7] = 0;
	m[r][c+7] = 0;
	m[r-1][c+7] = 0;
	m[r-2][c+7] = 0;
	m[r-2][c+4] = 0;
	m[r-2][c+1] = 0;
	m[r-2][c-2] = 0;
	m[r-2][c-5] = 0;
	m[r-1][c-5] = 0;
	m[r][c-5] = 0;
	m[r+1][c-5] = 0;
	m[r+2][c-5] = 0;
	m[r+2][c-2] = 0;
	
	std::sort(arr, arr+30, ySort);
	r = (int)(arr[0].y * 300);
	c = (int)(arr[0].x * 900);
	while(c%3 != 0)
	{
		c--;
	}
	
	m[r+2][c] = 0;
	m[r+2][c+3] = 0;
	m[r+2][c+6] = 0;
	m[r+1][c+6] = 0;
	m[r][c+6] = 0;
	m[r-1][c+6] = 0;
	m[r-2][c+6] = 0;
	m[r-2][c+3] = 0;
	m[r-2][c] = 0;
	m[r-2][c-3] = 0;
	m[r-2][c-6] = 0;
	m[r-1][c-6] = 0;
	m[r][c-6] = 0;
	m[r+1][c-6] = 0;
	m[r+2][c-6] = 0;
	m[r+2][c-3] = 0;

	for(int x = 0; x < num; x++)
	{
		int r = (int)(arr[x].y * 300);
		int c = (int)(arr[x].x * 900);
		while(c%3 != 0)
		{
			c--;
		}
		m[r][c] = 0;
		m[r][c + 1] = 0;
		m[r][c + 2] = 0;

	}
}

void writeToFile(int m[][900])
{
	printf("here\n");
	ofstream file;
	file.open("output.ppm");
	file << "P3 300 300 1" << endl;

	for(int row = 0; row<300; row++)
	{
		int size;
		size = 900;
		for(int x=0; x<size-1; x++)
		{
			file << m[row][x] << " ";
		}
		file << m[row][size-1] << endl;
	}
	file.close();
}

int main()
{
	int m[300][900] = {};
	for(int i = 0; i < 300; i++)
		for(int j=0; j<900; j++)
			m[i][j] = 1;
	nSquared(m);
	writeToFile(m);
}
