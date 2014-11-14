//Brian Wichers (bwichers@ucsc.edu)
//Matthew Wang (majwang@ucsc.edu)
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <climits>
#include <random>
#include <algorithm>
using namespace std;

static random_device rd;
static unsigned long seed = rd();
static mt19937 Mersenne(seed);

class posNode{
   public:
      int coordX;
      int coordY;
      char owner;
      posNode(){}
      posNode(int x, int y, char o):coordX(x),coordY(y),owner(o){}
};

class board{

   private:
      int size;
      vector<vector<posNode>> b1;
      vector<posNode> fill;

   public:
      vector<posNode> fillAI;

       void initialFillWithRemaining(){
         for(auto &row : b1)
            for(auto element : row)
               if (element.owner == '-')
                  fill.push_back(element);
      }

      void fillAIRemaining(){
         for(auto &row : b1)
            for(auto element : row)
               if (element.owner == '-')
                  fillAI.push_back(element);
      }

      friend ostream& operator<<(ostream& out, board& test);

      board (int s):size(s){
         b1.resize(s);
         for(auto &element : b1)
            element.resize(s);
         for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
               b1[i][j] = posNode(i, j, '-');
      }

      void fillBoard(){
         initialFillWithRemaining();
         shuffle(fill.begin(), fill.end(), Mersenne);
         int fillHalf = fill.size()/2;
         for(;fillHalf > 0; fillHalf--){
            int x = fill.back().coordX;
            int y = fill.back().coordY;
            fill.pop_back();
            b1[x][y].owner = 'R';
         }
         while(!fill.empty()){
            int x = fill.back().coordX;
            int y = fill.back().coordY;
            fill.pop_back();
            b1[x][y].owner = 'B';
         }
      }

      void placePiece(int x, int y, char owner){
         if(owner == 'R') b1[x][y].owner = 'R';
         else b1[x][y].owner = 'B';
      }

      vector<vector<posNode>>& getVector(){return b1;}
};

class graph{
   private:
         int size;
         vector<vector<int>> g1;

   public:
      friend ostream& operator<<(ostream& out, graph& test);

      graph (int s):size(s){
         g1.resize(s*s); 
         for(auto &element : g1)
            element.resize(s*s);

         int insert = 1;
         g1[0][1] = insert;
         g1[0][size] = insert;
         g1[size-1][size-2] = insert;
         g1[size-1][2*(size-1)] = insert;
         g1[size-1][2*(size-1)-1] = insert;

         for(int i = 1; i < size*size; ++i){
            if(i == size-1) continue; 
            int temp1 = (i+1)%(size);
            int temp2 = i%(size);

         if(temp1 != 0 && temp2 != 0 && i < (size*size - size)){
            g1[i][i-1] = insert;
            g1[i][i+1] = insert;
            g1[i][i+size-1] = insert;
            g1[i][i+size] = insert;
         }else if(temp1 == 0 && i < (size*size - size)){
            g1[i][i-1] = insert;
            g1[i][i-size] = insert;
            g1[i][i+size-1] = insert;
            g1[i][i+size] = insert;
         }else if(temp2 == 0 && i < (size*size - size)){ 
            g1[i][i-size] = insert;
            g1[i][i-size+1] = insert;
            g1[i][i+1] = insert;
            g1[i][i+size] = insert;
         }

         if(i > size && temp1 != 0 && temp2 != 0 && i < (size*size - size)){
            g1[i][i-size] = insert;
            g1[i][i-size+1] = insert;
         }

         if(temp1 != 0 && temp2 != 0 && i >= (size*size - size)){
            g1[i][i-1] = insert;
            g1[i][i+1] = insert;
            g1[i][i-size+1] = insert;
            g1[i][i-size] = insert;
         }else if(temp2 == 0 && i >= (size*size - size)){
            g1[i][i-size] = insert;
            g1[i][i-size+1] = insert;
            g1[i][i+1] = insert;
         }else if(temp1 == 0 && i >= (size*size - size)){
            g1[i][i-1] = insert;
            g1[i][i-size] = insert;
         }

      }
   }

   vector<vector<int>>& getVector(){return g1;}

   int getSize(){return size;}

};


int dijkstra(int vStart, int size, vector<vector<int>>& g1, 
                    vector<vector<posNode>>& b1, int flag);

ostream& operator<<(ostream& out, graph& test);

ostream& operator<<(ostream& out, board& test);

void determineWinner(graph& test1, board& board1, int bWidth, char& winner, 
                     bool AIdeciding);


posNode moveAI(graph & connectivityMatrix, board & hexBoard);

