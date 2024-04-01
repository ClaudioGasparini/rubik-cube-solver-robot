from ctypes import *
lib = cdll.LoadLibrary('./libCubeSolver.so')


cubeMatrix = [[ord('Y'), ord('O'), ord('B'), ord('B'), ord('R'), ord('O'), ord('W'), ord('G'), ord('W')], 
              [ord('B'), ord('O'), ord('Y'), ord('R'), ord('O'), ord('R'), ord('Y'), ord('O'), ord('O')],
              [ord('R'), ord('Y'), ord('Y'), ord('G'), ord('W'), ord('B'), ord('G'), ord('G'), ord('G')],
              [ord('G'), ord('Y'), ord('W'), ord('W'), ord('Y'), ord('W'), ord('B'), ord('Y'), ord('O')],
              [ord('R'), ord('R'), ord('R'), ord('W'), ord('G'), ord('W'), ord('O'), ord('G'), ord('B')],
              [ord('R'), ord('B'), ord('G'), ord('B'), ord('B'), ord('Y'), ord('W'), ord('R'), ord('O')]
            ]

for i in range(6):
    cubeMatrix[i] = (c_char * 9)(*cubeMatrix[i])

cubeMatrix = ((c_char * 9) * 6)(*cubeMatrix)

c = lib.Cube_new()
lib.Cube_define(c, cubeMatrix)
lib.Cube_display(c)
lib.Cube_solve(c)
lib.Cube_display(c)
move_num = lib.Cube_getMoveNum()
moves = lib.Cube_getMove(1)
print("Move num:", move_num)
