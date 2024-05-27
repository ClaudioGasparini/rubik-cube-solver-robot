#include "CubeSolver.h"
//**GLOBAL VARIABLES************************************************************************
char cube[6][9];                                                               //rubix cube matrix (6x9 matrix = 6 faces with 9 blocks per face)
char copy[6][9];                                                               //matrix copy of the cube
char face_color[6] = { 'R', 'O', 'W', 'Y', 'G', 'B' };                         //the color of each face (correlates to the index of each face)
char pair[4][2] = { { 'R', 'G' }, { 'G', 'O' }, { 'B', 'R' }, { 'O', 'B' } };  //edge and corner color pairs for F2L
string move[100];                                                              //the catalog of all the moves that the cube should make
int move_num = 0;                                                              //counter for the number of moves
int start = 0;                                                                 //the starting index for printing the moves of the current step

//**FUNCTIONS***************************************************************************************
//turns a specified side of the cube num number of times by rearranging the matrix
void turnSide(int side, int turn) {
  //defines the block number (of the old cube) that will be in the place of the corresponding index after one right turn
  //this is the same assignment for all sides
  //for example: the new block[0] is the old block[2]
  int adoptA[9] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };  //adoption assignments for the main face
  //adoption assignments for the 4 sides affected by a turn
  //each level is for each side. Each level has 4 sub arrays with the name of the face and the blocks affected by the turn
  int adoptB[6][4][4] = { { { left, 6, 3, 0 }, { back, 6, 3, 0 }, { right, 6, 3, 0 }, { front, 6, 3, 0 } },    //top
                          { { left, 2, 5, 8 }, { front, 2, 5, 8 }, { right, 2, 5, 8 }, { back, 2, 5, 8 } },    //bottom
                          { { left, 8, 7, 6 }, { top, 2, 5, 8 }, { right, 0, 1, 2 }, { bottom, 6, 3, 0 } },    //front
                          { { right, 8, 7, 6 }, { top, 6, 3, 0 }, { left, 0, 1, 2 }, { bottom, 2, 5, 8 } },    //back
                          { { front, 8, 7, 6 }, { top, 8, 7, 6 }, { back, 0, 1, 2 }, { bottom, 8, 7, 6 } },    //right
                          { { back, 8, 7, 6 }, { top, 0, 1, 2 }, { front, 0, 1, 2 }, { bottom, 0, 1, 2 } } };  //left

  char copy2[6][9];                  //copy of the cube
  int faceO, blockO, faceN, blockN;  //old face and block as well as new face and block
  //array of the abbreviations of each move
  //directly correlate to face indexes (top, bottom, etc...)
  string move_name[6] = { "U", "D", "F", "B", "R", "L" };

  //rotates the side 'turn' number of times
  for (int i = 0; i < turn; i++) {
    //creates a copy of the cube
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 9; j++) {
        copy2[i][j] = cube[i][j];
      }
    }
    //rotates the primary face (aka the side being turned)
    for (int i = 0; i < 9; i++) {
      cube[side][i] = copy2[side][adoptA[i]];
    }
    //rotates the secondary faces faces (the other 4 faces partially affected by the turn)
    for (int k = 0; k < 4; k++) {
      for (int p = 1; p < 4; p++) {
        faceN = adoptB[side][k][0];   //defines the new face
        blockN = adoptB[side][k][p];  //defines the new block
        //when k is 3 there is a special case for defining the old face and block
        if (k == 3) {
          faceO = adoptB[side][0][0];   //defines the old face
          blockO = adoptB[side][0][p];  //defines the old block
        }
        //when k is not 3
        else {
          faceO = adoptB[side][k + 1][0];   //defines the old face
          blockO = adoptB[side][k + 1][p];  //defines the old block
        }
        cube[faceO][blockO] = copy2[faceN][blockN];  //replaces the old with the new
      }
    }
  }
  if (turn != 0) {  //if more than 0 turns are made
    //catalogs the move(s) that was/were just made
    switch (turn) {
      case 1:  //if 1 turn is made
        move[move_num] = move_name[side];
        break;
      case 2:  //if 2 turns are made
        move[move_num] = "2" + move_name[side];
        break;
      case 3:  //if 3 turns are made
        move[move_num] = "'" + move_name[side] + "'";
        break;
    }
    move_num++;  //increments the number of moves
  }
}
//prints all or some of the the moves
//some of the moves == the moves from the last step
void printMoves(string amount) {
  int i = 0;
  if (amount == "ALL") {
    i = 0;
  } else if (amount == "STEP") {
    i = start;
  }
  for (int j = i; j < move_num; j++) {
    printf("%s\t", move[j].c_str());
  }
  printf("\n");
}
//prints a matrix of the input face of the cube
void printCube(int face) {
  if (face == all) {
    string name[6] = { "TOP", "BOTTOM", "FRONT", "BACK", "RIGHT", "LEFT" };  //the names of each face of the cube
    /*for (int i = 0; i < 6; i = i + 3) {
      printf("%s\t\t\t\t", name[i].c_str());  //prints the name of the face
      printf("%s\t\t\t\t", name[i+1].c_str());
      printf("%s\t\t\t\t", name[i+2].c_str());
      for (int j = 0; j < 3; j++) {
        printf("%c ", 		cube[i][j]);
        printf("%c ", 		cube[i][j + 3]);
        printf("%c  ", 	cube[i][j + 6]);
        printf("%c ", 		cube[i + 1][j]);
        printf("%c ", 		cube[i + 1][j + 3]);
        printf("%c ", 	cube[i + 1][j + 6]);
        printf("%c ", 		cube[i + 2][j]);
        printf("%c ", 		cube[i + 2][j + 3]);
        printf("%c ", 		cube[i + 2][j + 6]);
      }
      
    }*/
      for (int i = 0; i < 6; i++) {
      printf("%s\t\t\t", name[i].c_str());  //prints the name of the face
      for (int j = 0; j < 9; j++) {
        printf("%c ", cube[i][j]);
      }
      printf("\n");
      
    }
  }
  //prints an inividual face
  else {
    for (int i = 0; i < 3; i++) {
      printf("%c\t", cube[face][i]);
      printf("%c\t", cube[face][i + 3]);
      printf("%c\t", cube[face][i + 6]);
    }
  }
}
//makes a copy of the current cube config
void copyCube() {
  //creates a copy of the cube
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 9; j++) {
      copy[i][j] = cube[i][j];
    }
  }
}
//simplifies the moves by combines like moves that are next to each other
void simplifyMoves() {
  string move_new[move_num], carry;
  int count, skip;
  //simplifies the moves
  //runs 5 times
  for (int i = 0; i < 5; i++) {
    //checks the move directly next and simplifies
    //runs 5 times
    for (int i = 0; i < 5; i++) {
      count = 0;
      skip = 0;
      //checks the moves directly next
      for (int i = 0; i < move_num - 1; i++) {
        //if this move shouldnt be skipped
        if (skip == 0) {
          //if this move and the next are the same and single charcter
          if (move[i + 1] == move[i] && move[i].length() == 1) {
            move_new[count] = string("2") + move[i];
            count++;
            skip = 1;
          }
          //if this move and the next are the same and 3 characters
          else if (move[i + 1] == move[i] && move[i].length() == 3) {
            if (move[i] == "'U'") {
              move_new[count] = "2U";
            }
            if (move[i] == "'D'") {
              move_new[count] = "2D";
            }
            if (move[i] == "'F'") {
              move_new[count] = "2F";
            }
            if (move[i] == "'B'") {
              move_new[count] = "2B";
            }
            if (move[i] == "'R'") {
              move_new[count] = "2R";
            }
            if (move[i] == "'L'") {
              move_new[count] = "2L";
            }
            count++;
            skip = 1;
          }
          //if this move and the next are the same and double charcter
          else if (move[i + 1] == move[i] && move[i].length() == 2) {
            skip = 1;
          }
          //if the next move is double this move
          else if (move[i + 1] == "2" + move[i]) {
            move_new[count] = "'" + move[i] + "'";
            count++;
            skip = 1;
          }
          //if the next move is half this move
          else if (move[i] == "2" + move[i + 1]) {
            move_new[count] = "'" + move[i + 1] + "'";
            count++;
            skip = 1;
          }
          //if the next move is in the opposite direction of this move they cancel
          else if (move[i + 1] == "'" + move[i] + "'" || move[i] == "'" + move[i + 1] + "'") {
            skip = 1;
          }
          //example: 2U + 'U' (defined explicitly)
          else if (move[i] == "2U" && move[i + 1] == "'U'" || move[i + 1] == "2U" && move[i] == "'U'") {
            move_new[count] = "U";
            skip = 1;
            count++;
          } else if (move[i] == "2D" && move[i + 1] == "'D'" || move[i + 1] == "2D" && move[i] == "'D'") {
            move_new[count] = "D";
            skip = 1;
            count++;
          } else if (move[i] == "2F" && move[i + 1] == "'F'" || move[i + 1] == "2F" && move[i] == "'F'") {
            move_new[count] = "F";
            skip = 1;
            count++;
          } else if (move[i] == "2B" && move[i + 1] == "'B'" || move[i + 1] == "2B" && move[i] == "'B'") {
            move_new[count] = "B";
            skip = 1;
            count++;
          } else if (move[i] == "2R" && move[i + 1] == "'R'" || move[i + 1] == "2R" && move[i] == "'R'") {
            move_new[count] = "R";
            skip = 1;
            count++;
          } else if (move[i] == "2L" && move[i + 1] == "'L'" || move[i + 1] == "2L" && move[i] == "'L'") {
            move_new[count] = "L";
            skip = 1;
            count++;
          }
          //else just count the move
          else {
            move_new[count] = move[i];
            count++;
          }
        }
        //if this move should be skipped
        else if (skip == 1) {
          skip = 0;  //reset skip
        }
      }
      //gets the last move
      if (skip == 0 && (move_num > 0)) {
        move_new[count] = move[move_num - 1];
        count++;
      }
      //saves new moves to the move array
      for (int i = 0; i < count; i++) {
        move[i] = move_new[i];
      }
      move_num = count;
    }
    //checks if the next move doesnt affect this move
    //if so, switch their placement
    for (int i = 0; i < move_num - 2; i++) {
      if (move[i] == "U" || move[i] == "'U'" || move[i] == "2U") {
        if (move[i + 1] == "D" || move[i + 1] == "'D'" || move[i + 1] == "2D") {
          if (move[i + 2] == "U" || move[i + 2] == "'U'" || move[i + 2] == "2U") {
            carry = move[i + 1];
            move[i + 1] = move[i + 2];
            move[i + 2] = carry;
          }
        }
      }
      if (move[i] == "D" || move[i] == "'D'" || move[i] == "2D") {
        if (move[i + 1] == "U" || move[i + 1] == "'U'" || move[i + 1] == "2U") {
          if (move[i + 2] == "D" || move[i + 2] == "'D'" || move[i + 2] == "2D") {
            carry = move[i + 1];
            move[i + 1] = move[i + 2];
            move[i + 2] = carry;
          }
        }
      }
      if (move[i] == "F" || move[i] == "'F'" || move[i] == "2F") {
        if (move[i + 1] == "B" || move[i + 1] == "'B'" || move[i + 1] == "2B") {
          if (move[i + 2] == "F" || move[i + 2] == "'F'" || move[i + 2] == "2F") {
            carry = move[i + 1];
            move[i + 1] = move[i + 2];
            move[i + 2] = carry;
          }
        }
      }
      if (move[i] == "B" || move[i] == "'B'" || move[i] == "2B") {
        if (move[i + 1] == "F" || move[i + 1] == "'F'" || move[i + 1] == "2F") {
          if (move[i + 2] == "B" || move[i + 2] == "'B'" || move[i + 2] == "2B") {
            carry = move[i + 1];
            move[i + 1] = move[i + 2];
            move[i + 2] = carry;
          }
        }
      }
      if (move[i] == "R" || move[i] == "'R'" || move[i] == "2R") {
        if (move[i + 1] == "L" || move[i + 1] == "'L'" || move[i + 1] == "2L") {
          if (move[i + 2] == "R" || move[i + 2] == "'R'" || move[i + 2] == "2R") {
            carry = move[i + 1];
            move[i + 1] = move[i + 2];
            move[i + 2] = carry;
          }
        }
      }
      if (move[i] == "L" || move[i] == "'L'" || move[i] == "2L") {
        if (move[i + 1] == "R" || move[i + 1] == "'R'" || move[i + 1] == "2R") {
          if (move[i + 2] == "L" || move[i + 2] == "'L'" || move[i + 2] == "2L") {
            carry = move[i + 1];
            move[i + 1] = move[i + 2];
            move[i + 2] = carry;
          }
        }
      }
    }
  }
}
//resets all the moves from the current step
//resets the cube to how it was before the current step
void resetStep() {
  //resets moves
  for (int i = start; i < move_num; i++) {
    move[i] = "0";
  }
  move_num = start;  //resets move number to start
  //resets cube
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 9; j++) {
      cube[i][j] = copy[i][j];
    }
  }
}
//completes a trigger move
//enter the face and the side on the face to perform the trigger (left or right: 0 or 1)
void pullTrigger(int face, int side) {
  //0 = face that is L, 1 = face that is R for each side
  //no triggers can be done for front or back
  int handle[6][2] = { { left, right }, { right, left }, { none, none }, { none, none }, { top, bottom }, { bottom, top } };
  //left trigger
  if (side == 0) {
    turnSide(handle[face][0], 3);
    turnSide(back, 3);
    turnSide(handle[face][0], 1);
  }
  //right trigger
  else if (side == 1) {
    turnSide(handle[face][1], 1);
    turnSide(back, 1);
    turnSide(handle[face][1], 3);
  }
}
//checks a corner to see if it has the colors the user is looking for
int cornerCheck(int layer, int cnum, int pnum) {
  //checks the top layer
  if (layer == top) {
    //locations of the 3 blocks that make each corner on the  top layer
    int corner[4][6] = { { top, 6, right, 6, back, 0 },
                         { bottom, 8, right, 8, back, 2 },
                         { top, 0, left, 0, back, 6 },
                         { bottom, 2, left, 2, back, 8 } };

    char block1 = cube[corner[cnum][0]][corner[cnum][1]];
    char block2 = cube[corner[cnum][2]][corner[cnum][3]];
    char block3 = cube[corner[cnum][4]][corner[cnum][5]];
    char color1 = pair[pnum][0];
    char color2 = pair[pnum][1];
    int num = 0;

    //checks if one of the blocks is white
    if (block1 == 'W' || block2 == 'W' || block3 == 'W') {
      num++;
    }
    //checks if one of the blocks is color1
    if (block1 == color1 || block2 == color1 || block3 == color1) {
      num++;
    }
    //checks if one of the blocks is color2
    if (block1 == color2 || block2 == color2 || block3 == color2) {
      num++;
    }

    return (num / 3);  //if a corner exists 1 will be returned, else 0 will be returned
  }
  //checks the bottom layer
  else if (layer == bottom) {
    //locations of the 3 blocks that make each corner on the  bottom layer
    int corner[4][6] = { { top, 2, left, 6, front, 0 },
                         { bottom, 0, left, 8, front, 2 },
                         { top, 8, right, 0, front, 6 },
                         { bottom, 6, right, 2, front, 8 } };
    //the trigger that is pulled depending on which corner needs to move (all are right triggers)
    int trigger[4] = { left, bottom, top, right };
    char block1 = cube[corner[cnum][0]][corner[cnum][1]];
    char block2 = cube[corner[cnum][2]][corner[cnum][3]];
    char block3 = cube[corner[cnum][4]][corner[cnum][5]];
    char color1 = pair[pnum][0];
    char color2 = pair[pnum][1];
    int num = 0;

    //checks if one of the blocks is white
    if (block1 == 'W' || block2 == 'W' || block3 == 'W') {
      num++;
    }
    //checks if one of the blocks is color1
    if (block1 == color1 || block2 == color1 || block3 == color1) {
      num++;
    }
    //checks if one of the blocks is color2
    if (block1 == color2 || block2 == color2 || block3 == color2) {
      num++;
    }
    if (num == 3) {
      pullTrigger(trigger[cnum], 1);
    }
    return (num / 3);  //if a corner exists, 1 will be returned, else 0 will be returned
  }
}
//checks an edge to see if it has the colors the user is looking for
int edgeCheck(int layer, int ednum, int pnum) {
  //top layer edge piece
  if (layer == top) {
    //locations of the 2 blocks that make each edge on the  top layer
    int edge[4][4] = { { right, 7, back, 1 },
                       { top, 3, back, 3 },
                       { bottom, 5, back, 5 },
                       { left, 1, back, 7 } };
    int block1 = cube[edge[ednum][0]][edge[ednum][1]];
    int block2 = cube[edge[ednum][2]][edge[ednum][3]];
    int color1 = pair[pnum][0];
    int color2 = pair[pnum][1];

    //checks if one of the blocks is color1 and the other is color2
    if (block1 == color1 && block2 == color2 || block2 == color1 && block1 == color2) {
      return 1;
    }
    //returns 0
    else {
      return 0;
    }
  }
  //bottom layer edge piece
  else if (layer == bottom) {
    //locations of the 2 blocks that make each edge on the bottom layer
    int edge[4][4] = { { top, 7, right, 3 },
                       { bottom, 7, right, 5 },
                       { top, 1, left, 3 },
                       { bottom, 1, left, 5 } };

    //the trigger that is pulled depending on which edge needs to move (all are right triggers)
    int trigger[4] = { top, right, left, bottom };
    int block1 = cube[edge[ednum][0]][edge[ednum][1]];
    int block2 = cube[edge[ednum][2]][edge[ednum][3]];
    int color1 = pair[pnum][0];
    int color2 = pair[pnum][1];

    //checks if one of the blocks is color1 and the other is color 2 and returns 1
    if (block1 == color1 && block2 == color2 || block2 == color1 && block1 == color2) {
      pullTrigger(trigger[ednum], 1);
      return 1;
    }
    //returns 0
    else {
      return 0;
    }
  }
}
//hides a corner and checks that a solved pair isn't being interupted
//also returns the corner to its original position
void hideCorner(int solved[4], int pnum, char type, int turn) {
  int face[4] = { right, bottom, top, left };      // this is R when performing a hide move
  int face_con[4] = { top, right, left, bottom };  // this is R when performing a hide move for a conflicting edge
  int con[4] = { 1, 3, 0, 2 };                     //each number is the edge pair that creates a conflict if pair i is being solved

  //if hide is type A and there is a solved pair in the way do a specicial hide
  if (type == 'A' && solved[con[pnum]] == 1) {
    turnSide(back, 1);
    turnSide(face_con[pnum], 3);
    turnSide(back, turn);
    turnSide(face_con[pnum], 1);
    turnSide(back, 3);
  }
  //hide A
  else if (type == 'A') {
    turnSide(face[pnum], 3);
    turnSide(back, turn);
    turnSide(face[pnum], 1);
  }
  //hide B
  else if (type == 'B') {
    turnSide(back, 3);
    turnSide(face[pnum], 1);
    turnSide(back, turn);
    turnSide(face[pnum], 3);
    turnSide(back, 1);
  }
}
//enter an edge number (of a top edge) and the color of the top of that edge is returned
char edgeColor(int edge) {
  switch (edge) {
    case 0:  //edge 0
      return cube[back][1];
      break;
    case 1:  //edge 1
      return cube[back][3];
      break;
    case 2:  //edge 2
      return cube[back][5];
      break;
    case 3:  //edge 3
      return cube[back][7];
      break;
  }
}
//all the different orders for 4 Pick 4 (there are 24 combinations)
//enter an order and a number and it returns the index
int order4P4(int order, int num) {
  int combo[24][4] = {
    { 0, 1, 2, 3 },
    { 3, 0, 1, 2 },
    { 2, 3, 0, 1 },
    { 1, 2, 3, 0 },

    { 0, 2, 3, 1 },
    { 1, 0, 2, 3 },
    { 3, 1, 0, 2 },
    { 2, 3, 1, 0 },

    { 0, 3, 2, 1 },
    { 1, 0, 3, 2 },
    { 2, 1, 0, 3 },
    { 3, 2, 1, 0 },

    { 0, 1, 3, 2 },
    { 2, 0, 1, 3 },
    { 3, 2, 0, 1 },
    { 1, 3, 2, 0 },

    { 0, 2, 1, 3 },
    { 3, 0, 2, 1 },
    { 1, 3, 0, 2 },
    { 2, 1, 3, 0 },

    { 0, 3, 1, 2 },
    { 2, 0, 3, 1 },
    { 1, 2, 0, 3 },
    { 3, 1, 2, 0 },
  };
  return combo[order][num];
}
//returns the index of the smallest value in an array of a defined size
int minAmount(int array[24], int size) {
  int index = 0;       //initial index value
  int min = array[0];  //initial minimum value
  for (int i = 1; i < size; i++) {
    //checks if the value is smaller than the minimum
    if (array[i] < min) {
      min = array[i];  //saves the min value
      index = i;       //saves the index
    }
  }
  return index;
}
//rotates the back face so that the color of a specified block matches the center color of a face
void matchCenter(int face, int block) {
  switch (face) {
    case top:
      switch (cube[face][block]) {
        case 'G':  //green (3 turn)
          turnSide(back, 3);
          break;
        case 'O':  //orange (2 turns)
          turnSide(back, 2);
          break;
        case 'B':  //blue (1 turns)
          turnSide(back, 1);
          break;
      }
      break;
    case bottom:
      switch (cube[face][block]) {
        case 'R':  //red (2 turns)
          turnSide(back, 2);
          break;
        case 'G':  //green (1 turn)
          turnSide(back, 1);
          break;
        case 'B':  //blue (3 turns)
          turnSide(back, 3);
          break;
      }
      break;
    case right:
      switch (cube[face][block]) {
        case 'R':  //red (1 turns)
          turnSide(back, 1);
          break;
        case 'O':  //orange (3 turns)
          turnSide(back, 3);
          break;
        case 'B':  //blue (2 turns)
          turnSide(back, 2);
          break;
      }
      break;
    case left:
      switch (cube[face][block]) {
        case 'R':  //red (3 turns)
          turnSide(back, 3);
          break;
        case 'G':  //green (2 turn)
          turnSide(back, 2);
          break;
        case 'O':  //blue (1 turns)
          turnSide(back, 1);
          break;
      }
      break;
  }
}
//ends a step by copying the cube, simplifying the steps and printing the outputs
void endStep(int num) {
  /*
  switch (num) {
    case 1:
      printf("STEP 1: WHITE CROSS COMPLETE!");
      break;
    case 2:
      printf("STEP 2: F2L pairs complete!");
      break;
    case 3:
      printf("STEP 2: OLL complete!");
      break;
    case 4:
      printf("STEP 3: PLL complete!");
      break;
  }
  */
  simplifyMoves();
  //printMoves("STEP");
  //printf((string) "(" + move_num + " MOVES)");  //prints moves
  copyCube();        //creates a copy of the cube to finalize the daisy
  start = move_num;  //defines the new starting position of the moves
}
//STEP 1: creates the white cross on the front face
void whiteCross() {
  //locations of both sided of the edge pieces on the bottom layer
  int edge[3][4][4] = { { { front, 1, left, 7 }, { front, 3, top, 5 }, { front, 5, bottom, 3 }, { front, 7, right, 1 } },  //bottom layer edges
                        { { left, 3, top, 1 }, { bottom, 1, left, 5 }, { top, 7, right, 3 }, { right, 5, bottom, 7 } },    //middle layer edges
                        { { back, 1, right, 7 }, { back, 3, top, 3 }, { back, 5, bottom, 5 }, { back, 7, left, 1 } } };    //top layer edges
  //the number of turns the back side must make in order to put an edge from the top of face [i] to the top of of position [j]
  int turns[6][4] = { { 0, 3, 2, 1 }, { 2, 1, 0, 3 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 0, 3, 2 }, { 3, 2, 1, 0 } };
  int pface[4] = { top, right, bottom, left };  //the correlating face to each edge position
  //these are all the sequential orders (of the colored edge pieces) that the white cross can be solved (4 combos aka 4C4) (going clockwise)
  char order[4][4] = { { 'R', 'G', 'O', 'B' },
                       { 'B', 'R', 'G', 'O' },
                       { 'O', 'B', 'R', 'G' },
                       { 'G', 'O', 'B', 'R' } };
  char side1, side2, ecolor;
  int face1, face2, block1, block2, face, epos, turn_front, ednum;
  int order_moves[4];  //the number of moved made by each order

  //runs 5 times, the last time redoes the order with the smallest number of moves
  //the first 4 times runs different orders and catalogs the number of moves
  for (int order_num = 0; order_num < 5; order_num++) {
    //cycles through the 4 edge positions on the front face
    //finds the next edge and puts it in the right place
    epos = 0;
    for (int i = 0; i < 4; i++) {
      //if runnign for the last time pick the index that has the least amount of moves
      if (order_num == 4) {
        ecolor = order[minAmount(order_moves, 4)][i];  //the color of the target edge
      }
      //if not running for the last time
      else {
        ecolor = order[order_num][i];  //the color of the target edge
      }
      //cycles through each edge on layer 0 (bottom layer) in search of the defined edge
      for (ednum = 0; ednum < 4; ednum++) {
        //defines the two sides of the current edge
        side1 = cube[edge[0][ednum][0]][edge[0][ednum][1]];
        side2 = cube[edge[0][ednum][2]][edge[0][ednum][3]];

        //if the edge in question is on the bottom, move it to the middle
        if (side1 == 'W' && side2 == ecolor || side2 == 'W' && side1 == ecolor) {
          turnSide(edge[0][ednum][2], 1);  //moves the edge to the middle layer
          //now its a middle layer problem
        }
      }
      //cycles through each edge on layer 2 (top layer) in search for the defined edge
      for (ednum = 0; ednum < 4; ednum++) {
        //defines the two sides of the current edge and face 2
        side1 = cube[edge[2][ednum][0]][edge[2][ednum][1]];
        side2 = cube[edge[2][ednum][2]][edge[2][ednum][3]];
        face2 = edge[2][ednum][2];

        //case 1: if side 1 is white
        if (side1 == 'W' && side2 == ecolor) {
          turnSide(back, turns[face2][epos]);  //turns the back to make the edge be above where it should be (on the front face)
          turnSide(pface[epos], 2);            //moves the edge to the bottom layer
          epos++;                              //increments epos so that the next edge is put in the next position
        }
        //case 2: if side 2 is white
        else if (side2 == 'W' && side1 == ecolor) {
          turnSide(back, turns[face2][epos]);  //turns the back to make the edge be above where it should be (on the front face)
          turnSide(pface[epos], 3);            //moves the edge to the middle layer
          //now that the edge is in the middle section it will move to the front in the next for loop
        }
      }
      //cycles through each edge on layer 1 (middle layer) in search for the defined edge
      for (ednum = 0; ednum < 4; ednum++) {
        //defines the two sides of the current edge and two faces of the current edge
        side1 = cube[edge[1][ednum][0]][edge[1][ednum][1]];
        side2 = cube[edge[1][ednum][2]][edge[1][ednum][3]];
        face1 = edge[1][ednum][0];
        face2 = edge[1][ednum][2];

        //case 1: if side 1 is white
        if (side1 == 'W' && side2 == ecolor) {
          //moves the front so that the edge can be dropped in
          turn_front = turns[face2][epos];
          turnSide(front, turns[face2][epos]);
          turnSide(face2, 3);                                     //drops edge
          if (turn_front != 3) { epos = epos + turn_front + 1; }  //changes epos since the front was turned
        }
        //case 2: if side 2 is white
        else if (side2 == 'W' && side1 == ecolor) {
          //defines the new turn amount to drop the edge in
          turn_front = turns[face2][epos] - 1;
          if (turn_front == -1) { turn_front = 3; }  //corrects a negative turn
          //moves the front so that the edge can be dropped in
          turnSide(front, turn_front);
          turnSide(face1, 1);                                     //drops edge
          if (turn_front != 3) { epos = epos + turn_front + 1; }  //changes epos since the front was turned
        }
      }
      if (epos > 3) { epos -= 4; }  //if epos is greater than 3, correct it
    }
    //matches each edge with its center
    switch (cube[top][5]) {
      case 'G':
        turnSide(front, 1);
        break;
      case 'O':
        turnSide(front, 2);
        break;
      case 'B':
        turnSide(front, 3);
        break;
    }
    simplifyMoves();  //simplifies moves
    //if running for the last time
    if (order_num == 4) {
      endStep(1);  //ends the step, copies the cube, saves the "start" counter
    }
    //if not running for the last time
    else {
      order_moves[order_num] = move_num;  //cataloges the number of moves taken for this combo
      resetStep();                        //resets steps
    }
  }
}
//STEP 2: F2L method to complete the white face and the first 2 layers
void F2L() {
  int pnum, num, edge1, edge2, con1, con2, turn;
  int solved[4];                  //if pair i is solved the value is 1, else its 0
  char topC, topE, side1, side2;  //top corner color, top edge color, corner side 1 color, corner side 2 color
  //assignment of the two useful faces for each pair
  //index 0 = face considered to be the front when working with the pair in the right hand
  //index 1 = face considered to be the right when working with the pair in the right hand
  int face[4][2] = { { top, right }, { right, bottom }, { left, top }, { bottom, left } };
  int order_moves[24];  //the is the number of moves made by each order

  //runs the 24 different combinations that the edge-corner pairs can be brought in
  //runs 25 times. On the 25th time, all combos have been found. The comb0 with the smallest number of moves is run again and saved
  for (int order = 0; order < 25; order++) {
    //resets the solved variable
    for (int i = 0; i < 4; i++) {
      solved[i] = 0;
    }
    //runs 4 times to make and insert each corner - edge pair
    for (int i = 0; i < 4; i++) {
      //if running the last time pnum will go in the sequence of the order with the smallest number of moves
      if (order == 24) {
        pnum = order4P4(minAmount(order_moves, 24), i);
      }
      //gets the pair number to find next from the permuation 4P4
      else {
        pnum = order4P4(order, i);
      }
      //________________________________________________________________
      //moves the edge and corner pair to the top
      num = 1;
      while (num != 0) {
        num = 0;
        //cycles through each bottom corner to potentially find the needed corner piece
        //if found, the corner is moved to the top
        for (int cnum = 0; cnum < 4; cnum++) {
          //checks if the corner is found
          if (cornerCheck(bottom, cnum, pnum) == 1) {
            num++;
            break;  //breaks from the for loop
          }
        }
        //cycles through each bottom edge to potentially find the needed edge piece
        //if found, the edge is moved to the top
        for (int ednum = 0; ednum < 4; ednum++) {
          //checks if the corner is found
          if (edgeCheck(bottom, ednum, pnum) == 1) {
            num++;
            break;  //breaks from the for loop
          }
        }
      }
      //moves the corner to be above the spot where its suppose to go
      while (cornerCheck(top, pnum, pnum) == 0) {
        turnSide(back, 1);
      }
      //________________________________________________________________
      //defines variables to be used for each pair
      switch (pnum) {
        case 0:  //red and green
          //define the color of the top, side1, and side 2 of the corner
          topC = cube[back][0];
          side1 = cube[top][6];
          side2 = cube[right][6];
          //defines the edges that cannot be associated with this pair (conflicting edges)
          con1 = 0;
          con2 = 1;
          //defines the edges that can be associated with this pair
          edge1 = 2;
          edge2 = 3;
          break;
        case 1:  //green and orange
          //define the color of the top, side1, and side 2 of the corner
          topC = cube[back][2];
          side1 = cube[right][8];
          side2 = cube[bottom][8];
          //defines the edges that cannot be associated with this pair (conflicting edges)
          con1 = 2;
          con2 = 0;
          //defines the edges that can be associated with this pair
          edge1 = 3;
          edge2 = 1;
          break;
        case 2:  //red and blue
          //define the color of the top, side1, and side 2 of the corner
          topC = cube[back][6];
          side1 = cube[left][0];
          side2 = cube[top][0];
          //defines the edges that cannot be associated with this pair (conflicting edges)
          con1 = 1;
          con2 = 3;
          //defines the edges that can be associated with this pair
          edge1 = 0;
          edge2 = 2;
          break;
        case 3:  //red and orange
          //define the color of the top, side1, and side 2 of the corner
          topC = cube[back][8];
          side1 = cube[bottom][2];
          side2 = cube[left][2];
          //defines the edges that cannot be associated with this pair (conflicting edges)
          con1 = 3;
          con2 = 2;
          //defines the edges that can be associated with this pair
          edge1 = 1;
          edge2 = 0;
          break;
      }
      //________________________________________________________________
      //checks for and fixes a conflicting edge on position con1
      if (edgeCheck(top, con1, pnum) == 1) {
        topE = edgeColor(con1);  //defines the color of the top edge using conflict #1 (aka the edge number)
        //if the edge has been solved
        if (topC == topE && side1 == 'W') {
          solved[pnum] = 1;
        }
        //seperate the match by hiding the corner (hide 2) and moving the edge
        else {
          hideCorner(solved, pnum, 'B', 3);
        }
      }
      //checks for and fixes a conflicting edge on position con2
      else if (edgeCheck(top, con2, pnum) == 1) {
        topE = edgeColor(con2);  //defines the color of the top edge using conflict #2 (aka the edge number)
        //if the match has been solved
        if (topC == topE && side2 == 'W') {
          solved[pnum] = 1;
        }
        //seperates the match by hiding the corner (hide A) and moving the edge
        else {
          hideCorner(solved, pnum, 'A', 1);
        }
      }
      //determines the top color of the edge
      //if the edge is edge 1, top will be the color of edge 1 (same with edge 2)
      topE = (edgeCheck(top, edge1, pnum) * (int)edgeColor(edge1)) + (edgeCheck(top, edge2, pnum) * (int)edgeColor(edge2));
      //________________________________________________________________
      //if the edge-corner pair has not been solved yet
      if (solved[pnum] == 0) {
        //case #1 (matching): white is side 2 and top colors are the same
        if (side2 == 'W' && topC == topE) {
          //if edge1, turn = 2, if edge 2, turn = 3
          turn = (edgeCheck(top, edge1, pnum) * 2) + (edgeCheck(top, edge2, pnum) * 3);
          hideCorner(solved, pnum, 'A', turn);  //hides the corner, moves the edge piece, unhides the corner so that they are together
          //inserts the pair
          turnSide(back, 3);
          turnSide(face[pnum][0], 3);
          turnSide(back, 1);
          turnSide(face[pnum][0], 1);
        }
        //case #2 (non matching): non matching: white is side 2 and top colors are not the same
        else if (side2 == 'W' && topC != topE) {
          //if edge is edge2 then perform a hide to make it edge1
          if (edgeCheck(top, edge2, pnum) == 1) {
            hideCorner(solved, pnum, 'A', 1);
          }
          //inserts the pair
          turnSide(face[pnum][1], 1);
          turnSide(back, 1);
          turnSide(face[pnum][1], 3);
        }
        //case #3 (white on top): top color is white
        else if (topC == 'W') {
          //if edge is edge1
          if (edgeCheck(top, edge1, pnum) == 1) {
            //center matches the edge piece
            //for example (pair 0): if the top is red then the side is green so match to green
            if (topE == pair[pnum][0]) {
              turnSide(back, 1);
              turnSide(face[pnum][1], 1);
              turnSide(back, 2);
              turnSide(face[pnum][1], 3);
              turnSide(back, 1);
              //inserts the pair
              turnSide(face[pnum][1], 1);
              turnSide(back, 3);
              turnSide(face[pnum][1], 3);
            }
            //for example (pair 0): if the top is greeb then the side is red so match to red
            else if (topE == pair[pnum][1]) {
              turnSide(back, 2);
              turnSide(face[pnum][0], 3);
              turnSide(back, 3);
              turnSide(face[pnum][0], 1);
              turnSide(back, 3);
              //inserts the block
              turnSide(face[pnum][0], 3);
              turnSide(back, 1);
              turnSide(face[pnum][0], 1);
            }
          }
          //if edge is 2
          else if (edgeCheck(top, edge2, pnum) == 1) {
            //center matches the color of the side part of the edge piece
            //for example (pair 1): if the top is red then the side is green so match green
            if (topE == pair[pnum][0]) {
              turnSide(back, 2);
              turnSide(face[pnum][1], 1);
              turnSide(back, 1);
              turnSide(face[pnum][1], 3);
              turnSide(back, 1);
              //inserts the pair
              turnSide(face[pnum][1], 1);
              turnSide(back, 3);
              turnSide(face[pnum][1], 3);
            }
            //for example (pair 1): if the top is green then the side is red so match red
            else if (topE == pair[pnum][1]) {
              turnSide(back, 3);
              turnSide(face[pnum][0], 3);
              turnSide(back, 2);
              turnSide(face[pnum][0], 1);
              turnSide(back, 3);
              //inserts pair
              turnSide(face[pnum][0], 3);
              turnSide(back, 1);
              turnSide(face[pnum][0], 1);
            }
          }
        }
        //case #4 (mirrored matching): white is side 1 and top colors match
        else if (side1 == 'W' && topC == topE) {
          //if edge1, turn = 1, if edge2, turn = 2
          turn = (edgeCheck(top, edge1, pnum) * 1) + (edgeCheck(top, edge2, pnum) * 2);
          hideCorner(solved, pnum, 'B', turn);  //hides the corner, moves the edge piece, unhides the corner so that they are together
          turnSide(back, 3);                    //undoes the last command of the hide move
          //drops the corner - edge pair in place
          turnSide(back, 2);
          turnSide(face[pnum][1], 1);
          turnSide(back, 3);
          turnSide(face[pnum][1], 3);
        }
        //case #5 (mirrored non matching): white is side 1 and top colors dont match
        else if (side1 == 'W' && topC != topE) {
          //if the edge is in position edge1 put it in positon edge2
          if (edgeCheck(top, edge1, pnum) == 1) {
            hideCorner(solved, pnum, 'A', 3);  //hides the corner, moves the edge piece, unhides the corner so that they are together
          }
          //inserts the pair
          turnSide(face[pnum][0], 3);
          turnSide(back, 3);
          turnSide(face[pnum][0], 1);
        }
        solved[pnum] = 1;
      }
      //if the edge-corner pair has already been solved
      else if (solved[pnum] == 1) {
        //if side 1 is white
        if (side1 == 'W') {
          turnSide(back, 1);
          turnSide(face[pnum][1], 1);
          turnSide(back, 3);
          turnSide(face[pnum][1], 3);
        }
        //if side 2 is white
        if (side2 == 'W') {
          turnSide(back, 3);
          turnSide(face[pnum][0], 3);
          turnSide(back, 1);
          turnSide(face[pnum][0], 1);
        }
      }
    }
    //if running for the last time
    if (order == 24) {
      endStep(2);  //ends the step, copies the cube, saves the "start" counter
    }
    //if not running for the last time
    else {
      simplifyMoves();                //simplifies the moves
      order_moves[order] = move_num;  //captures the number of moves taken for this order
      resetStep();                    //resets the last steps
    }
  }
}
//STEP 3: OLL method to complete the yellow face
void OLL() {
  int F, f, R, U, check;
  //these are the 4 edges that matter when performing OLL
  char b[9];
define:
  for (int i = 0; i < 9; i++) {
    b[i] = cube[back][i];
  }
  //-----All these shapes require 2 look operations---
  //line cases
  if (b[3] == 'Y' && b[5] == 'Y' && b[1] != 'Y' && b[7] != 'Y' || b[1] == 'Y' && b[7] == 'Y' && b[3] != 'Y' && b[5] != 'Y') {
    //line case 1
    if (b[3] == 'Y') {
      F = right;
      R = bottom;
    }
    //line case 2
    else if (b[1] == 'Y') {
      F = top;
      R = right;
    }
    turnSide(F, 1);     //F
    turnSide(R, 1);     //R
    turnSide(back, 1);  //U
    turnSide(R, 3);     //R'
    turnSide(back, 3);  //U'
    turnSide(F, 3);     //F'
    goto define;        //redefines b array
  }
  //dot
  else if (b[1] != 'Y' && b[7] != 'Y' && b[3] != 'Y' && b[5] != 'Y') {
    turnSide(top, 1);     //F
    turnSide(right, 1);   //R
    turnSide(back, 1);    //U
    turnSide(right, 3);   //R'
    turnSide(back, 3);    //U'
    turnSide(top, 3);     //F'
    turnSide(bottom, 1);  //f
    turnSide(back, 1);    //R
    turnSide(left, 1);    //U
    turnSide(back, 3);    //R'
    turnSide(left, 3);    //U'
    turnSide(bottom, 3);  //f'
    goto define;
  }
  //clock cases
  else {
    check = 0;
    //clock case 1
    if (b[1] == 'Y' && b[3] == 'Y' && b[5] != 'Y' && b[7] != 'Y') {
      f = bottom;
      U = left;
      check = 1;
    }
    //clock case 2
    else if (b[1] == 'Y' && b[5] == 'Y' && b[3] != 'Y' && b[7] != 'Y') {
      f = left;
      U = top;
      check = 1;
    }
    //clock case 3
    else if (b[5] == 'Y' && b[7] == 'Y' && b[1] != 'Y' && b[3] != 'Y') {
      f = top;
      U = right;
      check = 1;
    }
    //clock case 4
    else if (b[3] == 'Y' && b[7] == 'Y' && b[1] != 'Y' && b[5] != 'Y') {
      f = right;
      U = bottom;
      check = 1;
    }
    //does moves if there is a clock shape
    if (check == 1) {
      turnSide(f, 1);     //f
      turnSide(back, 1);  //R
      turnSide(U, 1);     //U
      turnSide(back, 3);  //R'
      turnSide(U, 3);     //U'
      turnSide(f, 3);     //f'
      goto define;
    }
  }

  //-----All these are solved one time---
  //checks if the cube is already solved
  if (b[0] == 'Y' && b[1] == 'Y' && b[2] == 'Y' && b[3] == 'Y' && b[4] == 'Y' && b[5] == 'Y' && b[6] == 'Y' && b[7] == 'Y' && b[8] == 'Y') {
  }
  //diagonal pieces
  else if (b[0] == 'Y' && b[8] == 'Y' || b[6] == 'Y' && b[2] == 'Y') {
    while (cube[top][0] != 'Y' || cube[right][8] != 'Y') {
      turnSide(back, 1);
    }
    turnSide(right, 3);   //R'
    turnSide(top, 1);     //U
    turnSide(right, 1);   //R
    turnSide(bottom, 3);  //D'
    turnSide(right, 3);   //R'
    turnSide(top, 3);     //U'
    turnSide(right, 1);   //R
    turnSide(bottom, 1);  //D
  }
  //horizontal pieces
  else if (b[0] == 'Y' && b[2] == 'Y' || b[2] == 'Y' && b[8] == 'Y' || b[6] == 'Y' && b[8] == 'Y' || b[0] == 'Y' && b[6] == 'Y') {
    //different side match
    if (cube[top][0] == 'Y' && cube[bottom][2] == 'Y' || cube[top][6] == 'Y' && cube[bottom][8] == 'Y' || cube[left][0] == 'Y' && cube[right][6] == 'Y' || cube[right][8] == 'Y' && cube[left][2] == 'Y') {
      while (cube[top][0] != 'Y' && cube[right][2] != 'Y') {
        turnSide(back, 1);
      }
      turnSide(left, 1);   //L
      turnSide(top, 1);    //U
      turnSide(right, 3);  //R'
      turnSide(top, 3);    //U'
      turnSide(left, 3);   //L'
      turnSide(top, 1);    //U
      turnSide(right, 1);  //R
      turnSide(top, 3);    //U'
    }
    //same side match
    else {
      //printCube(all)
      //while(1){}
      //moves the headlights to the top
      while (cube[top][0] != 'Y') {
        turnSide(back, 1);
      }
      turnSide(right, 2);  //2R
      turnSide(front, 1);  //D
      turnSide(right, 3);  //R'
      turnSide(back, 2);   //2U
      turnSide(right, 1);  //R
      turnSide(front, 3);  //D'
      turnSide(right, 3);  //R'
      turnSide(back, 2);   //2U
      turnSide(right, 3);  //R'
    }
  }
  //fish
  else if (b[0] == 'Y' || b[2] == 'Y' || b[6] == 'Y' || b[8] == 'Y') {
    while (1) {
      if (cube[back][0] == 'Y' && cube[top][0] == 'Y') {
        break;
      } else if (cube[back][6] == 'Y' && cube[top][6] == 'Y') {
        break;
      }
      turnSide(back, 1);
    }
    //left version of alg
    if (cube[top][0] == 'Y') {
      turnSide(left, 3);  //L'
      turnSide(back, 3);  //U'
      turnSide(left, 1);  //L
      turnSide(back, 3);  //U'
      turnSide(left, 3);  //L'
      turnSide(back, 2);  //2U
      turnSide(left, 1);  //L
    }
    //right version of alg
    else if (cube[top][6] == 'Y') {
      turnSide(right, 1);  //R
      turnSide(back, 1);   //U
      turnSide(right, 3);  //R'
      turnSide(back, 1);   //U
      turnSide(right, 1);  //R
      turnSide(back, 2);   //2U
      turnSide(right, 3);  //R'
    }
  }
  //yellow cross
  else if (b[0] != 'Y' || b[2] != 'Y' || b[6] != 'Y' || b[8] != 'Y') {
    //  printf("yellow cross");
    while (cube[top][6] != 'Y') {
      turnSide(back, 1);
    }
    //headlight pair
    if (cube[top][0] == 'Y' && cube[bottom][2] == 'Y' && cube[bottom][8] == 'Y') {
      turnSide(top, 1);  //F
      //runs 3 times
      for (int i = 0; i < 3; i++) {
        turnSide(right, 1);  //R
        turnSide(back, 1);   //U
        turnSide(right, 3);  //R'
        turnSide(back, 3);   //U'
      }
      turnSide(top, 3);  //F
    }
    //no headlight pair
    else {
      while (cube[top][0] == 'Y' || cube[top][6] != 'Y') {
        turnSide(back, 1);
      }
      turnSide(right, 1);  //R
      turnSide(back, 2);   //2U
      turnSide(right, 2);  //2R
      turnSide(back, 3);   //U'
      turnSide(right, 2);  //2R
      turnSide(back, 3);   //U'
      turnSide(right, 2);  //2R
      turnSide(back, 2);   //2U
      turnSide(right, 1);  //R
    }
  }
  endStep(3);  //ends the step, copies the cube, saves the "start" counter
}
//STEP 4: PLL method to complete the cube
void PLL() {
  int F, R, num, M1, M2, U1, U2;
  char color;
  //defines the two corners on each face
  char t1 = cube[top][0], t2 = cube[top][6], t3 = cube[top][3];
  char b1 = cube[bottom][2], b2 = cube[bottom][8], b3 = cube[bottom][5];
  char r1 = cube[right][6], r2 = cube[right][8], r3 = cube[right][7];
  char l1 = cube[left][0], l2 = cube[left][2], l3 = cube[left][1];

  //checks if the cube is solved
  if (t1 == t2 && t2 == t3 && r1 == r2 && r2 == r3) {
  }
  //if the cube is not solved
  else {
    //-----All these are 2 look operations---
    //all corners are different (diagional corner swap)
    if (t1 != t2 && b1 != b2 && r1 != r2 && l1 != l2) {
      turnSide(top, 1);    //F
      turnSide(right, 1);  //R
      turnSide(back, 3);   //U'
      turnSide(right, 3);  //R'
      turnSide(back, 3);   //U'
      turnSide(right, 1);  //R
      turnSide(back, 1);   //U
      turnSide(right, 3);  //R'
      turnSide(top, 3);    //F'
      turnSide(right, 1);  //R
      turnSide(back, 1);   //U
      turnSide(right, 3);  //R'
      turnSide(back, 3);   //U'
      turnSide(right, 3);  //R'
      turnSide(top, 1);    //F
      turnSide(right, 1);  //R
      turnSide(top, 3);    //F'
    }
    //one pair of corners are solved (adjacent corner swap)
    else if (t1 == t2 && b1 != b2 || b1 == b2 && r1 != r2 || r1 == r2 && l1 != l2 || l1 == l2 && t1 != t2) {
      //if the front side has the match
      if (t1 == t2) {
        matchCenter(top, 0);
        color = t1;
      }
      //if the bottom side has the match
      else if (b1 == b2) {
        matchCenter(bottom, 2);
        color = b1;
      }
      //if the right side has the match
      else if (r1 == r2) {
        matchCenter(right, 6);
        color = r1;
      }
      //if the left side has the match
      else if (l1 == l2) {
        matchCenter(left, 0);
        color = l1;
      }

      switch (color) {
        case 'R':  //red
          F = right;
          R = bottom;
          break;
        case 'O':  //orange
          F = left;
          R = top;
          break;
        case 'G':  //green
          F = bottom;
          R = left;
          break;
        case 'B':  //blue
          F = top;
          R = right;
          break;
      }
      turnSide(R, 1);     //R
      turnSide(back, 1);  //U
      turnSide(R, 3);     //R'
      turnSide(back, 3);  //U'
      turnSide(R, 3);     //R'
      turnSide(F, 1);     //F
      turnSide(R, 2);     //2R
      turnSide(back, 3);  //U'
      turnSide(R, 3);     //R'
      turnSide(back, 3);  //U'
      turnSide(R, 1);     //R
      turnSide(back, 1);  //U
      turnSide(R, 3);     //R'
      turnSide(F, 3);     //F'
    }

    matchCenter(top, 0);  //ensures that all corners are matched
    //re-defines the two corners on each face
    t1 = cube[top][0], t2 = cube[top][6], t3 = cube[top][3];
    b1 = cube[bottom][2], b2 = cube[bottom][8], b3 = cube[bottom][5];
    r1 = cube[right][6], r2 = cube[right][8], r3 = cube[right][7];
    l1 = cube[left][0], l2 = cube[left][2], l3 = cube[left][1];

    //-----All these are 1 look operations---
    //one side is completely solved
    if (t1 == t3 && b1 != b3 || b1 == b3 && t1 != t3 || r1 == r3 && l1 != l3 || l1 == l3 && r1 != r3) {
      //if the top side is solved
      if (t1 == t3) {
        //alg option 1
        if (l3 == 'G') {
          num = 0;
        }
        //alg option 2
        else if (r3 == 'B') {
          num = 1;
        }
        R = left;
      }
      //if the bottom side is solved
      else if (b1 == b3) {
        //alg option 1
        if (r3 == 'B') {
          num = 0;
        }
        //alg option 2
        else if (l3 == 'G') {
          num = 1;
        }
        R = right;
      }
      //if the right side is solved
      else if (r1 == r3) {
        //alg option 1
        if (t3 == 'O') {
          num = 0;
        }
        //alg option 2
        else if (b3 == 'R') {
          num = 1;
        }
        R = top;
      }
      //if the left side is solved
      else if (l1 == l3) {
        //alg option 1
        if (b3 == 'R') {
          num = 0;
        }
        //alg option 2
        else if (t3 == 'O') {
          num = 1;
        }
        R = bottom;
      }

      //alg 1
      if (num == 0) {
        turnSide(R, 1);     //R
        turnSide(back, 3);  //U'
        turnSide(R, 1);     //R
        turnSide(back, 1);  //U
        turnSide(R, 1);     //R
        turnSide(back, 1);  //U
        turnSide(R, 1);     //R
        turnSide(back, 3);  //U'
        turnSide(R, 3);     //R'
        turnSide(back, 3);  //U'
        turnSide(R, 2);     //2R

      }
      //alg 2
      else if (num == 1) {
        turnSide(R, 2);     //2R
        turnSide(back, 1);  //U
        turnSide(R, 1);     //R
        turnSide(back, 1);  //U
        turnSide(R, 3);     //R'
        turnSide(back, 3);  //U'
        turnSide(R, 3);     //R'
        turnSide(back, 3);  //U'
        turnSide(R, 3);     //R'
        turnSide(back, 1);  //U
        turnSide(R, 3);     //R'
      }
    }
    //no sides are solved
    else if (t1 != t3 && b1 != b3 && r1 != r3 && l1 != l3) {
      //opposite sides have the middle piece
      if (t3 == 'O') {
        turnSide(right, 2);  //2M
        turnSide(left, 2);   //2M
        turnSide(front, 3);  //U'
        turnSide(right, 2);  //2M
        turnSide(left, 2);   //2M
        turnSide(back, 2);   //2U
        turnSide(right, 2);  //2M
        turnSide(left, 2);   //2M
        turnSide(front, 3);  //U'
        turnSide(right, 2);  //2M
        turnSide(left, 2);   //2M
      }
      //the next side has the middle piece
      else {
        //top and right need to swap
        if (t3 == 'G' && r3 == 'R') {
          M1 = right;
          M2 = left;
          U1 = top;
          U2 = bottom;
        }
        //right and bottom need to swap
        else if (r3 == 'O' && b3 == 'G') {
          M1 = bottom;
          M2 = top;
          U1 = right;
          U2 = left;
        }
        //bottom  and left need to swap
        else if (b3 == 'B' && l3 == 'O') {
          M1 = left;
          M2 = right;
          U1 = bottom;
          U2 = top;
        }
        //left and top need to swap
        else if (t3 == 'B' && l3 == 'R') {
          M1 = top;
          M2 = bottom;
          U1 = left;
          U2 = right;
        }

        turnSide(M1, 3);     //M'
        turnSide(M2, 1);     //M'
        turnSide(U1, 3);     //U'
        turnSide(M1, 2);     //2M
        turnSide(M2, 2);     //2M
        turnSide(U2, 3);     //U'
        turnSide(M1, 2);     //2M
        turnSide(M2, 2);     //2M
        turnSide(U1, 3);     //U'
        turnSide(M1, 3);     //M'
        turnSide(M2, 1);     //M'
        turnSide(front, 2);  //2U
        turnSide(M1, 2);     //2M
        turnSide(M2, 2);     //2M
      }
    }
  }
  matchCenter(top, 0);  //ensures that all corners are matched
  endStep(4);           //ends the step, copies the cube, saves the "start" counter
}

