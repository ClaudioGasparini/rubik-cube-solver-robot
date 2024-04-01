import RPi.GPIO as GPIO
import time
GPIO.setwarnings(False)
# Imposta i pin del driver TMC2208
DIR_PIN1 = 17 # Pin per la direzione
STEP_PIN1 = 27  # Pin per il passo

# Imposta i pin del driver TMC2208 come output
GPIO.setmode(GPIO.BCM)
GPIO.setup(DIR_PIN1, GPIO.OUT)
GPIO.setup(STEP_PIN1, GPIO.OUT)

def moveMotors(PIN_DIRECTION, PIN_STEP, direction, num_steps):
    GPIO.output(PIN_DIRECTION, direction) 
    for _ in range(num_steps*100):
        GPIO.output(PIN_STEP, GPIO.HIGH)
        time.sleep(0.0005)
        GPIO.output(PIN_STEP, GPIO.LOW)
        time.sleep(0.0005)
print("ciao")
moveMotors(DIR_PIN1, STEP_PIN1, 1, 1)
time.sleep(1)
moveMotors(DIR_PIN1, STEP_PIN1, 1, 1)
time.sleep(1)
moveMotors(DIR_PIN1, STEP_PIN1, 1, 1)
time.sleep(1)
moveMotors(DIR_PIN1, STEP_PIN1, 1, 1)
time.sleep(1)
