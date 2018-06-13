//Will Spencer, Period 4
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <limits>
#include <time.h>
#include <ctime>


using namespace std;

float calcDist(float x1, float x2, float y1, float y2)
{
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

double nSquared(int m[][900], int numPoints)
{
	srand(time(NULL));

	clock_t begin = clock();

	int num = numPoints;
	float rows[num];
	float cols[num];
	int index1 = -1;
	int index2 = -1;
	float dist = numeric_limits<float>::infinity();

	for(int x = 0; x < num; x++)
	{
		cols[x] = ((float) rand() / (RAND_MAX));
		rows[x] = ((float) rand() / (RAND_MAX));
	}

	for(int i = 0; i < num; i++)
	{
		for(int j=i+1; j<num; j++)
		{
			float newDist = calcDist(cols[i], cols[j], rows[i], rows[j]);
			if(newDist < dist)
			{
				dist = newDist;
				index1 = i;
				index2 = j;
			}
		}
	}

	//printf("distance: %f\n", dist);
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	return elapsed_secs;

	for(int x = 0; x < num; x++)
	{
		int r = (int)(rows[x] * 300);
		int c = (int)(cols[x] * 900);
		while(c%3 != 0)
		{
			c--;
		}
		m[r][c] = 0;
		m[r][c + 1] = 0;
		m[r][c + 2] = 0;

		if(x == index1 || x == index2)
		{
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
		}
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
	
	ofstream file;
	file.open("graph.txt");
	int count = 0;
	for(int numPoints = 3; numPoints < 20000; numPoints += 100)
	{
		double secs = nSquared(m, numPoints);
		if(count % 10 == 0)
			printf("%d\n", numPoints);
		file << secs << endl;
	}
	file.close();
	
	//writeToFile(m);
}