//**LIBRARY CONTENT***************************************************************************************
Cube::Cube() {
}
void Cube::define(char array[6][9]) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 9; j++) {
      cube[i][j] = array[i][j];
    }
  }
  copyCube();
  move_num = 0;
  start = 0;
}
void Cube::defineBlock(int face, int block, char color) {
  cube[face][block] = color;
  copyCube();
  move_num = 0;
  start = 0;
}
void Cube::solve() {
  copyCube();    //copies the cube
  resetStep();   //resets the steps
  whiteCross();  //STEP 1: create the white cross
  F2L();         //STEP 2: create and insert the F2L pairs
  OLL();         //STEP 3: solves the white face with two look OLL
  PLL();         //STEP 4: finishes solving the cube
}
void Cube::display() {
  printCube(all);
}
void Cube::turn(int face, int turn) {
  turnSide(face, turn);
  move_num = 0;
}
int Cube::getMoveNum() {
  return move_num;
}
string Cube::getMove(int num) {
  return move[num];
}
void Cube::printCommands() {
  for (int i = 0; i < move_num; i++) {
    printf("rubiksCube.turn(");
    //top
    if (move[i] == "U" || move[i] == "2U" || move[i] == "'U'") {
      printf("top");
    }
    //bottom
    else if (move[i] == "D" || move[i] == "2D" || move[i] == "'D'") {
      printf("bottom");
    }
    //front
    else if (move[i] == "F" || move[i] == "2F" || move[i] == "'F'") {
      printf("front");
    }
    //back
    else if (move[i] == "B" || move[i] == "2B" || move[i] == "'B'") {
      printf("back");
    }
    //right
    else if (move[i] == "R" || move[i] == "2R" || move[i] == "'R'") {
      printf("right");
    }
    //left
    else if (move[i] == "L" || move[i] == "2L" || move[i] == "'L'") {
      printf("left");
    }
    printf(", ");
    printf("%d", move[i].length());
    printf(");");
  }
}
