#pragma once
#include "Framework/Object2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Plan.h"
#include "Obstacle.h"
#include "Obstacle1.h"
#include <vector>

#define PI 3.14159265358979323846

using namespace std;

class Player:public Object2D
{
public:
	Rectangle2D *body;
	Rectangle2D *mouth;
	Rectangle2D *left_eye;
	Rectangle2D *right_eye;
	int player_dimension;
	float height;
	float angle;

public:
	Player(Point2D origin)
	{
		float x = origin.x;
		float y = origin.y;
		player_dimension = 30;
		int mouth_dimension_w = player_dimension / 6;
		int mouth_dimension_h = player_dimension * 0.6;
		int eyes_dimension = player_dimension / 10;

		angle = 0;

		Point2D *origin_player = new Point2D(x,y);
		Point2D *origin_mouth = new Point2D(x + player_dimension / 5, y + player_dimension / 6);
		Point2D *origin_left_eye = new Point2D(x + player_dimension / 5, y + player_dimension * 0.7);
		Point2D *origin_right_eye = new Point2D(x + player_dimension * 0.7, y + player_dimension * 0.7);

		body = new Rectangle2D(*origin_player, player_dimension, player_dimension, Color(0,1,0), true);
		mouth = new Rectangle2D(*origin_mouth, mouth_dimension_h, mouth_dimension_w, Color(0,0,1), true);
		right_eye = new Rectangle2D(*origin_right_eye, eyes_dimension, eyes_dimension, Color(0,0,1), true);
		left_eye = new Rectangle2D(*origin_left_eye, eyes_dimension, eyes_dimension, Color(0,0,1), true);

	}

	void applyTransform() {
		Transform2D::applyTransform(left_eye);
		Transform2D::applyTransform(right_eye);
		Transform2D::applyTransform(mouth);
		Transform2D::applyTransform(body);
	}


	void jump(float y, float rotate = 0.2) {

		if (rotate == 0) {
			angle = 0;
		}

		Transform2D::loadIdentityMatrix();

		Transform2D::translateMatrix(- body->points[0]->x - 15, - body->points[0]->y - 15);
		Transform2D::rotateMatrix(angle);
		Transform2D::translateMatrix(body->points[0]->x + 15, body->points[0]->y + 15);
		
		angle += rotate;
		if (rotate == 0) {
			angle = 0;
		}

		if (angle > 2*PI) {
			angle = 0;
		}

		Transform2D::translateMatrix(0, (body->transf_points[0]->y + body->transf_points[2]->y) / 2 - (body->points[0]->y + body->points[2]->y) / 2 + y);

		applyTransform();

	}

	bool onPlatform(Plan *p) {
		int eps = 2;
		if((40 + 15 + eps > (body->transf_points[0]->y + body->transf_points[2]->y) / 2 && 
			40 + 15 - eps < (body->transf_points[0]->y + body->transf_points[2]->y) / 2) )
			return true;
		return false;
	}

	bool onObstacle(Obstacle1 *obst) {
		int eps = 2;
		if((obst->platform->transf_points[0]->x < 230 && 
			obst->platform->transf_points[1]->x > 200 &&
			obst->platform->transf_points[2]->y + 15 + eps > (body->transf_points[0]->y + body->transf_points[2]->y) / 2 && 
			obst->platform->transf_points[2]->y + 15 - eps < (body->transf_points[0]->y + body->transf_points[2]->y) / 2) )
			return true;
		return false;
	}


	bool triangleCollision(Polygon2D *obst) {
		for(int i = 0; i < 4; i++) {
			Point2D *p1 = obst->transf_points[0];
			Point2D *p2 = obst->transf_points[1];
			Point2D *p3 = obst->transf_points[2];
			Point2D *p = body->transf_points[i];

			float s = ((p2->y - p3->y)*(p->x - p3->x) + (p3->x - p2->x)*(p->y - p3->y)) /
				((p2->y - p3->y)*(p1->x - p3->x) + (p3->x - p2->x)*(p1->y - p3->y));
			float t = ((p3->y - p1->y)*(p->x - p3->x) + (p1->x - p3->x)*(p->y - p3->y)) /
				((p2->y - p3->y)*(p1->x - p3->x) + (p3->x - p2->x)*(p1->y - p3->y));
			float x = 1.5f - s - t;

			if(s >= 0 && t >= 0 && x >= 0)
				return true;

		}
		
		return false;
	}

	bool squareCollision(Rectangle2D *obst) {
		int eps = 5;

		for(int i = 0; i < 4; i++) {

			Point2D *p1 = obst->transf_points[0];
			Point2D *p2 = obst->transf_points[3];
			Point2D *p = body->transf_points[i];

			if(p->y >= p1->y && p->y <= p2->y && p->x - eps <= p1->x && p->x + eps >= p1->x)
				return true;

		}

		return false;
	}

};