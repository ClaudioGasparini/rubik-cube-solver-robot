import cv2

def scatta_foto(nome_file="foto.jpg"):
    # Inizializza la webcam
    webcam = cv2.VideoCapture(0)  # 0 indica la webcam predefinita

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

if __name__ == "__main__":
    # Specifica il nome del file in cui salvare la foto (puoi cambiarlo)
    nome_file = "mia_foto.jpg"

    # Chiama la funzione per scattare la foto
    scatta_foto(nome_file)
