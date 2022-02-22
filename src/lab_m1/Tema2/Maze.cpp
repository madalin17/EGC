#include "lab_m1/Tema2/Maze.h"

#include <iostream>
#include <time.h>
#include <stack>

using namespace m1;
using namespace std;

Maze::Maze(int size)
{
	this->size = size;
}

Maze::~Maze()
{
}

/* Set and reset everything before generating the maze */
void Maze::set()
{
	/* Clear cells and visited matrices and lists of enemies and exits */
	cells.resize(size);
	visited.resize(size);
	enemies.clear();
	exits.clear();
	totalEnemies = 0;
	numberOfExits = 0;

	/* Set up cells */
	for (int i = 0; i < size; i++) {
		cells[i].resize(size);
		visited[i].resize(size);

		for (int j = 0; j < size; j++) {
			cells[i][j] = 1;
			visited[i][j] = 0;
		}
	}

	/* Set up the center cell(hero cell), starting cell for the algorithm */
	srand(time(NULL));
	cells[size / 2][size / 2] = 0;
	visited[size / 2][size / 2] = 1;
}

/* Generate a new maze, enemies positions and exits from the maze positions */
/* Algorithm inspired from https://pastebin.com/GFH3uKrb */
void Maze::generate()
{
	set();

	int random;
	int x = size / 2, y = size / 2;
	int visited_cells = 1;
	//int total_cells = (size + 1) * (size + 1) / 4;
	int total_cells = (size - 1) * (size - 1) / 4;

	stack<int> back_trackX, back_trackY;

	/* Until all cells are visited */
	while (visited_cells < total_cells)
	{
		/* Stay inside the maze */
		if (((y > 1) && (visited[x][y - 2] == 0 && cells[x][y - 1] == 1)) ||
			((y < size - 2) && (visited[x][y + 2] == 0 && cells[x][y + 1] == 1)) ||
			((x > 1) && (visited[x - 2][y] == 0 && cells[x - 1][y] == 1)) ||
			((x < size - 2) && (visited[x + 2][y] == 0 && cells[x + 1][y] == 1))) {

			/* Randomly choose a direction to break walls */
			random = (int) (rand() % 4) + 1;

			/* GO UP */
			if (random == 1 && y > 1) {
				if (visited[x][y - 2] == 0) {
					cells[x][y - 1] = 0;
					visited[x][y - 1] = 1;

					back_trackX.push(x);
					back_trackY.push(y);

					y -= 2;
					cells[x][y] = 0;
					visited[x][y] = 1;
					visited_cells++;
				}
				else
					continue;
			}
			/* GO DOWN */
			else if (random == 2 && y < size - 2) {
				if (visited[x][y + 2] == 0) {
					cells[x][y + 1] = 0;
					visited[x][y + 1] = 1;

					back_trackX.push(x);
					back_trackY.push(y);

					y += 2;
					cells[x][y] = 0;
					visited[x][y] = 1;
					visited_cells++;
				}
				else
					continue;
			}
			/* GO LEFT */
			else if (random == 3 && x > 1) {
				if (visited[x - 2][y] == 0) {
					cells[x - 1][y] = 0;
					visited[x - 1][y] = 1;

					back_trackX.push(x);
					back_trackY.push(y);

					x -= 2;
					cells[x][y] = 0;
					visited[x][y] = 1;
					visited_cells++;
				}
				else
					continue;
			}
			/* GO RIGHT */
			else if (random == 4 && x < size - 2) {
				if (visited[x + 2][y] == 0) {
					cells[x + 1][y] = 0;
					visited[x + 1][y] = 1;

					back_trackX.push(x);
					back_trackY.push(y);

					x += 2;
					cells[x][y] = 0;
					visited[x][y] = 1;
					visited_cells++;
				}
				else
					continue;
			}
		}
		/* Go back if can't advance in the maze without looping */
		else {
			x = back_trackX.top();
			back_trackX.pop();

			y = back_trackY.top();
			back_trackY.pop();
		}
	}

	createExits();
	createEnemies();
}

