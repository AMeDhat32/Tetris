// Author: TOP SECRET Team
// Date: 26.11.2023

#include <GL/glut.h>

#include "Includes/engine.hpp"

Engine eng;

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  eng.run();

  glutSwapBuffers();
}

void onTimer(int value) {
  if (value == 0) {
    glutTimerFunc(eng.getSpeed(), onTimer, value);

    glutPostRedisplay();
  }
}

void normalKey(unsigned char key, int x, int y) {
  eng.normalKey(key, x, y);
}

void specialKey(int key, int x, int y) {
  eng.specialKey(key, x, y);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(400, 800);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Tetris");
  glClearColor(0.2, 0.5, 0.5, 1.0f);
  glOrtho(0, 8 * 10, 8 * 20, 0, 0.0, 0.1);
  glDisable(GL_DEPTH_TEST);
  glutDisplayFunc(display);
  glutKeyboardFunc(normalKey);
  glutSpecialFunc(specialKey);
  glutTimerFunc(eng.getSpeed(), onTimer, 0);
  glutMainLoop();
}