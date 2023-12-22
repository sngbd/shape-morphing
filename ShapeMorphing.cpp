#include <iostream>
#include <GLUT/glut.h>
#include <vector>
#include <string>
#include <chrono>
#include "Point.h"
#include "Shape.h"
#include "Vector3.h"
#include "Text.h"
#include "Closest.h"
#include "MainMenu.h"
#include "ResultsScreen.h"

void init();
void reset();
void newShape();
void mainGameLoop();
void mainGameMouseClicked(int button, int state, int x, int y);
void mainMenuMouseClicked(int button, int state, int x, int y);
void resultsMenuMouseClicked(int button, int state, int x, int y);
	
Closest closest = Closest();
std::vector<int> drawOrder;

std::vector<Point> targetShape = std::vector<Point>();
std::vector<Point> oldShapePoints = std::vector<Point>();
std::vector<Point> newShapePoints = std::vector<Point>();

std::vector<Point>* currentShape = new std::vector<Point>();
std::vector<std::vector<Point> > tweenedShape = std::vector<std::vector<Point> >();
std::vector<Square>* currentShapePoints = new std::vector<Square>();

Shape mousePointer = Square(Vector3(0,0,0), 10, 10);
float mouseX;
float mouseY;

bool showOrignalShape = true;
bool showOrignalFinished = false;
bool mergeShapes = false;
bool changingShapes = false;

Shape currentGameShape = Square(Vector3(300, 300, 0), 100, 100);

std::chrono::system_clock::time_point learnStartTime;
int currentPoints;

std::chrono::system_clock::time_point changeShapeStartTime;
float shapeChangeTime = 5;

float learnTime = 5;

int numFrames = 5;
int currentFrame = 0;
int totalAnimationTime = 5000;
int frameDuration = totalAnimationTime / numFrames;

float animationProgress = 0.0;

int score = 0;
int bestScore = 9999;

int level = 0, maxLevels = 8;
float levelStartTime = 2;
int previousShape = 0;
std::chrono::system_clock::time_point endStartTime;

Text scoreText = Text(Vector3(275,20,0), Color(255,255,255,255), "Score");
Text playerScore = Text(Vector3(275,35,0), Color(255,255,255,255), "0");
Text pointsLeftText = Text(Vector3(250,575,0), Color(255,255,255,255), "Points Left");
Text currentPointsText = Text(Vector3(295,595,0), Color(255,255,255,255), "0");
Text learnText = Text(Vector3(265, 50, 0), Color(255,0,0,255), "Learn");
Text drawText = Text(Vector3(270, 50, 0), Color(0,255,0,255), "Draw");
Text welldoneText = Text(Vector3(255, 50, 0), Color(0,0,255,255), "Well Done");
Text nextShapeText = Text(Vector3(255, 50, 0), Color(0,255,255,255), "Next Shape");
Text currentLevelText = Text(Vector3(10, 20, 0), Color(255, 255, 255, 255), "Level: ");
Text currentLevel = Text(Vector3(30, 35, 0), Color(255, 255, 255, 255), "1");

bool inMainMenu = true;
MainMenu mainMenu = MainMenu();

bool inResultsScreen = false;
ResultsScreen resultsScreen = ResultsScreen();

void checkTime()
{
	auto elapsedTime = std::chrono::system_clock::now() - learnStartTime;
	
	if (std::chrono::duration<float>(elapsedTime).count() > learnTime)
	{
		showOrignalShape = false;
	}
}

Point linearInterpolation(Point p1, Point p2, float t) {
	return p1 * (1.0 - t) + p2 * t;
}

void interpolateShape(int currentFrame)
{
	currentFrame = std::min(numFrames - 1, std::max(0, currentFrame));

	for (size_t i = 0; i < currentShape->size(); i++)
	{
		currentShape->at(i) = linearInterpolation(currentShape->at(i), targetShape.at(i), static_cast<float>(currentFrame) / numFrames);
	}

	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	glColor4f(255, 0, 0, 255);
	for (size_t i = 0; i < currentShape->size(); i++)
	{
		glVertex3f(currentShape->at(i).position.x, currentShape->at(i).position.y, currentShape->at(i).position.z);
	}
	glEnd();
	glPopMatrix();
}