/* Generate random exits(at least one) for the newly generated maze with no exits */
/* Generate bosses on each exit */
void Maze::createExits() 
{
	int random;
	bool first = true;

	for (int i = 0; i < size; i++) {
		/* If on the second line is not a wall */
		if (cells[1][i] != 1) {
			random = (int)(rand() % EXITS_FACTOR);
			/* Randomly generate or not an exit */
			if (random == 0 || first) {
				cells[0][i] = 0;
				exits.push_back(make_pair(BOX_LEN * (-size / 2 + i), BOX_LEN * -size / 2));
				numberOfExits++;
				first = false;

				/* Create enemy bisses on each exit */
				Enemy3d* enemy = new Enemy3d(glm::vec3(BOX_LEN * (-size / 2 + i), 0, BOX_LEN * -size / 2), BOSS_ENEMY_SPEED);
				enemy->isBoss(M_PI);
				enemies.push_back(enemy);
			}
		}

		/* If on the second column is not a wall */
		if (cells[i][1] != 1) {
			random = (int)(rand() % EXITS_FACTOR);
			/* Randomly generate or not an exit */
			if (random == 0 || first) {
				cells[i][0] = 0;
				exits.push_back(make_pair(BOX_LEN * -size / 2, BOX_LEN * (-size / 2 + i)));
				numberOfExits++;
				first = false;

				/* Create enemy bisses on each exit */
				Enemy3d* enemy = new Enemy3d(glm::vec3(BOX_LEN * -size / 2, 0, BOX_LEN * (-size / 2 + i)), BOSS_ENEMY_SPEED);
				enemy->isBoss(3 * M_PI_2);
				enemies.push_back(enemy);
			}
		}

		/* If on the second to last line is not a wall */
		if (cells[size - 2][i] != 1) {
			random = (int)(rand() % EXITS_FACTOR);
			/* Randomly generate or not an exit */
			if (random == 0 || first) {
				cells[size - 1][i] = 0;
				exits.push_back(make_pair(BOX_LEN * (-size / 2 + i), BOX_LEN * size / 2));
				numberOfExits++;
				first = false;

				/* Create enemy bisses on each exit */
				Enemy3d* enemy = new Enemy3d(glm::vec3(BOX_LEN * (-size / 2 + i), 0, BOX_LEN * size / 2), BOSS_ENEMY_SPEED);
				enemy->isBoss(0);
				enemies.push_back(enemy);
			}
		}

		/* If on the second to last column is not a wall */
		if (cells[i][size - 2] != 1) {
			random = (int)(rand() % EXITS_FACTOR);
			/* Randomly generate or not an exit */
			if (random == 0 || first) {
				cells[i][size - 1] = 0;
				exits.push_back(make_pair(BOX_LEN * size / 2, BOX_LEN * (-size / 2 + i)));
				numberOfExits++;
				first = false;

				/* Create enemy bisses on each exit */
				Enemy3d* enemy = new Enemy3d(glm::vec3(BOX_LEN * size / 2, 0, BOX_LEN * (-size / 2 + i)), BOSS_ENEMY_SPEED);
				enemy->isBoss(M_PI_2);
				enemies.push_back(enemy);
			}
		}
	}
}

/* Generate enemies inside the maze */
void Maze::createEnemies()
{
	for (int i = 1; i < size - 1; i++) {
		for (int j = 1; j < size - 1; j++) {
			/* For a cell that is not a wall, and not in the center */
			if (i != size / 2 && j != size / 2 && cells[i][j] != 1) {
				int random = (int) (rand() % ENEMY_FACTOR);
				float speed = MINIMUM_SPEED + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				if (random == 0) {
					/* Generate an enemy in this box with the random speed */
					cells[i][j] = 2;

					int z = BOX_LEN * (-size / 2 + i);
					int x = BOX_LEN * (-size / 2 + j);

					/* Create a basic enemy in this box */
					enemies.push_back(new Enemy3d(glm::vec3(x, 0, z), speed));
					totalEnemies++;
				}
			}
		}
	}
}

/* Printing the maze */
void Maze::print()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << cells[i][j] << " ";
		}
		cout << endl;
	}
}

/* Printing the exits */
void Maze::printExits()
{
	for (int i = 0; i < numberOfExits; i++) {
		cout << "(" << exits[i].first << ", " << exits[i].second << ")" << (i == numberOfExits - 1 ? "\n" : ", ");
	}
}
