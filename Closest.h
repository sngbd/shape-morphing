#pragma once 

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <GLUT/glut.h>
#include "Point.h"
#include "Shape.h"
#include "Vector3.h"
#include "Text.h"

class Closest
{
private:

public:
	Closest();

	std::vector<int> listOrder;

	std::vector<Point> findClosest(std::vector<Point> playerPoints, std::vector<Point> targetPoints);
	float distance(Vector3 point1, Vector3 point2);
};

