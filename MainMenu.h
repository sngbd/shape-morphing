#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <GLUT/glut.h>
#include "Point.h"
#include "Shape.h"
#include "Vector3.h"
#include "Text.h"
#include "Closest.h"

class MainMenu
{
private:
public:
	MainMenu();
	
	Text titleText;
	Text instructionsTitleText;
	Text instructionsline1Text;
	Text instructionsline2Text;
	Text instructionsline3Text;
	Text instructionsline4Text;
	Text instructionsline5Text;
	Text instructionsline6Text;
	Text instructionsline7Text;
	Text instructionsline8Text;
	Text instructionsline9Text;
	Text instructionsline10Text;
	Text clickToStartText;

	void drawMainMenu();
};

