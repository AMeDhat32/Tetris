#include "game.hpp"

#include <GL/glut.h>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

const int SHAPES[7][4][4] = {
    0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0};

const Color colors[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.5f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f};

Game::Game() : x(3), y(0), level(1) {
  memset(shape, 0, sizeof(shape));
  memset(grid, 0, sizeof(grid));
  randomTetris();
}

Color Game::getColor() const { return color; }

void Game::randomTetris() {
  srand(time(NULL));
  int random = rand() % 7;
  memcpy(shape, SHAPES[random], 16 * sizeof(int));
  color = colors[random];
}

void Game::drawGrid() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j][0] != 0 || grid[i][j][1] != 0 || grid[i][j][2] != 0) {
        int x1 = j * 8, y1 = i * 8;
        int x2 = x1 + 8, y2 = y1 + 8;
        glColor3f(grid[i][j][0], grid[i][j][1], grid[i][j][2]);
        glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
      }
    }
  }
}

void Game::drawTetris() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (shape[i][j] == 1) {
        int x1 = (j + x) * 8, y1 = (i + y) * 8;
        int x2 = x1 + 8, y2 = y1 + 8;
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
      }
}

int Game::collision() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (shape[i][j] == 1) {
        int tx = x + j, ty = y + i;
        if (tx < 0 || tx > 9)
          return 1;
        else if (ty > 19 || grid[ty][tx][0] == 1 || grid[ty][tx][1] == 1 || grid[ty][tx][2] == 1)
          return 2;
      }
  return -1;
}

void Game::update() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (shape[i][j] == 1) {
        grid[y + i][x + j][0] = color.r;
        grid[y + i][x + j][1] = color.g;
        grid[y + i][x + j][2] = color.b;
      }
  x = 3;
  y = 0;
  randomTetris();
}

void Game::drawScore() {
  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2f(10, 10);
  std::string scoreStr = "Score: " + std::to_string(score);
  for (const char c : scoreStr) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
}

void Game::resetLevel() {
  level = 1;
}

int Game::getLevel() const { return level; }

void Game::drawLevel() {
  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2f(10, 15);
  std::string levelStr = "Level: " + std::to_string(level);
  for (const char c : levelStr) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
}

void Game::rotate() {
  int tmp[4][4] = {0};
  memcpy(tmp, shape, sizeof(shape));
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      shape[i][j] = tmp[3 - j][i];
  if (collision() != -1) memcpy(shape, tmp, sizeof(shape));
}

void Game::removeLine() {
  int linesCleared = 0;
  for (int i = 19; i > 0; i--) {
    int count = 0;
    for (int j = 0; j < 10; j++)
      if (grid[i][j][0] == 1 || grid[i][j][1] == 1 || grid[i][j][2] == 1) count++;
    if (count == 10) linesCleared++;
  }

  if (linesCleared > 0) {
    for (int i = 19; i > 0; i--) {
      int count = 0;
      for (int j = 0; j < 10; j++)
        if (grid[i][j][0] == 1 || grid[i][j][1] == 1 || grid[i][j][2] == 1) count++;

      if (count == 10) {
        for (int t = i; t > 0; t--)
          for (int k = 0; k < 10; k++)
            for (int l = 0; l < 3; l++)
              grid[t][k][l] = grid[t - 1][k][l];
        i++;
      }
    }

    switch (linesCleared) {
      case 1:
        score += 100;
        break;
      case 2:
        score += 200;
        break;
      case 3:
        score += 300;
        break;
      case 4:
        score += 1200;
        break;
      default:
        break;
    }

    glutPostRedisplay();
  }
}

int Game::getScore() const { return score; }

void Game::levelUp() {
  level++;
}

bool Game::isGameOver() {
  for (int i = 0; i < 10; i++)
    if (grid[4][i][0] == 1 || grid[4][i][1] == 1 || grid[4][i][2] == 1) return 1;
  return 0;
}

void Game::restart() {
  memset(grid, 0, sizeof(grid));
  x = 3, y = 0;
  score = 0;
  resetLevel();
  randomTetris();
}

void Game::moveDown() {
  y++;
  if (collision() == 2) {
    y--;
    update();
  }
}
void Game::moveLeft() {
  x--;
  if (collision() != -1) x++;
}

void Game::moveRight() {
  x++;
  if (collision() != -1) x--;
}
