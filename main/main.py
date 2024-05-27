import RPi.GPIO as GPIO
import time
import cv2
import os
import numpy as np
import pandas as pd
from PIL import Image
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306
from PIL import ImageFont
from ctypes import *

current_directory = os.path.dirname(__file__)

lib = cdll.LoadLibrary(current_directory + '/libCubeSolver.so')

csv_front = pd.read_csv(current_directory + "/colors-front.csv",
                  names=["R", "G", "B", "Name"],
                  header=0)

csv_up = pd.read_csv(current_directory + "/colors-up.csv",
                  names=["R", "G", "B", "Name"],
                  header=0)

csv_down = pd.read_csv(current_directory + "/colors-down.csv",
                  names=["R", "G", "B", "Name"],
                  header=0)

# Imposta i pin del driver TMC2208
DIR_PIN_DX = 7  	# Pin per la direzione
STEP_PIN_DX = 8 	# Pin per il passo
DIR_PIN_SX = 25  	# Pin per la direzione
STEP_PIN_SX = 24 	# Pin per il passo
DIR_PIN_BK = 23   	# Pin per la direzione
STEP_PIN_BK = 18  	# Pin per il passo
DIR_PIN_FR = 20  	# Pin per la direzione
STEP_PIN_FR = 16 	# Pin per il passo
DIR_PIN_DW = 22  	# Pin per la direzione
STEP_PIN_DW = 21 	# Pin per il passo
DIR_PIN_UP = 17  	# Pin per la direzione
STEP_PIN_UP = 27 	# Pin per il passo


# Imposta i pin dei pulsanti
redButtonPin = 9
blackButtonPin = 11



# Configura i dettagli del tuo schermo OLED
width = 128
height = 64
serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, width=width, height=height)

font_path = "/home/claudiogasparini/Desktop/Swansea-q3pd.ttf"


# Inizializzazione variabili per riconoscimento colori
ordinecolori = [5, 2, 4, 3]
ordineCaselleSopra = [0,1,2,2,5,8,8,7,6,6,3,0]
ordineCaselleSotto  = [2,1,0,0,3,6,6,7,8,8,5,2]
x = 		[1091,	1081,	1067,	928,	1,	925,	766,	775,	785,	1088,	932,	784,	1022,	941,	841]
y = 		[671,	551,	441,	680,	1,	453,	680,	561,	453,	780,	774,	786,	282,	290,	290]
widthRectCube = 20
heightRectCube = 20
# Specifica il nome del file in cui salvare la foto (puoi cambiarlo)
nome_file = current_directory + "/Photo.jpg"

cubeMatrixRecognise = [ ['0']*9, 
                        ['0']*9,
                        ['0']*9,
                        ['0']*9,
                        ['0']*9,
                        ['0']*9   ]

cubeMatrixRecognise[0][4] = 'R'
cubeMatrixRecognise[1][4] = 'O'
cubeMatrixRecognise[2][4] = 'W'
cubeMatrixRecognise[3][4] = 'Y'
cubeMatrixRecognise[4][4] = 'G'
cubeMatrixRecognise[5][4] = 'B'


# Matrice del cubo
cubeMatrix = [  [(0)]*9, 
                [(0)]*9,
                [(0)]*9,
                [(0)]*9,
                [(0)]*9,
                [(0)]*9   ]



# Imposta i pin come input e output
GPIO.setmode(GPIO.BCM)
GPIO.setup(redButtonPin, GPIO.IN)
GPIO.setup(blackButtonPin, GPIO.IN)
GPIO.setup(DIR_PIN_DX, GPIO.OUT)
GPIO.setup(STEP_PIN_DX, GPIO.OUT)
GPIO.setup(DIR_PIN_SX, GPIO.OUT)
GPIO.setup(STEP_PIN_SX, GPIO.OUT)
GPIO.setup(DIR_PIN_BK, GPIO.OUT)
GPIO.setup(STEP_PIN_BK, GPIO.OUT)
GPIO.setup(DIR_PIN_FR, GPIO.OUT)
GPIO.setup(STEP_PIN_FR, GPIO.OUT)
GPIO.setup(DIR_PIN_DW, GPIO.OUT)
GPIO.setup(STEP_PIN_DW, GPIO.OUT)
GPIO.setup(DIR_PIN_UP, GPIO.OUT)
GPIO.setup(STEP_PIN_UP, GPIO.OUT)

GPIO.setwarnings(False)

