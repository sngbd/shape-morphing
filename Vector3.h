#pragma once

#include <iostream>
#include <vector>
#include <GLUT/glut.h>


class Vector3
{
public:
	Vector3();
	Vector3(float, float, float);
	
	float x;
	float y;
	float z;

	float magnitude();

	Vector3 operator + (Vector3 p) 
	{
		return Vector3(x + p.x, y + p.y, z + p.z);
	}
	Vector3 operator - (Vector3 p)
	{
		return Vector3(x - p.x, y - p.y, z - p.z);
	}

	Vector3 operator * (float k) {
		return Vector3(x * k, y * k, z * k);
	}
};

class Color
{
public:
	Color();
	Color(float, float, float, float);

	float r;
	float g;
	float b;
	float a;
};

