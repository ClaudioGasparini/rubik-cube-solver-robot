import RPi.GPIO as GPIO
import time
from ctypes import *
lib = cdll.LoadLibrary('./libCubeSolver.so')


# Imposta i pin del driver TMC2208
DIR_PIN1 = 18  # Pin per la direzione
STEP_PIN1 = 17  # Pin per il passo
DIR_PIN2 = 23  # Pin per la direzione
STEP_PIN2 = 22  # Pin per il passo
DIR_PIN3 = 25  # Pin per la direzione
STEP_PIN3 = 24  # Pin per il passo
DIR_PIN4 = 8  # Pin per la direzione
STEP_PIN4 = 7  # Pin per il passo
DIR_PIN5 = 6  # Pin per la direzione
STEP_PIN5 = 5  # Pin per il passo
DIR_PIN6 = 26  # Pin per la direzione
STEP_PIN6 = 16  # Pin per il passo

# Imposta i pin del driver TMC2208 come output
GPIO.setmode(GPIO.BCM)
GPIO.setup(DIR_PIN1, GPIO.OUT)
GPIO.setup(STEP_PIN1, GPIO.OUT)
GPIO.setup(DIR_PIN2, GPIO.OUT)
GPIO.setup(STEP_PIN2, GPIO.OUT)
GPIO.setup(DIR_PIN3, GPIO.OUT)
GPIO.setup(STEP_PIN3, GPIO.OUT)
GPIO.setup(DIR_PIN4, GPIO.OUT)
GPIO.setup(STEP_PIN4, GPIO.OUT)
GPIO.setup(DIR_PIN5, GPIO.OUT)
GPIO.setup(STEP_PIN5, GPIO.OUT)
GPIO.setup(DIR_PIN6, GPIO.OUT)
GPIO.setup(STEP_PIN6, GPIO.OUT)

def moveMotors(PIN_DIRECTION, PIN_STEP, direction, num_steps):
    GPIO.output(PIN_DIRECTION, direction) 
    for _ in range(num_steps):
        GPIO.output(PIN_STEP, GPIO.HIGH)
        time.sleep(delay)
        GPIO.output(PIN_STEP, GPIO.LOW)
        time.sleep(delay)
    time.sleep(0.05)

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
moves = lib.Cube_getMove()
print("Move num:", move_num)
print(moves)
