#include "engine.hpp"

Engine::Engine() : speed(200), level(1) {
  srand(time(NULL));
}

void Engine::run() {
  GM.removeLine();
  GM.drawGrid();
  GM.drawTetris();
  GM.drawScore();
  GM.drawLevel();
  GM.moveDown();

  if (GM.isGameOver()) {
    GM.restart();
    speed = 300;
  } else if (GM.getScore() >= GM.getLevel() * 500) {
    GM.levelUp();
    speed = 300 - 30;
  }
}

int Engine::getSpeed() {
  return speed;
}

void Engine::normalKey(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      exit(0);
      break;
    case 'r':
      GM.restart();
      break;
    default:
      break;
  }
}

void Engine::specialKey(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      GM.rotate();
      break;
    case GLUT_KEY_DOWN:
      GM.moveDown();
      break;
    case GLUT_KEY_LEFT:
      GM.moveLeft();
      break;
    case GLUT_KEY_RIGHT:
      GM.moveRight();
      break;
    default:
      break;
  }
}
