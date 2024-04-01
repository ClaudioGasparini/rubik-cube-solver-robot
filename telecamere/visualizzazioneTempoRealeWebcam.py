import cv2
import time

cap = cv2.VideoCapture(1)  # 0 indica la prima webcam
while True:
    time.sleep(0.1)
    ret, frame = cap.read()
    cv2.imshow('Webcam', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()