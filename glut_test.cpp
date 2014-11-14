//Brian Wichers (bwichers@ucsc.edu)
//Matthew Wang (majwang@ucsc.edu)
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <string>
#include <vector>
#include "hex.h"
using namespace std;

   int boardWidth = 11; //Size of board width
   graph connectivityMatrix = graph(boardWidth); //constructs connectivity matrix
   board hexBoard = board(boardWidth); //constructs HEX board

bool gameOver = false;
vector<posNode> board;

   void drawHex(double x, double y, double hexSize){
      glColor3f(0, 204, 204); // sets color to white.
      glBegin(GL_LINE_LOOP); 
               glVertex2f(x,         y+hexSize);   //vertex 1 (topleft)
               glVertex2f(x-hexSize, y+hexSize/2); //vertex 2 (midleft)
               glVertex2f(x-hexSize, y-hexSize/2); //vertex 3 (botleft)
               glVertex2f(x,         y-hexSize);   //vertex 4 (botright)
               glVertex2f(x+hexSize, y-hexSize/2); //vertex 5 (midright)
               glVertex2f(x+hexSize, y+hexSize/2); //vertex 6 (topright)
       glEnd();
   }

   void drawBoard(int size){
      double hexSize = .05;
      for(int i = 0; i < size; i++){
         double startX = -(hexSize*2*(size-1) + hexSize*(size-1))/2;
         startX+= i*hexSize;
         double startY = (size*hexSize*2 - size*hexSize + size*hexSize/2)/2;
         startY-= i*hexSize*2;
         startY+=i*hexSize/2;
         for(int j = 0; j < size; j++){
           drawHex(startX,startY, hexSize);
           startX+=hexSize*2;
         }
      }
   }

   void colorHex(double oldY, double oldX, char owner){
     double hexSize = .05;
     int size = 11;

     double startX = -(hexSize*2*(size-1) + hexSize*(size-1))/2;
     double startY = (size*hexSize*2 - size*hexSize + size*hexSize/2)/2;

     startX += hexSize*oldY;
     startX += 2*oldX*hexSize;

     startY -= 2*hexSize*oldY;
     startY += oldY*hexSize/2;

     double x = startX;
     double y = startY;

     if(owner == 'R' || owner == 'r') glColor3f(255, 0, 0); // sets color to red.
     else glColor3f(0, 0, 255); // sets color to blue.
        glBegin(GL_POLYGON);
               glVertex2f(x,         y+hexSize);   //vertex 1 (top)
               glVertex2f(x-hexSize, y+hexSize/2); //vertex 2 (topleft)
               glVertex2f(x-hexSize, y-hexSize/2); //vertex 3 (botleft)
               glVertex2f(x,         y-hexSize);   //vertex 4 (bot)
               glVertex2f(x+hexSize, y-hexSize/2); //vertex 5 (botright)
               glVertex2f(x+hexSize, y+hexSize/2); //vertex 6 (topright)
        glEnd();
   }

   void writeWords(){
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glRasterPos2f(0 , .5);

      glColor3f(0, 255, 0);
      
      for(int i = 1; i < 10; i++){
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, static_cast<char>(i + '0'));
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, ' ');
      }
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, '1');
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, '0');
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, ' ');
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, '1');
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, '1');

         glutSwapBuffers();

   }

void drawRects()
{
  glPushMatrix();  //No affect any other transformations in other code
     glTranslatef(-.88, .5, 0);  //Translate rectangle to its assigned x and y position
     glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
        glColor3f(1, 0, 0); //Set the color to red 
        glVertex2f(0, 0);            //Draw the four corners of the rectangle
        glVertex2f(0, .05);
        glVertex2f(1.75, .05);
        glVertex2f(1.75, 0);      
     glEnd();
  glPopMatrix();

  glPushMatrix();  //No affect any other transformations in other code
     glTranslatef(-.88, -.5, 0);  //Translate rectangle to its assigned x and y position
     glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
        glColor3f(1, 0, 0); //Set the color to red
        glVertex2f(0, 0);            //Draw the four corners of the rectangle
        glVertex2f(0, .05);
        glVertex2f(1.75, .05);
        glVertex2f(1.75, 0);
     glEnd();
  glPopMatrix();

  glPushMatrix();  //No affect any other transformations in other code
     glTranslatef(-.88, -.45, 0);  //Translate rectangle to its assigned x and y position
     glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
        glColor3f(0, 0, 1); //Set the color to blue
        glVertex2f(0, 0);            //Draw the four corners of the rectangle
        glVertex2f(0, .95);
        glVertex2f(.05, .95);
        glVertex2f(.05, 0);
     glEnd();
  glPopMatrix();

  glPushMatrix();  //No affect any other transformations in other code
     glTranslatef(.82, -.45, 0);  //Translate rectangle to its assigned x and y position
     glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
        glColor3f(0, 0, 1); //Set the color to blue
        glVertex2f(0, 0);            //Draw the four corners of the rectangle
        glVertex2f(0, .95);
        glVertex2f(.05, .95);
        glVertex2f(.05, 0);
     glEnd();
  glPopMatrix();

  glutSwapBuffers();
}

   void displayCall(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBoard(11);  
  
  int x, y; char owner;
  while(true){
     cout<<"Enter (row) [space] (column)"<<endl;
     cout<<"Ex: 1 11 or 11 5"<<endl;
     cin>>x>>y;
     if(x>0 && x<=11 && y>0 && y<=11 && hexBoard.getVector()[x-1][y-1].owner == '-'){
        owner = 'R'; 
        break;
     }
  }
  x--; y--; //used for translating input of 1-11 to data of 0-10
  hexBoard.placePiece(x, y, owner);
  posNode temp = posNode(x,y,owner);
  board.push_back(temp);

  for(auto element: board)
     colorHex(element.coordX, element.coordY, element.owner);

  drawBoard(11);
  drawRects();
 
  glutSwapBuffers();
  char winner = ' ';
  determineWinner(connectivityMatrix, hexBoard, boardWidth, winner, false);
  if(winner == 'R') exit(0);
}

static void idle(){
      drawRects();
      drawBoard(11);
      glutSwapBuffers();

      hexBoard.fillAIRemaining();
      posNode move = moveAI(connectivityMatrix, hexBoard);
      cout<<move.coordX<<", "<<move.coordY<<" " <<move.owner<<endl;
      hexBoard.placePiece(move.coordX, move.coordY, move.owner);
      board.push_back(move);
      char winner = ' ';
      determineWinner(connectivityMatrix, hexBoard, boardWidth, winner, false);
      if(winner == 'B') exit(0);

      for(auto element: board)
         colorHex(element.coordX, element.coordY, element.owner);
      drawBoard(11);
      drawRects();
      glutSwapBuffers();

      glutPostRedisplay();
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1000,1000); //Set the size you want
  glutCreateWindow("RED");
     glutDisplayFunc(displayCall);
     glutIdleFunc(idle);
     glutMainLoop();
  return 0;
}