std::vector<Point> interpolateToNextShape()
{
	std::vector<Point> interpolatedPoints = std::vector<Point>();
	
	if (oldShapePoints.size() > newShapePoints.size())
	{
		int tempCount = oldShapePoints.size() - newShapePoints.size();
		for (size_t i = 0; i < tempCount; i++)
		{
			oldShapePoints.pop_back();
		}
	}
	if (newShapePoints.size() > oldShapePoints.size())
	{
		int oldLast = oldShapePoints.size() - 1;
		int tempCount = newShapePoints.size() - oldShapePoints.size();
		for (size_t i = 0; i < tempCount; i++)
		{
			oldShapePoints.push_back(Point(oldShapePoints.at(oldLast).position, oldShapePoints.at(oldLast).color,
				oldShapePoints.at(oldLast).scale, oldShapePoints.at(oldLast).rotation));
		}
	}

	currentFrame = std::min(numFrames - 1, std::max(0, currentFrame));

	for (size_t i = 0; i < oldShapePoints.size(); i++)
	{
		interpolatedPoints.push_back(linearInterpolation(oldShapePoints.at(i), newShapePoints.at(i), animationProgress));
	}

	return interpolatedPoints;
}

void display()
{
	if (inMainMenu)
		mainMenu.drawMainMenu();
	else if (inResultsScreen)
		resultsScreen.drawResultsScreen();
	else
		mainGameLoop();
}

void timer(int value)
{
	currentFrame++;

	currentFrame = std::min(numFrames - 1, std::max(0, currentFrame));

	glutPostRedisplay();

	glutTimerFunc(frameDuration, timer, 0);
}

void mainGameLoop()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	checkTime();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const double w = glutGet(GLUT_WINDOW_WIDTH);
	const double h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0, w, h, 0, -100, 100);

	scoreText.drawText();
	playerScore.drawText();
	pointsLeftText.drawText();
	currentPointsText.drawText();
	currentLevelText.drawText();
	currentLevel.drawText();

	if (changingShapes)
	{
		Text frameNumberText = Text(Vector3(420, 20, 0), Color(255, 255, 255, 255), "Frame: " + std::to_string(static_cast<int>(animationProgress * numFrames)) + "/" + std::to_string(numFrames));
		frameNumberText.drawText();

		std::vector<Point> interpolatedPoints = interpolateToNextShape();

		glPushMatrix();

		glBegin(GL_LINE_LOOP);
		glColor4f(255, 0, 0, 255);

		for (size_t i = 0; i < interpolatedPoints.size(); i++)
		{
			glVertex3f(interpolatedPoints.at(i).position.x, interpolatedPoints.at(i).position.y, interpolatedPoints.at(i).position.z);
		}
		glEnd();
		glPopMatrix();

		learnStartTime = std::chrono::system_clock::now();

		if (animationProgress >= 1.0)
		{
			animationProgress = 0.0;
			changingShapes = false;
		}
		
		nextShapeText.drawText();

		glPopMatrix();
		glColor4f(255, 255, 255, 255);
		mousePointer.drawShape(Vector3(mouseX, mouseY, 0));
		glPopMatrix();
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glutSwapBuffers();
		return;
	}

	if (showOrignalFinished)
	{
		welldoneText.drawText();
		
		auto elapsedTime = std::chrono::system_clock::now() - endStartTime;

		if (std::chrono::duration<float>(elapsedTime).count() > levelStartTime)
		{
			animationProgress = 0.0;
			level++;
			currentLevel.setText(std::to_string(level+1));
			if (level < maxLevels)
				newShape();
			else
			{
				if (score < bestScore)
					bestScore = score;
				resultsScreen.updateScores(score, bestScore);
				inResultsScreen = true;
				reset();
			}
		}
	}

	if (showOrignalShape || showOrignalFinished)
	{
		if (!showOrignalFinished)
			learnText.drawText();
		currentGameShape.drawShapeOutline();
	}
	else
		drawText.drawText();


	if (mergeShapes)
	{
		interpolateShape(currentFrame);
	}
	else
	{
		glPushMatrix();
		glBegin(GL_LINE_LOOP);
		for (size_t i = 0; i < currentShape->size(); i++)
		{
			glColor4f(currentShape->at(i).color.r, currentShape->at(i).color.g, currentShape->at(i).color.b, currentShape->at(i).color.a);
			glVertex3f(currentShape->at(i).position.x, currentShape->at(i).position.y, currentShape->at(i).position.z);
		}
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glColor4f(255, 255, 255, 255);
		for (size_t i = 0; i < currentShapePoints->size(); i++)
		{
			currentShapePoints->at(i).drawShape();
		}
		glPopMatrix();
	}

	glPopMatrix();
	glColor4f(255, 255, 255, 255);
	mousePointer.drawShape(Vector3(mouseX, mouseY, 0));
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();
}

void motion(int x, int y) 
{
	mouseX = x;
	mouseY = y;
}

void mouseClicks(int button, int state, int x, int y)
{
	if (changingShapes)
		return;
	if (inMainMenu)
		mainMenuMouseClicked(button, state, x, y);
	else if (inResultsScreen)
		resultsMenuMouseClicked(button, state, x, y);
	else
		mainGameMouseClicked(button, state, x, y);
}

