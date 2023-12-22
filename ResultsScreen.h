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

class ResultsScreen
{
private:
public:
	ResultsScreen();

	Text titleText;
	Text scoreText;
	Text actualScoreText;
	Text bestText;
	Text actualBestText;
	Text playAgainMessage;
	Text mainMenuMessage;

	void drawResultsScreen();
	void updateScores(int score, int bestScore);
};

