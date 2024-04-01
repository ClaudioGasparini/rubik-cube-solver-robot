#ifndef CubeSolver_H
#define CubeSolver_H

#include <cstdio>
#include <string>

//global constants
#define top 0     //top face/side array index
#define bottom 1  //bottom side/face array index
#define front 2   //front side/face array index
#define back 3    //back side/face array index
#define right 4   //right side/face array index
#define left 5    //left side/face array index
#define none -1   //for any use
#define all -2    //for any use

using std::string;


class Cube { //single Servo class
  public:
    //class functions
    Cube();
    void define(char array[6][9]); //defines the cube with a 6 x 9 array
    void defineBlock(int face, int block, char color); //defines a single block on the cube 
    void solve(); //solves the cube
    void display(); //prints the cube on the serial monitor
    void turn(int face, int turn); //turns the cube 
    int getMoveNum(); //returns the number of moves to solve the cube
    string getMove(int num); //returns a specific move number
    void printCommands(); //converts moves to commands and prints them
};

extern "C" {
    Cube* Cube_new() { return new Cube(); }
    void Cube_define(Cube *cube, char array[6][9]) { cube->define(array); }
    void Cube_display(Cube *cube) { cube->display(); }
    void Cube_solve(Cube *cube) { cube->solve(); }
    int Cube_getMoveNum(Cube *cube) { return cube->getMoveNum(); }
    string Cube_getMove(Cube *cube, int num) { return cube->getMove(num); }
    void Cube_printCommands(Cube *cube) { cube->printCommands(); }
}

#endif
