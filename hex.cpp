//Brian Wichers (bwichers@ucsc.edu)
//Matthew Wang (majwang@ucsc.edu)
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <climits>
#include <random>
#include <algorithm>
#include "hex.h"

using namespace std;

   //used to check to see if there are any elements in the open set remaining
   bool isEmptyFringe(vector<int>& test){
      bool answer = true;
      for (auto element : test){
         if (element == 1) return false;
      }
      return answer;
   }
      
   //used to send elements of "RED" and '-' to closed set
   void markTakenSpacesForBlue(vector<vector<posNode>>& b1, int size, vector<int>& status){
      for(int i = 0; i < size; ++i)
         for(int j = 0; j < size; ++j){
            if(b1[i][j].owner == 'R' || b1[i][j].owner == '-'){
               status[i*size + j] = 2;
         }
      }       
   }   

   //used to send elements of "BLUE" and '-' to closed set
   void markTakenSpacesForRed(vector<vector<posNode>>& b1, int size, vector<int>& status){
      for(int i = 0; i < size; ++i)
         for(int j = 0; j < size; ++j){
            if(b1[i][j].owner == 'B' || b1[i][j].owner == '-'){
               status[i*size + j] = 2;
         }
      }
   }


   int dijkstra(int vStart, int size, 
                    vector<vector<int>>& g1, vector<vector<posNode>>& b1, int flag){
      vector<int> status1(size*size); //essentially determines open/closed elements

      if (flag == 0)
         markTakenSpacesForBlue(b1, size, status1); //Red and '-' occupied
      else
          markTakenSpacesForRed(b1, size, status1); //Blue and '-' occupied
      size *= size; //Sets size to size^2 in order to work through matrix

      if(status1[vStart] == 2) return 0;
      
      int selected1 = vStart; //the chosen element from open to go to closed set
   
      status1[vStart] = 3; //send front vertex to closed set
      
      do{
       //update the fringe and the status of all elements entering the fringe
         for (int cols = 0; cols < size; ++cols){
            if(status1[cols] == 0 && g1[selected1][cols] > 0) {
                  status1[cols] = 1;
            }
         }
         //if no open set elements break out
         if(isEmptyFringe(status1)) break;
 
         //select the next open set element
         for(int index = 0; index < size; index++){
            if(status1[index] == 1){
               selected1 = index; break;}
         }

         status1[selected1] = 3; //and send it to the closed set
         //if we have reached the opposing end vertex finished unidirectional search
      }while(true);

      if(flag == 0){
         for(int i = 10; i < size; i+=11){
            if(status1[i] == 3) return 1;
         }
      }
      else{
         for(int i = 120; i > 109; i--){
            if (status1[i] == 3) return 1;
         }
      }

      return 0;
   }

 //Used to print hex board
   ostream& operator<<(ostream& out, board& test){
      out<<"\nboard size "<<test.size<<endl;
      int spaces = 0;
      for (auto row : test.b1){   //to go through the the rows of the matrix
         spaces++;
         for (auto element : row) //print every element in row
            out << element.owner << " ";
         out<<"\n";
         for(int i = 0; i < spaces; ++i)
            out<<" ";
      }
      return out;
   }

 //Used to determine winner of HEX and print to screen
   void determineWinner(graph& test1, board& board1, int bWidth, char & winner, 
                        bool AIdeciding){
      int redWinner = 0;  //determine if there is a winning distance from left to right
      int blueWinner = 0; //determine in there is a winning distance from top to bottom
      int topRow = bWidth;    //used to iterate through top row of board
      int posTopRow = 0;                  //keep track of top row position
      int posLeftCol = 0;

    //while there is no path from current top to bottom position and there are more top
    //  positions to check against the bottom row
      while(blueWinner == 0 && redWinner == 0 && topRow > 0){ 
       //while there is no path from current top to bottom position and there are more
       //  bottom positions to check against the current top position
          //quick dijkstra's to determine if there is a winning path
            redWinner = dijkstra(posTopRow, 
                     test1.getSize(), test1.getVector(), board1.getVector(), 1);
            blueWinner = dijkstra(posLeftCol,
                     test1.getSize(), test1.getVector(), board1.getVector(), 0);
         topRow--;
         posTopRow++;
         posLeftCol+=bWidth;
      }

      if(!AIdeciding){
         if(blueWinner != 0){ 
            winner = 'B';
            cout<<"WINNER IS BLUE!"<<endl;
         }
         else if(redWinner != 0){
            winner = 'R';
            cout << "WINNER IS RED!"<<endl;
         }
      }
      else{
         if(blueWinner != 0)
            winner = 'B';
         else if(redWinner != 0)
            winner = 'R';
      }

   }

   posNode moveAI(graph & connectivityMatrix, board & hexBoard){
      int maxNumWins = 0;
      posNode bestNode;
      board testBoard = hexBoard; 
      while(!hexBoard.fillAI.empty()){
         int MonteCarlo = 1000;
         int currentWins = 0;
         posNode temp = hexBoard.fillAI.back();
         testBoard.placePiece(temp.coordX, temp.coordY, 'B');
         for(; MonteCarlo!=0; MonteCarlo--){
            testBoard.fillBoard();
            char winner = ' ';
            determineWinner(connectivityMatrix, testBoard, 11, winner, true);
            if(winner == 'B') currentWins++;
            testBoard = hexBoard;
            testBoard.placePiece(temp.coordX, temp.coordY, 'B');
         }
         if(currentWins > maxNumWins) {
            maxNumWins = currentWins;
            bestNode = temp;
            cout<<maxNumWins<<endl;
         }
         hexBoard.fillAI.pop_back();
      }
      bestNode.owner = 'B';
      return bestNode;
   }
