#pragma once

#include <iostream>
#include <vector>
#include <GLUT/glut.h>
#include "Vector3.h"

#ifndef POINTDEFINTION
#define POINTDEFINTION

class Point
{
public:
	Point(Vector3 pos, Color col, float sca, float rot);

	Vector3 position;
	Color color;
	float scale;
	float rotation;
	
	Point operator * (float k) {
		return Point(position * k, color, scale, rotation);
	}
	Point operator + (Point p) {
		return Point(position + p.position, color, scale, rotation);
	}
	Point operator - (Point p) {
		return Point(position - p.position, color, scale, rotation);
	}
};
#endif
