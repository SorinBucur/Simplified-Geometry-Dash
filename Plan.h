#pragma once
#include "Framework/Object2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include <vector>

using namespace std;

class Plan:public Object2D
{
public:
	Rectangle2D *platform;
	float h;

public:
	Plan(Point2D origin, float width, float height)
	{
		platform = new Rectangle2D(origin,width,height,Color(0,1,1),true);
		h = height;
	}

	void translatePlan(float tx) {
		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(tx,0);
		Transform2D::applyTransform(platform);
	}

};