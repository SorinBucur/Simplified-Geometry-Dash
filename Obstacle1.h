#pragma once
#include "Framework/Object2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include <vector>

using namespace std;

class Obstacle1:public Object2D
{
public:
	Rectangle2D *platform;
	float h;

public:
	Obstacle1(Point2D origin, float width, float height)
	{
		platform = new Rectangle2D(origin,width,height,Color(0,1,1),true);
		h = height;
	}

	void translateObstacle1(float tx) {
		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(tx,0);
		Transform2D::applyTransform(platform);
	}

};