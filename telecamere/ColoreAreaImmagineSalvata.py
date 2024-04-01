import cv2
import numpy as np
from PIL import Image

def get_color(image, area):
    # Estrai l'area predefinita dall'immagine
    x, y, w, h = area
    roi = image[y:y+h, x:x+w]

    # Calcola il colore medio dell'area predefinita
    average_color = np.mean(roi, axis=(0, 1))

    return average_color

def main():
    # Carica l'immagine salvata
    image_path = "/media/claudiogasparini/CANZONI USB/prove/image.jpg"
    photo = cv2.imread(image_path)

    if photo is None:
        print("Errore: impossibile leggere l'immagine.")
        return

    # Definisci le coordinate dell'area predefinita (x, y, larghezza, altezza)
    predefined_area = (314, 920, 74, 58)

    # Ottieni il colore medio dell'area predefinita
    color = get_color(photo, predefined_area)

    print("Colore medio dell'area predefinita:", color)

if __name__ == "__main__":
    main()