void resultsMenuMouseClicked(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		inMainMenu = false;
		inResultsScreen = false;
		learnStartTime = std::chrono::system_clock::now();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		inMainMenu = true;
		inResultsScreen = false;
	}
}

void mainMenuMouseClicked(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		inMainMenu = false;
		learnStartTime = std::chrono::system_clock::now();
	}
	glutPostRedisplay();
}

void mainGameMouseClicked(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (currentPoints > 0)
		{
			showOrignalShape = false;

			Point point = Point(Vector3(x, y, 0), Color(255, 0, 0, 255), 1, 0);
			currentShape->push_back(point);
			Square position = Square(Vector3(x, y, 0), 10, 10);
			currentShapePoints->push_back(position);

			currentPoints--;
			currentPointsText.setText(std::to_string(currentPoints));

			if (currentPoints == 0 && !mergeShapes)
			{
				currentFrame = 0;

				mergeShapes = true;
				showOrignalFinished = true;
				currentShapePoints->clear();
				endStartTime = std::chrono::system_clock::now();
				targetShape = closest.findClosest(*currentShape, *currentGameShape.points);
				drawOrder = closest.listOrder;
				int runningTotal = 0;
				for (size_t i = 0; i < currentShape->size(); i++)
				{
					int X1 = currentShape->at(i).position.x;
					int Y1 = currentShape->at(i).position.y;
					int Z1 = currentShape->at(i).position.z;
					int X2 = targetShape.at(i).position.x;
					int Y2 = targetShape.at(i).position.y;
					int Z2 = targetShape.at(i).position.z;

					int p1 = pow(X2 - X1, 2);
					int p2 = pow(Y2 - Y1, 2);
					int p3 = pow(Z2 - Z1, 2);
					int distance = sqrtf(p1 + p2 + p3);
					runningTotal += distance;
				}
				score += runningTotal;
				playerScore.setText(std::to_string(score));
			}
		}
	}
	glutPostRedisplay();
}

void chooseShape()
{
	switch (level)
	{
	case 0:
		currentGameShape = Triangle(Vector3(300, 300, 0), 100, 100);
		break;
	case 1:
		currentGameShape = Square(Vector3(300, 300, 0), 100, 100);
		break;
	case 2:
		currentGameShape = Trapezium(Vector3(300, 300, 0), 100, 100);
		break;
	case 3:
		currentGameShape = Rhombus(Vector3(300, 300, 0), 100, 100);
		break;
	case 4:
		currentGameShape = Pentagon(Vector3(300, 300, 0), 100);
		break;
	case 5:
		currentGameShape = Arrow(Vector3(300, 300, 0), 100, 100);
		break;
	case 6:
		currentGameShape = Star(Vector3(300, 300, 0), 100);		
		break;
	case 7:
		currentGameShape = Cross(Vector3(300, 300, 0), 100, 100);
		break;
	default:
		currentGameShape = Triangle(Vector3(300, 300, 0), 100, 100);
		break;
	}
}

void init()
{
	chooseShape();
	newShapePoints = *currentGameShape.points;
	currentPoints = currentGameShape.points->size();
	currentPointsText.setText(std::to_string(currentPoints));
	learnStartTime = std::chrono::system_clock::now();
}

void reset()
{
	chooseShape();
	currentPoints = currentGameShape.points->size();
	currentPointsText.setText(std::to_string(currentPoints));
	learnStartTime = std::chrono::system_clock::now();
	showOrignalShape = true;
	showOrignalFinished = false;
	mergeShapes = false;
	score = 0;
	playerScore.setText(std::to_string(score));
	level = 0;
	levelStartTime = 2;
	currentShape->clear();
}

void newShape()
{
	changingShapes = true;
	oldShapePoints = *currentGameShape.points;
	changeShapeStartTime = std::chrono::system_clock::now();

	mergeShapes = false;
	showOrignalFinished = false;
	showOrignalShape = true;
	currentShape->clear();
	currentShapePoints->clear();
	init();
}

void keyReleased(int key, int x, int y)
{
  if (key == GLUT_KEY_RIGHT)
  {
    animationProgress += 1.0 / numFrames;
    glutPostRedisplay();
  } else if (key == GLUT_KEY_LEFT) {
    animationProgress -= 1.0 / numFrames;
    glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	srand(time(NULL));
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow("GLUT");
	
	glutDisplayFunc(display);
	glutMouseFunc(mouseClicks);
	glutPassiveMotionFunc(motion);	
	glutSpecialFunc(keyReleased);
	glutIdleFunc(display);
	init();

	std::cout << "Enter the number of frames: ";
	std::cin >> numFrames;

	if (numFrames < 5) {
		std::cerr << "Invalid number of frames. Using default value (5)." << std::endl;
	} else if (numFrames > 25) {
		std::cerr << "Invalid number of frames. Using default value (5)." << std::endl;
	}
	
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
	return 0;
}