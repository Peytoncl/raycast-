#include <gl/gl.h>
#include <GL/glut.h>

#include <stdio.h>
#include "stdbool.h"
#include <stdlib.h> 
#include <math.h>

#include "windows.h"

#include "player.h"
#include "graphics.h"

#define M_PI 3.14159265358979323846

#define KEY_COUNT 256 

bool keys[KEY_COUNT];

float CAMERA_DEPTH = 8;
float CAMERA_FOV = 120;

float WALKING_FOV = 120;
float RUNNING_FOV = 135;

float CAMERA_PITCH = 0;

float CENTER_X = WINDOW_W / 2;
float CENTER_Y = WINDOW_H / 2;

#define ROTATION_ANGLE 0.025

#define PLAYER_SPAWN {5, 5}
#define PLAYER_SPEED 0.00025

Color backgroundColor = {30, 30, 30};

float planeX;
float planeY;

Player player = 
{
    .position = PLAYER_SPAWN,
    .angle = {0, 0}   
};

void castRays() 
{
  for (int x = 0; x <= VIEWPORT_WIDTH; x++) 
  {
    float cameraX = 2 * x / (float) VIEWPORT_WIDTH - 1; 

    float rayDirX = player.angle.x + (planeX * cameraX), 
    rayDirY = player.angle.y + (planeY * cameraX);
    
    int currentTileX = (int) player.position.x,
        currentTileY = (int) player.position.y;

    float sideDistX,
          sideDistY; 

    float deltaDistX = fabsf(1.f / rayDirX), 
          deltaDistY = fabsf(1.f / rayDirY), 
          perpWallDist; 
    
    int stepX;
    int stepY;

    int hit = 0; 
    int side;

    if (rayDirX < 0) 
    {
      stepX = -1;
      sideDistX = (player.position.x - currentTileX) * deltaDistX;
    } 
    else 
    {
      stepX = 1;
      sideDistX = (currentTileX + 1.0 - player.position.x) * deltaDistX;
    }

    if (rayDirY < 0) 
    {
      stepY = -1;
      sideDistY = (player.position.y - currentTileY) * deltaDistY;
    } 
    else 
    {
      stepY = 1;
      sideDistY = (currentTileY + 1.0 - player.position.y) * deltaDistY;
    }

    while (!hit) 
    {
      if (sideDistX < sideDistY) 
      {

        sideDistX += deltaDistX;
        currentTileX += stepX;

        side = 0;
      } 
      else 
      {
        sideDistY += deltaDistY;
        currentTileY += stepY;
        side = 1;
      }

      if (map[(currentTileY * MAP_W) + currentTileX] > 0) hit = 1;
    }

    if (!side) 
    {
      perpWallDist = (currentTileX - player.position.x  + (1 - stepX) / 2) / rayDirX;
    } 
    else 
    {
      perpWallDist = (currentTileY - player.position.y  + (1 - stepY) / 2) / rayDirY;
    }


    float cameraPlaneMagnitude = sqrt((planeX*planeX) + (planeY*planeY));
    float scaleModifier = 0.8; 
    int lineHeight = (int)(scaleModifier*(VIEWPORT_HEIGHT / perpWallDist / cameraPlaneMagnitude));

    int lineStart = (-(lineHeight / 2) + (VIEWPORT_HEIGHT / 2)) + CAMERA_PITCH;
    if (lineStart < 0) lineStart = 0;
    int lineEnd = ((lineHeight / 2) + (VIEWPORT_HEIGHT / 2)) + CAMERA_PITCH;
    if (lineEnd >= VIEWPORT_HEIGHT) lineEnd = VIEWPORT_HEIGHT - 1;

    unsigned char red, green, blue; 
    glBegin(GL_LINES);

    switch (map[(currentTileY * MAP_W) + currentTileX]) 
    {

        case 1:
            red = 105;
            green = 155;
            blue = 102;
        break;
        case 2:
            red = 151;
            green = 204;
            blue = 199;
        break;
        default:
            red = 137;
            green = 138;
            blue = 107;
        break;
    }

    if (!side) 
    {
      red -= 50;
      green -= 50;
      blue -= 50;
    }

    glBegin(GL_LINES);
      glColor3ub(red, green, blue);
      glVertex2i(x, lineStart);
      glVertex2i(x, lineEnd);
    glEnd();
  }
}

void init()
{
  glClearColor(0.3, 0.3, 0.3, 0);

  gluOrtho2D(0, WINDOW_W, WINDOW_H, 0);

  float fovRadians = (CAMERA_FOV * (M_PI / 180));

  player.angle.x = 1;
  player.angle.y = 0;

  planeX = 0;
  planeY = tan(fovRadians/2);
}