def moveMotors(PIN_DIRECTION, PIN_STEP, direction, num_steps, stepSpeed, restTime):
    GPIO.output(PIN_DIRECTION, direction) 
    for _ in range(num_steps):
        GPIO.output(PIN_STEP, GPIO.HIGH)
        time.sleep(stepSpeed)
        GPIO.output(PIN_STEP, GPIO.LOW)
        time.sleep(stepSpeed)
    time.sleep(restTime)

def clearScreen(device):
    with canvas(device) as draw:
        draw.rectangle((0,0,width,height), outline="black", fill="black")
        
def get_color(image, area):
    # Estrai l'area predefinita dall'immagine
    x, y, w, h = area
    roi = image[y:y+h, x:x+w]
    # Calcola il colore medio dell'area predefinita
    average_color = np.mean(roi, axis=(0, 1))
    # dà i colori in BGR
    return average_color

def get_color_distance(r1,g1,b1,r2,g2,b2):
    return abs(r1-r2) + abs(g1-g2) + abs(b1-b2)

def get_color_name(csv, r, g, b):
    d_min = get_color_distance(r, g, b, int(csv.loc[0,"R"]), int(csv.loc[0,"G"]), int(csv.loc[0,"B"]))
    color_name = csv.loc[0,"Name"]
    for i in range(1, len(csv)):
        d = get_color_distance(r, g, b, int(csv.loc[i,"R"]), int(csv.loc[i,"G"]), int(csv.loc[i,"B"]))
        if d < d_min :
            d_min = d
            color_name = csv.loc[i,"Name"]
    
    return color_name

def scatta_foto(nome_file):
    # Inizializza la webcam
    webcam = cv2.VideoCapture(0)  # 0 indica la webcam predefinita
    webcam.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'RGB3'))
    webcam.set(cv2.CAP_PROP_CONVERT_RGB, 1)
    cv2
    if not webcam.isOpened():
        print("Errore: Impossibile aprire la webcam.")
        return
    # Leggi l'immagine dalla webcam
    _, frame = webcam.read()
    # Salva l'immagine su file
    cv2.imwrite(nome_file, frame)
    # Rilascia la risorsa della webcam
    webcam.release()
    print(f"Foto scattata con successo e salvata come {nome_file}")

def getColorsFace (image_path, j):
    # Carica l'immagine salvata
    photo = cv2.imread(image_path)

    if photo is None:
        print("Errore: impossibile leggere l'immagine.")

    # Definisci le coordinate dell'area predefinita (x, y, larghezza, altezza)
    predefined_area = (x[j], y[j], widthRectCube, heightRectCube)
    
    # Ottieni il colore medio dell'area predefinita
    colour = get_color(photo, predefined_area)
    colour[0] = colour[0]//1
    colour[1] = colour[1]//1
    colour[2] = colour[2]//1
    print("Colore medio dell'area predefinita", j)
    print(colour)
    return colour





state=0
changeState=1
change = 0
updatePage = 1
stepSpeed = 0.0005


