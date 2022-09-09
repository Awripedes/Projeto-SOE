import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar

def decodeCam(image):
    x,y,w,h = 0,0,0,0
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    barcodes = pyzbar.decode(gray)
    print('reading...', end='\r')
    for barcode in barcodes:
        #barcodeData = barcode.data.decode()
        #barcodeType = barcode.type
        x,y,w,h = barcode.rect
        #print("X: {} | Y: {} | W: {} | H: {} ".format(x, y, w, h))
        #print("Type:{} | Data: {} | X: {} | Y: {} | W: {} | H: {} ".format(barcodeType, barcodeData, x, y, w, h))
        
    return image, x, y, w, h

camera = cv2.VideoCapture(0)


while True:
    ret, frame = camera.read()
    
    width = int (camera.get(3))
    height = int (camera.get(4))
    
    img1, x, y, w, h = decodeCam(frame)
    
    #img2 = cv2.line(img1, (width//2,0), (width//2,height), (255, 0, 0), 1)
    img2 = cv2.line(img1, ((width//3),0), ((width//3),height), (255, 0, 0), 1)
    img2 = cv2.line(img2, ((width//3)*2,0), ((width//3)*2,height), (255, 0, 0), 1)
    
    cv2.imshow('preview', img2)
    
    if h < (height/2):
        if x != 0:
            if (x + w/2) < (width/3):
                print('esquerda')     
            elif(x + w/2) > ((width/3)*2):
                print('direita')
            else:
                print('centro')
    else:
        print('pare')
        
    if cv2.waitKey(1) == ord('q'):
        break

camera.release()
cv2.destroyAllWindows()
print('Interrupted!')
