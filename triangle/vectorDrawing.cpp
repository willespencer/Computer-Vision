//Will Spencer, Period 4
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#define PI 3.141592

using namespace std;

void changeColor(int m[][2100], int x, int y)
{
	for(int i=0; i<3; i++) m[y][x*3+i] = 0;
}

void drawSingleCircle(int m[][2100], double centerX, double centerY, double radius)
{
	for (double angle=0; angle<=2*PI; angle+=0.001)
	{
		int xVal = (int) centerX + radius * cos(angle);
		int yVal = (int) centerY + radius * sin(angle);

		if (yVal < 700 and yVal >= 0 and xVal * 3 + 2 < 2100 and xVal >= 0)
		{
			changeColor(m, xVal, yVal);
		}
	}

}

void drawCircles(int m[][2100], int rows[], int cols[])
{
	//general calcs
	double d2 = sqrt(pow(rows[1]-rows[0], 2)+pow(cols[1]-cols[0], 2));
	double d0 = sqrt(pow(rows[2]-rows[1], 2)+pow(cols[2]-cols[1], 2));
	double d1 = sqrt(pow(rows[0]-rows[2], 2)+pow(cols[0]-cols[2], 2));
	
	//radii
	double semi = .5 * (d1 + d2 + d0);
	double inR = sqrt(((semi-d1) * (semi-d2) * (semi - d0))/semi);
	double outR = (d1 * d2 * d0) / (4 * inR * semi);
	
	//circumcenter calcs
	double temp = 2*(cols[0] * (rows[1]-rows[2])+cols[1] * (rows[2]-rows[0]) + cols[2] * (rows[0] - rows[1]));
	double square0 = pow(cols[0], 2) + pow(rows[0], 2);
	double square1 = pow(cols[1], 2) + pow(rows[1], 2);
	double square2 = pow(cols[2], 2) + pow(rows[2], 2);
	double circumX = (1/temp)*(square0 * (rows[1]-rows[2])+square1*(rows[2]-rows[0])+(square2*(rows[0]-rows[1])));
	double circumY = (1/temp)*(square0 * (cols[2]-cols[1])+square1*(cols[0]-cols[2])+(square2*(cols[1]-cols[0])));
	
	//incenter calcs
	double inX = (d0 * cols[0] + d1 * cols[1] + d2 * cols[2]) / (d1+d2+d0);
	double inY = (d0 * rows[0] + d1 * rows[1] + d2 * rows[2]) / (d1+d2+d0);

	//draw
	drawSingleCircle(m, inX, inY, inR);
	drawSingleCircle(m, circumX, circumY, outR);

}

void drawLine(int m[][2100], int x1, int y1, int x2, int y2)
{
	bool yLarger = (abs(y2-y1) > abs(x2-x1));
	if(yLarger)
	{
		swap(x1, y1);
		swap(x2, y2);
	}
	if(x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	int dx = x2 - x1;
	int dy = abs(y2 - y1);
	double error = dy - dx;
	int ystep = (y1 < y2) ? 1 : -1;
	int j  = y1;

	for(int i=x1; i<x2; i++)
	{
		if(yLarger)
		{
			changeColor(m, j, i);
		}
		else
		{
			changeColor(m, i, j);
		}
		if(error >= 0)
		{
			j += ystep;
			error -= dx;
		}
		error += dy;
	}
}

void randomDots(int m[][2100])
{
	srand(time(0));	
	
	int rows[3];
        int cols[3];
      
        for (int n=0; n<3; n++)
        {
		cols[n] = (rand() % 700);
              	rows[n] = rand() % 700;
		changeColor(m, cols[n], rows[n]);
      	} 

	drawLine(m, cols[0], rows[0], cols[1], rows[1]);
	drawLine(m, cols[1], rows[1], cols[2], rows[2]);
	drawLine(m, cols[2], rows[2], cols[0], rows[0]);
	drawCircles(m, rows, cols);
}

void writeToFile(int m[][2100])
{
        ofstream file;
        file.open("drawingOutput.ppm");
        file << "P3 700 700 1" << endl;

	for (int row=0; row<700; row++)
        {
		int size;
		size = 2100;
                for(int x=0; x<size - 1; x++)
		{
			file << m[row][x] <<  " ";
		}
		file << m[row][size-1] << endl;
	}
	file.close();
}

int main()
{
	int m[700][2100] = {};
	for(int i = 0; i < 700; i++)
		for (int j = 0; j < 2100; j++)
			m[i][j] = 1; 

	randomDots(m);

	writeToFile(m);
}
