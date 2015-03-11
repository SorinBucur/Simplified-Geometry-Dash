#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "Player.h"
#include "Plan.h"
#include "Obstacle.h"
#include "Obstacle1.h"
#include "Flag.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#define PI 3.14159265358979323846

using namespace std;

int initial_height = 40;
int jump_height = 150, base_height = initial_height, old_height;
int lungime_plan = 10000;

Player *player;
Plan *plan;

Plan *vector_plan[10];
Obstacle *vector_obstacle[10];
Obstacle1 *vector_obstacle1[10];
vector<Flag*> vector_flag;

Visual2D *game, *stats;

float tx = 0;
int pas = 0;
int inc = 0;

bool can_jump = true;
bool dead = false;
int scor = 0;
int nr_vieti = 3;
bool paused = false;

Text *score, *lifes;


void DrawingWindow::init() {
	player = new Player(Point2D(200,initial_height));

	game = new Visual2D(0.0f,0.0f,(float)DrawingWindow::width,5 * (float)DrawingWindow::height / 6,0,0,DrawingWindow::width,DrawingWindow::height);
	stats = new Visual2D(0.0f,0.0f,(float)DrawingWindow::width,(float)DrawingWindow::height,0,0,DrawingWindow::width,DrawingWindow::height / 6);
	
	addVisual2D(game);
	addVisual2D(stats);

	game->cadruFereastra(Color(0,0,0));
	stats->cadruFereastra(Color(0,0,0));

	score = new Text("Score: 0", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 1.5), Color(0, 0, 0));
	lifes = new Text("Lifes remaining: 3", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
	addText_to_Visual2D(score, stats);
	addText_to_Visual2D(lifes, stats);
	
	addObject2D_to_Visual2D(player->left_eye, game);
	addObject2D_to_Visual2D(player->right_eye, game);
	addObject2D_to_Visual2D(player->mouth, game);
	addObject2D_to_Visual2D(player->body, game);

	Plan *plan = new Plan(Point2D(0, 0), lungime_plan, initial_height);
	addObject2D_to_Visual2D(plan->platform, game);

	vector_flag.resize(10);

	for(int i = 0; i < 10; i++) {
		
		Point2D *p = new Point2D(i * 2000 + 195, initial_height);
		vector_flag[i] = new Flag(*p);

		Point2D *p1 = new Point2D(i * 1000 + 500, initial_height);
		Point2D *p2 = new Point2D(i * 1000 + 500 + 50, initial_height);
		Point2D *p3 = new Point2D(i * 1000 + 500 + 25, initial_height + 50);
		vector_obstacle[i] = new Obstacle(Color(1,0,1),true);
		vector_obstacle[i]->addPoint(*p1);
		vector_obstacle[i]->addPoint(*p2);
		vector_obstacle[i]->addPoint(*p3);

		Point2D *p11 = new Point2D(i * 1000 + 900, initial_height);
		vector_obstacle1[i] = new Obstacle1(*p11, 100, 50);

		addObject2D_to_Visual2D(vector_obstacle[i], game);
		addObject2D_to_Visual2D(vector_obstacle1[i]->platform, game);
		addObject2D_to_Visual2D(vector_flag[i]->part1, game);
		addObject2D_to_Visual2D(vector_flag[i]->part2, game);

	}
	
	player->height = initial_height;
	vector_flag.resize(7);
	
}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	if (paused)
		return;
	
	if (base_height > 20)
		old_height = base_height;
	
	base_height = 20;
	
	for(int i = 0; i < 10; i++) {
		vector_obstacle[i]->translateObstacle(tx);
		vector_obstacle1[i]->translateObstacle1(tx);

		if (player->triangleCollision(vector_obstacle[i])) {
			nr_vieti --;
			removeText_from_Visual2D(lifes, stats);
			if (nr_vieti == 2) {
				lifes = new Text("Lifes remaining: 2", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
				addText_to_Visual2D(lifes, stats);
			}
			if (nr_vieti == 1) {
				lifes = new Text("Lifes remaining: 1", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
				addText_to_Visual2D(lifes, stats);
			}
			if (nr_vieti == 0) {
				lifes = new Text("GAME OVER!", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
				addText_to_Visual2D(lifes, stats);
				paused = true;
			}

			PlaySound("sound.WAV", NULL, SND_ASYNC);
			tx = (player->body->transf_points[0]->x + player->body->transf_points[2]->x) / 2 - vector_flag[0]->part1->points[0]->x - 25 - 15;

			dead = true;
		
			base_height = 40;
			player->angle = 0;
			pas = 0;

			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(tx,0);

			for (auto flag : vector_flag) {
				Transform2D::applyTransform(flag->part1);
				Transform2D::applyTransform(flag->part2);
			}
			for(auto obstacle : vector_obstacle) {
				Transform2D::applyTransform(obstacle);
			}
			for(auto obstacle1 : vector_obstacle1) {
				Transform2D::applyTransform(obstacle1->platform);
			}
			
			Transform2D::loadIdentityMatrix();
			player->applyTransform();

			paused = true;
			return;
		}

		if (player->squareCollision(vector_obstacle1[i]->platform)) {
			nr_vieti --;
			removeText_from_Visual2D(lifes, stats);
			if (nr_vieti == 2) {
				lifes = new Text("Lifes remaining: 2", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
				addText_to_Visual2D(lifes, stats);
			}
			if (nr_vieti == 1) {
				lifes = new Text("Lifes remaining: 1", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
				addText_to_Visual2D(lifes, stats);
			}
			if (nr_vieti == 0) {
				lifes = new Text("GAME OVER!", Point2D(DrawingWindow::width / 8, DrawingWindow::height / 4), Color(0, 0, 0));
				addText_to_Visual2D(lifes, stats);
				paused = true;
			}

			PlaySound("sound.WAV", NULL, SND_ASYNC);
			tx = (player->body->transf_points[0]->x + player->body->transf_points[2]->x) / 2 - vector_flag[0]->part1->points[0]->x - 25 - 15;

			dead = true;
		
			base_height = 40;
			player->angle = 0;
			pas = 0;

			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(tx,0);

			for (auto flag : vector_flag) {
				Transform2D::applyTransform(flag->part1);
				Transform2D::applyTransform(flag->part2);
			}
			for(auto obstacle : vector_obstacle) {
				Transform2D::applyTransform(obstacle);
			}
			for(auto obstacle1 : vector_obstacle1) {
				Transform2D::applyTransform(obstacle1->platform);
			}
			
			Transform2D::loadIdentityMatrix();
			player->applyTransform();

			paused = true;
			return;
		}

		
		if (player->onPlatform(plan)) {
			base_height = 40;
		}

		if (player->onObstacle(vector_obstacle1[i])) {
			base_height = vector_obstacle1[i]->platform->transf_points[3]->y;
		}


	}

	for (auto flag : vector_flag) {
		flag->translateFlag(tx);
	}

	tx -= 5;

	if (vector_flag.size() == 1) {
		PlaySound("sound.WAV", NULL, SND_ASYNC);
		Text *winner = new Text("WINNER!", Point2D(DrawingWindow::width / 1.5, DrawingWindow::height / 2.5), Color(0, 0, 0));
		addText_to_Visual2D(winner, stats);
		paused = true;
		return;
	}


	if (vector_flag[1]->part1->transf_points[0]->x < 200) {
		PlaySound("sound.WAV", NULL, SND_ASYNC);
		scor ++;
		vector_flag.erase(vector_flag.begin());
		removeText_from_Visual2D(score, stats);
		if (scor == 1) {
			score->text = "Score: 1";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 1) {
			score->text = "Score: 1";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 2) {
			score->text = "Score: 2";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 3) {
			score->text = "Score: 3";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 4) {
			score->text = "Score: 4";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 5) {
			score->text = "Score: 5";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 6) {
			score->text = "Score: 6";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 7) {
			score->text = "Score: 7";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 8) {
			score->text = "Score: 8";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 9) {
			score->text = "Score: 9";
			addText_to_Visual2D(score, stats);
		}
		if (scor == 10) {
			score->text = "Score: 10";
			addText_to_Visual2D(score, stats);
		}


	}

	
	if (base_height < old_height && can_jump == true) {
		can_jump = false;
		inc = -4;
		player->angle = 0;
	}

	if (can_jump == false)  {
		pas += inc;
		
		if(pas >= jump_height) {
			inc = -4;
		}

		if((player->body->transf_points[0]->y + player->body->transf_points[2]->y) / 2 + inc - 15 <= base_height) {
			can_jump = true;
			pas = - (player->body->transf_points[0]->y + player->body->transf_points[2]->y) / 2 + base_height + 15;
			player->jump(pas, 0);
			player->angle = 0;
			pas = 0;
			inc = 4;
		
		} else {
			player->jump(inc);
		}
	}

}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{

}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
  	if (key == 32) {
		PlaySound("sound.WAV", NULL, SND_ASYNC);
		if (paused == true)
			paused = false;
		else if (can_jump == true) {
			inc = 4;
			can_jump = false;
		}
	}
	
	if (key == 27)
		exit(0);
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{

}


int main(int argc, char** argv)
{
	DrawingWindow dw(argc, argv, 600, 600, 200, 100, "Laborator EGC");

	dw.init();

	dw.run();
	return 0;

}