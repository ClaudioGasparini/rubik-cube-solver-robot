import cv2
import numpy as np
from PIL import Image


# Inizializza la webcam
webcam = cv2.VideoCapture(0)  # 0 indica la webcam predefinita


if not webcam.isOpened():
    print("Errore: Impossibile aprire la webcam.")
    #return

# Leggi l'immagine dalla webcam
_, frame = webcam.read()

# Salva l'immagine su file
cv2.imwrite("foto.jpg", frame)

# Rilascia la risorsa della webcam
webcam.release()

print("Foto scattata con successo e salvata come foto.jpg")