void display() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  castRays();

  glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) 
{
  keys[key] = true; 

  if (keys['w'])
  {

  }
}

void keyUp(unsigned char key, int x, int y) 
{
  keys[key] = false; 

  if (!keys['w'])
  {

  }
}

Angle oldAngle = {0, 0};
Position oldPosition = PLAYER_SPAWN;
float oldPitch = 0;

void update()
{
  //Change input system so i can get special keys like shift and escape
  
  if (keys['w'])
  {
    player.position.x += (player.angle.x * PLAYER_SPEED);
    player.position.y += (player.angle.y * PLAYER_SPEED);
  }
  if (keys['s']) 
  {
    player.position.x -= (player.angle.x * PLAYER_SPEED);
    player.position.y -= (player.angle.y * PLAYER_SPEED);
  }
  if (keys['a']) {
    player.position.x += (player.angle.y * PLAYER_SPEED);
    player.position.y -= (player.angle.x * PLAYER_SPEED);
  }
  if (keys['d']) {
    player.position.x -= (player.angle.y * PLAYER_SPEED);
    player.position.y += (player.angle.x * PLAYER_SPEED);
  }

  if (oldPitch != CAMERA_PITCH || oldAngle.x != player.angle.x || oldAngle.y != player.angle.y || oldPosition.x != player.position.x || oldPosition.y != player.position.y)
  {
    oldPosition = player.position;
    oldAngle = player.angle;
    oldPitch = CAMERA_PITCH;

    glutPostRedisplay();
  }
}

int lastX = 0;

int mouseLocked = 1;

void mouseMotion(int x, int y) 
{
  if (mouseLocked)
  {
    int deltaX = x - CENTER_X;
    int deltaY = y - CENTER_Y;

    if (deltaX < 0) 
    {
      float prevPlayerDirX = player.angle.x;

      player.angle.x = player.angle.x * cos(-ROTATION_ANGLE) - player.angle.y * sin(-ROTATION_ANGLE);
      player.angle.y = prevPlayerDirX * sin(-ROTATION_ANGLE) + player.angle.y * cos(-ROTATION_ANGLE);

      float prevPlaneX = planeX;

      planeX = planeX * cos(-ROTATION_ANGLE) - planeY * sin(-ROTATION_ANGLE);
      planeY = prevPlaneX * sin(-ROTATION_ANGLE) + planeY * cos(-ROTATION_ANGLE);
    } 
    else if (deltaX > 0) 
    {
      float prevPlayerDirX = player.angle.x;

      player.angle.x = player.angle.x * cos(ROTATION_ANGLE) - player.angle.y * sin(ROTATION_ANGLE);
      player.angle.y = prevPlayerDirX * sin(ROTATION_ANGLE) + player.angle.y * cos(ROTATION_ANGLE);

      float prevPlaneX = planeX;

      planeX = planeX * cos(ROTATION_ANGLE) - planeY * sin(ROTATION_ANGLE);
      planeY = prevPlaneX * sin(ROTATION_ANGLE) + planeY * cos(ROTATION_ANGLE);
    }

    if (deltaY < 0)
    {
      CAMERA_PITCH += 1 * ROTATION_ANGLE * 300;

      if (CAMERA_PITCH > 600) CAMERA_PITCH = 600;
    }
    else if (deltaY > 0)
    {
      CAMERA_PITCH -= 1 * ROTATION_ANGLE * 300;

      if (CAMERA_PITCH < -600) CAMERA_PITCH = -600;
    }

    glutWarpPointer(CENTER_X, CENTER_Y);
  }
}

void windowEntry(int state) 
{
  if (state == GLUT_LEFT) 
  {
    glutSetCursor(GLUT_CURSOR_INHERIT);
    mouseLocked = 0; 
  } 
  else if (state == GLUT_ENTERED) 
  {
    glutSetCursor(GLUT_CURSOR_NONE); 
    glutWarpPointer(CENTER_X, CENTER_Y);
    mouseLocked = 1;
  }
}

void windowResize(int width, int height)
{
  glutReshapeWindow(WINDOW_W, WINDOW_H);
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);    

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);        
    glutInitWindowSize(WINDOW_W, WINDOW_H);        
    
    glutCreateWindow("raycasts");  

    init();

    for (int i = 0; i < KEY_COUNT; i++) 
    {
      keys[i] = false;
    }

    glutDisplayFunc(display); 
    glutIdleFunc(update);      

    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyDown);  
    glutKeyboardUpFunc(keyUp);  

    glutReshapeFunc(windowResize);
    glutEntryFunc(windowEntry);

    glutSetCursor(GLUT_CURSOR_NONE);

    lastX = CENTER_X;
    glutWarpPointer(CENTER_X, CENTER_Y);

    glutMainLoop();      
    return 0;   
}