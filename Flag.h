#pragma once
#include "Framework/Object2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include <vector>

using namespace std;

class Flag:public Object2D
{
public:
	Rectangle2D *part1;
	Rectangle2D *part2;

public:
	Flag(Point2D origin)
	{
		part1 = new Rectangle2D(origin, 5, 40, Color(1,0,0), true);
		Point2D *origin1 = new Point2D(origin.x, origin.y + 20);
		part2 = new Rectangle2D(*origin1, 20, 20, Color(1,0,0), true);
	}

	void translateFlag(float tx) {
		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(tx,0);
		Transform2D::applyTransform(part1);
		Transform2D::applyTransform(part2);
	}

};