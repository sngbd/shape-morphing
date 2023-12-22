#pragma once

#include "Point.h"
#include "Shape.h"
#include "Vector3.h"

class Text
{
private:
	Vector3 position;
	Color color;
	std::string text;
public:
	Text();
	Text(Vector3, Color, std::string);

	void drawText();

	Vector3 getPosition();
	void setPosition(Vector3);
	Color getColor();
	void setColor(Color);
	std::string getText();
	void setText(std::string);
};

