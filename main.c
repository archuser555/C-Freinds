#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRAVITY 0.01
#define JUMP_VELOCITY 0.5
#define PLAYER_SPEED 0.1
#define BLOCK_SIZE 20
#define BLOCK_COUNT_X 40
#define BLOCK_COUNT_Y 20

// Struct for storing the player's position and velocity
typedef struct {
  float x;
  float y;
  float vx;
  float vy;
} Player;

// Struct for storing the position and size of a block
typedef struct {
  float x;
  float y;
  float width;
  float height;
} Block;

Player player;  // The player object
Block blocks[BLOCK_COUNT_X * BLOCK_COUNT_Y];  // Array of blocks

// Initialize the player and blocks
void init() {
  // Initialize player position and velocity
  player.x = 50;
  player.y = 50;
  player.vx = 0;
  player.vy = 0;

  // Initialize blocks
  for (int i = 0; i < BLOCK_COUNT_X; i++) {
    for (int j = 0; j < BLOCK_COUNT_Y; j++) {
      int index = i + j * BLOCK_COUNT_X;
      blocks[index].x = i * BLOCK_SIZE;
      blocks[index].y = j * BLOCK_SIZE;
      blocks[index].width = BLOCK_SIZE;
      blocks[index].height = BLOCK_SIZE;
    }
  }
}

// Draw a block at the given position and size
void drawBlock(float x, float y, float width, float height) {
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();
}

// Draw the player at the given position
void drawPlayer(float x, float y) {
  glBegin(GL_QUADS);
  glVertex2f(x - 10, y - 10);
  glVertex2f(x + 10, y - 10);
  glVertex2f(x + 10, y + 10);
  glVertex2f(x - 10, y + 10);
  glEnd();
}

// Check if the player is colliding with a block
int checkCollision(float x, float y, Block block) {
  if (x + 10 > block.x && x - 10 < block.x + block.width &&
      y + 10 > block.y && y - 10 < block.y + block.height) {
    return 1;
  }
  return 0;
}

// Update the player's position and velocity based on user input
void updatePlayer() {
  // Check for left and right arrow keys
  if (glutKeyIsDown('a')) {
    player.vx = -PLAYER_SPEED;
  } else if (glutKeyIsDown('d')) {
    player.vx = PLAYER_SPEED;
  } else {
    player.vx = 0;
  }

  // Check for space key (jump)
  if (glutKeyIsDown(' ') && player.vy == 0) {
    player.vy = JUMP_VELOCITY;
  }

  // Apply gravity to the player's vertical velocity
  player.vy -= GRAVITY;

  // Update the player's position based on velocity
  player.x += player.vx;
  player.y += player.vy;

  // Check for collisions with blocks
  for (int i = 0; i < BLOCK_COUNT_X * BLOCK_COUNT_Y; i++) {
    Block block = blocks[i];
    if (checkCollision(player.x, player.y, block)) {
      // Reset player's position to the top of the block
      player.y = block.y + block.height + 10;
      player.vy = 0;
      break;  // No need to check the other blocks
    }
  }
}

// Display callback function
void display() {
  glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

  // Draw the blocks
  for (int i = 0; i < BLOCK_COUNT_X * BLOCK_COUNT_Y; i++) {
    Block block = blocks[i];
    drawBlock(block.x, block.y, block.width, block.height);
  }

  // Draw the player
  drawPlayer(player.x, player.y);

  glFlush();  // Flush the pipeline
}

// Timer callback function
void timer(int value) {
  // Update the player
  updatePlayer();

  // Redisplay the scene
  glutPostRedisplay();

  // Set the timer for the next frame
  glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Platformer");

  // Initialize the player and blocks
  init();

  // Set the display and timer callback functions
  glutDisplayFunc(display);
  glutTimerFunc(16, timer, 0);

  // Set the background color to white
  glClearColor(1.0, 1.0, 1.0, 1.0);
  
    // Set the viewport and projection
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

  // Enter the main loop
  glutMainLoop();

  return 0;
}