while True:
    redButtonState = GPIO.input(redButtonPin)
    if state==0:
        #condizione 0
        if changeState==1:
            clearScreen(device)
            with canvas(device) as draw:
                rect_width = 60
                rect_height = 20
                rect_x = (width-rect_width)//2
                rect_y = (height-rect_height)//2
                draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                font = ImageFont.truetype(font_path,12)
                text = 'START'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                draw.text((text_x, text_y), text, fill="black", font=font)
            print("pagina 0")
        if redButtonState==0:
            state = 1
            time.sleep(0.3)
            changeState = 1
        else:
            changeState = 0
    elif state==1:
        #condizione 1
        if changeState==1:
            clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,12)
                text = 'Posizionare la testa del\nrobot sopra lo specchio'
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 60
                rect_height = 20
                rect_x = (width-rect_width)//2
                rect_y = ((height-rect_height))
                draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                #font = ImageFont.load_default()
                text = 'OK'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                draw.text((text_x, text_y), text, fill="black", font=font)
            print("pagina 1")
        if redButtonState==0:
            state = 2
            time.sleep(0.3)
            changeState = 1
        else:
            changeState = 0
    elif state==2:
        #parte riconoscimento colori
        changeState==0
        clearScreen(device)
        k = 0
        q = 0
        for j in range(4):  
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,12)
                if j==0:
                    text = 'Riconoscimento in corso...\n       1 di 4'
                elif j==1:
                    text = 'Riconoscimento in corso...\n       2 di 4'
                elif j==2:
                    text = 'Riconoscimento in corso...\n       3 di 4'
                elif j==3:
                    text = 'Riconoscimento in corso...\n       4 di 4'
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)       
            moveMotors(DIR_PIN_UP, STEP_PIN_UP, 0, 100, 0.005, 0)
            # Chiama la funzione per scattare la foto
            scatta_foto(nome_file)
            for i in range (15):
                color = getColorsFace("/home/claudiogasparini/Desktop/Photo.jpg", i)
                if i==4:
                    continue
                elif i<9:
                    #casi riconoscimento colori centrali
                    #se il colore è x inserisci x in cubeMatrixRecognise[[ordinecolori[j]][i]
                    color_name = get_color_name(csv_front, color[2], color[1], color[0])
                    cubeMatrixRecognise[ordinecolori[j]][i] = color_name
                    print(color_name)
                elif i<12:
                    #casi riconoscimento colori alto
                    color_name = get_color_name(csv_up, color[2], color[1], color[0])
                    cubeMatrixRecognise[0][ordineCaselleSopra[k]] = color_name
                    print(color_name)
                    k+=1
                else:
                    #casi riconoscimento colori basso
                    color_name = get_color_name(csv_down, color[2], color[1], color[0])
                    cubeMatrixRecognise[1][ordineCaselleSotto[q]] = color_name
                    print(color_name)
                    q+=1
        
        print(cubeMatrixRecognise)
        for i in  range(6):
            for j in range(9):
                cubeMatrix[i][j] = ord(cubeMatrixRecognise[i][j])
        
        state=3
        changeState = 1
    elif state==3:
        #controllo faccia cubo 1
        blackButtonState = GPIO.input(blackButtonPin)
        redButtonState = GPIO.input(redButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  ('La faccia sopra è giusta?\n' + cubeMatrixRecognise[0][0] + "  " + cubeMatrixRecognise[0][3] + "  " + cubeMatrixRecognise[0][6] + "  "
                                                        + cubeMatrixRecognise[0][1] + "  " + cubeMatrixRecognise[0][4] + "  " + cubeMatrixRecognise[0][7] + "  "
                                                        + cubeMatrixRecognise[0][2] + "  " + cubeMatrixRecognise[0][5] + "  " + cubeMatrixRecognise[0][8]
                        )
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'SI'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5*4
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'NO'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 4
            changeState = 1
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 1
            changeState = 1

    elif state==4:
        #controllo faccia cubo 2
        blackButtonState = GPIO.input(blackButtonPin)
        redButtonState = GPIO.input(redButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  ('La faccia sotto è giusta?\n' + cubeMatrixRecognise[1][0] + "  " + cubeMatrixRecognise[1][3] + "  " + cubeMatrixRecognise[1][6] + "  "
                                                        + cubeMatrixRecognise[1][1] + "  " + cubeMatrixRecognise[1][4] + "  " + cubeMatrixRecognise[1][7] + "  "
                                                        + cubeMatrixRecognise[1][2] + "  " + cubeMatrixRecognise[1][5] + "  " + cubeMatrixRecognise[1][8]
                        )
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'SI'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5*4
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'NO'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 5
            changeState = 1
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 1
            changeState = 1

    elif state==5:
        #controllo faccia cubo 3
        blackButtonState = GPIO.input(blackButtonPin)
        redButtonState = GPIO.input(redButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  ('La faccia davanti è giusta?\n' + cubeMatrixRecognise[2][0] + "  " + cubeMatrixRecognise[2][3] + "  " + cubeMatrixRecognise[2][6] + "  "
                                                        + cubeMatrixRecognise[2][1] + "  " + cubeMatrixRecognise[2][4] + "  " + cubeMatrixRecognise[2][7] + "  "
                                                        + cubeMatrixRecognise[2][2] + "  " + cubeMatrixRecognise[2][5] + "  " + cubeMatrixRecognise[2][8]
                        )
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'SI'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5*4
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'NO'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 6
            changeState = 1
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 1
            changeState = 1


        
    elif state==6:
        #controllo faccia cubo 4

        blackButtonState = GPIO.input(blackButtonPin)
        redButtonState = GPIO.input(redButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  ('La faccia dietro è giusta?\n' + cubeMatrixRecognise[3][0] + "  " + cubeMatrixRecognise[3][3] + "  " + cubeMatrixRecognise[3][6] + "  "
                                                        + cubeMatrixRecognise[3][1] + "  " + cubeMatrixRecognise[3][4] + "  " + cubeMatrixRecognise[3][7] + "  "
                                                        + cubeMatrixRecognise[3][2] + "  " + cubeMatrixRecognise[3][5] + "  " + cubeMatrixRecognise[3][8]
                        )
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'SI'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5*4
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'NO'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 7
            changeState = 1
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 1
            changeState = 1

        
    elif state==7:
        #controllo faccia cubo 5
        blackButtonState = GPIO.input(blackButtonPin)
        redButtonState = GPIO.input(redButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  ('La faccia destra è giusta?\n' + cubeMatrixRecognise[4][0] + "  " + cubeMatrixRecognise[4][3] + "  " + cubeMatrixRecognise[4][6] + "  "
                                                        + cubeMatrixRecognise[4][1] + "  " + cubeMatrixRecognise[4][4] + "  " + cubeMatrixRecognise[4][7] + "  "
                                                        + cubeMatrixRecognise[4][2] + "  " + cubeMatrixRecognise[4][5] + "  " + cubeMatrixRecognise[4][8]
                        )
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'SI'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5*4
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'NO'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 8
            changeState = 1
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 1
            changeState = 1


        
    elif state==8:
        #controllo faccia cubo 6

        blackButtonState = GPIO.input(blackButtonPin)
        redButtonState = GPIO.input(redButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  ('La faccia sinistra è giusta?\n' + cubeMatrixRecognise[5][0] + "  " + cubeMatrixRecognise[5][3] + "  " + cubeMatrixRecognise[5][6] + "  "
                                                        + cubeMatrixRecognise[5][1] + "  " + cubeMatrixRecognise[5][4] + "  " + cubeMatrixRecognise[5][7] + "  "
                                                        + cubeMatrixRecognise[5][2] + "  " + cubeMatrixRecognise[5][5] + "  " + cubeMatrixRecognise[5][8]
                        )
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'SI'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 30
                rect_height = 15
                rect_x = (width-rect_width)//5*4
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'NO'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 9
            changeState = 1
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 1
            changeState = 1

       
    elif state==9:
        if changeState==1:
            clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,12)
                text = 'Spostare la testa del\nrobot a sinistra e incastrare\nil cubo'
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 60
                rect_height = 20
                rect_x = (width-rect_width)//2
                rect_y = ((height-rect_height))
                draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                #font = ImageFont.load_default()
                text = 'OK'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                draw.text((text_x, text_y), text, fill="black", font=font)
            print("pagina 9")
        if redButtonState==0:
            state = 10
            time.sleep(0.3)
            changeState = 1
        else:
            changeState=0
            
    elif state==10:
        #selezionare la velocita' di risoluzione
        blackButtonState = GPIO.input(blackButtonPin)
        if changeState==1:
            clearScreen(device)
            changeState=0
            updatePage=1
        if updatePage == 1:
            #clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,10)
                text =  'Seleziona la velocità'
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 35
                rect_height = 15
                rect_x = (width-rect_width)//7
                rect_y = ((height-rect_height))
                if change==0:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'Lento'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==0:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 35
                rect_height = 15
                rect_x = ((width-rect_width)//7)+rect_width+5
                rect_y = ((height-rect_height))
                if change==1:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'Medio'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==1:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
                rect_width = 35
                rect_height = 15
                rect_x = ((width-rect_width)//7)+2*rect_width+10
                rect_y = ((height-rect_height))
                if change==2:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                else:
                    draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="white", fill="black")
                #font = ImageFont.load_default()
                text = 'Veloce'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                if change==2:
                    draw.text((text_x, text_y), text, fill="black", font=font)
                else:
                    draw.text((text_x, text_y), text, fill="white", font=font)
        if blackButtonState==0 and change==0:
            time.sleep(0.3)
            change = 1
            updatePage=1
        elif blackButtonState==0 and change==1:
            time.sleep(0.3)
            change = 2
            updatePage = 1
        elif blackButtonState==0 and change==2:
            time.sleep(0.3)
            change = 0
            updatePage = 1
        else:
            updatePage=0
        if change==0 and redButtonState==0:
            time.sleep(0.3)
            state = 11
            changeState = 1
            restTimeMotor = 0.4
        elif change==1 and redButtonState==0:
            time.sleep(0.3)
            state = 11
            changeState = 1
            restTimeMotor = 0.2
        elif change==2 and redButtonState==0:
            time.sleep(0.3)
            state = 11
            changeState = 1
            restTimeMotor = 0.05
        # timeRest: lento=0.4, medio=0.1, veloce=0.05
    elif state==11:
        #ci siamo
        for i in range(6):
            cubeMatrix[i] = (c_char * 9)(*cubeMatrix[i])

        cubeMatrix = ((c_char * 9) * 6)(*cubeMatrix)

        c = lib.Cube_new()
        lib.Cube_define(c, cubeMatrix)
        lib.Cube_display(c)
        lib.Cube_solve(c)
        lib.Cube_display(c)
        move_num = lib.Cube_getMoveNum(c)

        moves = []
        for i in range(move_num):
            lib.Cube_getMove.restype=c_char_p
            moves.append(lib.Cube_getMove(c, i).decode("utf-8"))
        
        if changeState==1:
            changeState = 0
            clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,12)
                text = 'Algoritmo pronto'
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 60
                rect_height = 20
                rect_x = (width-rect_width)//2
                rect_y = ((height-rect_height))
                draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                #font = ImageFont.load_default()
                text = 'INIZIA'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                draw.text((text_x, text_y), text, fill="black", font=font)
            print("pagina 18")
        if redButtonState==0:
            state = 12
            time.sleep(0.3)
            changeState = 1
        else:
            changeState = 0


    elif state==12:
        #risoluzione in corso
        
        clearScreen(device)
        for i in moves:
            if redButtonState==0:
                state = 0
                changeState = 1
                break
            if i == 'U':
                moveMotors(DIR_PIN_UP, STEP_PIN_UP, 0, 100, stepSpeed, restTimeMotor)
            elif i =='2U':
                moveMotors(DIR_PIN_UP, STEP_PIN_UP, 0, 200, stepSpeed, restTimeMotor)
            elif i == "'U'":
                moveMotors(DIR_PIN_UP, STEP_PIN_UP, 1, 100, stepSpeed, restTimeMotor)
            elif i == 'D':
                moveMotors(DIR_PIN_DW, STEP_PIN_DW, 0, 100, stepSpeed, restTimeMotor)
            elif i =='2D':
                moveMotors(DIR_PIN_DW, STEP_PIN_DW, 0, 200, stepSpeed, restTimeMotor)
            elif i == "'D'":
                moveMotors(DIR_PIN_DW, STEP_PIN_DW, 1, 100, stepSpeed, restTimeMotor)
            elif i == 'F':
                moveMotors(DIR_PIN_FR, STEP_PIN_FR, 0, 100, stepSpeed, restTimeMotor)
            elif i =='2F':
                moveMotors(DIR_PIN_FR, STEP_PIN_FR, 0, 200, stepSpeed, restTimeMotor)
            elif i == "'F'":
                moveMotors(DIR_PIN_FR, STEP_PIN_FR, 1, 100, stepSpeed, restTimeMotor)
            elif i == 'B':
                moveMotors(DIR_PIN_BK, STEP_PIN_BK, 0, 100, stepSpeed, restTimeMotor)
            elif i =='2B':
                moveMotors(DIR_PIN_BK, STEP_PIN_BK, 0, 200, stepSpeed, restTimeMotor)
            elif i == "'B'":
                moveMotors(DIR_PIN_BK, STEP_PIN_BK, 1, 100, stepSpeed, restTimeMotor)
            elif i == 'R':
                moveMotors(DIR_PIN_DX, STEP_PIN_DX, 0, 100, stepSpeed, restTimeMotor)
            elif i =='2R':
                moveMotors(DIR_PIN_DX, STEP_PIN_DX, 0, 200, stepSpeed, restTimeMotor)
            elif i == "'R'":
                moveMotors(DIR_PIN_DX, STEP_PIN_DX, 1, 100, stepSpeed, restTimeMotor)
            elif i == 'L':
                moveMotors(DIR_PIN_SX, STEP_PIN_SX, 0, 100, stepSpeed, restTimeMotor)
            elif i =='2L':
                moveMotors(DIR_PIN_SX, STEP_PIN_SX, 0, 200, stepSpeed, restTimeMotor)
            elif i == "'L'":
                moveMotors(DIR_PIN_SX, STEP_PIN_SX, 1, 100, stepSpeed, restTimeMotor)
        state = 13
        changeState = 1
    elif state==13:
        #risoluzione finita
        if changeState==1:
            clearScreen(device)
            with canvas(device) as draw:
                font = ImageFont.truetype(font_path,12)
                text = 'Risolto!'
                text_width, text_height = font.getsize(text)
                text_x = (width-text_width)//2
                text_y = (height-text_height)//2
                draw.text((0, 20), text, fill="white", font=font)
                rect_width = 60
                rect_height = 20
                rect_x = (width-rect_width)//2
                rect_y = ((height-rect_height))
                draw.rectangle((rect_x, rect_y, rect_x+rect_width, rect_y+rect_height), outline="black", fill="white")
                #font = ImageFont.load_default()
                text = 'FINE'
                text_width, text_height = font.getsize(text)
                text_x = rect_x+(rect_width-text_width)//2
                text_y = rect_y+(rect_height-text_height)//2
                draw.text((text_x, text_y), text, fill="black", font=font)
            print("pagina 20")
        if redButtonState==0:
            state = 0
            time.sleep(0.3)
            changeState = 1
        else:
            changeState = 0
            