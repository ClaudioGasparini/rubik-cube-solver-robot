import RPi.GPIO as GPIO
import time
GPIO.setwarnings(False)

# Imposta i pin del driver TMC2208
DIR_PIN1 = 23  # Pin per la direzione
STEP_PIN1 = 18  # Pin per il passo
DIR_PIN2 = 25  # Pin per la direzione
STEP_PIN2 = 24  # Pin per il passo
DIR_PIN3 = 7  # Pin per la direzione
STEP_PIN3 = 8  # Pin per il passo
DIR_PIN4 = 20  # Pin per la direzione
STEP_PIN4 = 16  # Pin per il passo
DIR_PIN5 = 22  # Pin per la direzione
STEP_PIN5 = 21  # Pin per il passo
DIR_PIN6 = 17  # Pin per la direzione
STEP_PIN6 = 27  # Pin per il passo

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

num_steps = 200
delay = 0.0003


def muovi(direction, step):
    GPIO.output(direction, 1) 
    for _ in range(num_steps):
        GPIO.output(step, GPIO.HIGH)
        time.sleep(delay)
        GPIO.output(step, GPIO.LOW)
        time.sleep(delay)
    time.sleep(1)


muovi(DIR_PIN1, STEP_PIN1)
muovi(DIR_PIN2, STEP_PIN2)
muovi(DIR_PIN3, STEP_PIN3)
muovi(DIR_PIN4, STEP_PIN4)
muovi(DIR_PIN5, STEP_PIN5)
muovi(DIR_PIN6, STEP_PIN6)