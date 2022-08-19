import cv2
import pyzbar.pyzbar as pyzbar
from datetime import datetime

#width = 2592
#height = 1944
cv2.namedWindow("preview")
camera = cv2.VideoCapture(0)
#camera.set(3,width)
#camera.set(4,height)

def decodeCam(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    barcodes = pyzbar.decode(gray)
    print('reading...', end='\r')
    for barcode in barcodes:
        barcodeData = barcode.data.decode()
        barcodeType = barcode.type
        (x,y,w,h) = barcode.rect
        print("["+str(datetime.now())+"] Type:{} | Data: {} | X: {} | Y: {} | W: {} | H: {} ".format(barcodeType, barcodeData, x, y, w, h))
        
    return image

try:
    while True:
        # Read current frame
        ret, frame = camera.read()
        im=decodeCam(frame)
        cv2.imshow("preview", frame)
except KeyboardInterrupt:
    cv2.destroyWindow("preview")
    camera.release()
    print('interrupted!')
