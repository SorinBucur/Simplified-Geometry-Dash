#pragma once
#include "Framework/Object2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <vector>

using namespace std;

class Obstacle:public Polygon2D
{
public:


public:
	Obstacle(Color color, bool fill):Polygon2D(color, fill)
	{
	
	}

	void translateObstacle(float tx) {
		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(tx,0);
		Transform2D::applyTransform(this);
	}

};