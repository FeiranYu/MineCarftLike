#include"Minecarft.h"


float interp(const float& y, const float& y0, const  float& y1, const  float& x0, const  float& x1)
{
	if (y < y0)return x0;
	else if (y > y1)return x1;
	else if (y0 == y1)return x0;
	return (y - y0) / (y1 - y0) * (x1 - x0) + x0;
}

float vecLength(const vector& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void vecNormal(vector& v)
{
	float length = vecLength(v);
	if (length != 0.0f) {
		float inv = 1.0f / length;
		v.x *= inv;
		v.y *= inv;
		v.z *= inv;
	}
}

void vecMul(const vector& a, const vector& b, vector& c)
{
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	c.w = 1;
}

void vecSub(const vector& a, const vector& b, vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
}

float  vecDotMul(const vector& a, const vector& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


void matMul(const matrix& a, const matrix& b, matrix& c)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				c.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
}

void showMat(const matrix& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mat.m[i][j] << "\t";
		}
		cout << endl;
	}
}