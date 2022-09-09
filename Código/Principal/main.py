import numpy as np
import RPi.GPIO as GPIO
import cv2
import pyzbar.pyzbar as pyzbar

GPIO.setmode(GPIO.BCM)
#GPIO.setwarnings(False)

class Motor():
    def __init__(self, EN, IN1, IN2):
        self.EN = EN
        self.IN1 = IN1
        self.IN2 = IN2
        GPIO.setup(self.EN, GPIO.OUT)
        GPIO.setup(self.IN1, GPIO.OUT)
        GPIO.setup(self.IN2, GPIO.OUT)
        self.pwm = GPIO.PWM(self.EN,100)
        self.pwm.start(0)
        
    def moveF(self, dc=50):
        GPIO.output(self.IN1,GPIO.LOW)
        GPIO.output(self.IN2,GPIO.HIGH)
        self.pwm.ChangeDutyCycle(dc)
    
    def moveB(self, dc=50):
        GPIO.output(self.IN1,GPIO.HIGH)
        GPIO.output(self.IN2,GPIO.LOW)
        self.pwm.ChangeDutyCycle(dc)
        
    def stop(self):
        GPIO.output(self.IN1,GPIO.LOW)
        GPIO.output(self.IN2,GPIO.LOW)
        self.pwm.ChangeDutyCycle(0)
           
LeftMotor = Motor(12, 5, 6)
RightMotor = Motor(13, 19, 26)
#ENA, IN1, IN2, IN3, IN4, ENB = 12, 5, 6, 19, 26, 13

def decodeCam(image):
    x,y,w,h = 0,0,0,0
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    barcodes = pyzbar.decode(gray)
    print('reading...', end='\r')
    for barcode in barcodes:
        x,y,w,h = barcode.rect    
    return image, x, y, w, h

camera = cv2.VideoCapture(0)

while True:
    
    ret, frame = camera.read()
    
    width = int (camera.get(3))
    height = int (camera.get(4))
    
    img1, x, y, w, h = decodeCam(frame)
    img2 = cv2.line(img1, ((width//3),0), ((width//3),height), (255, 0, 0), 1)
    img2 = cv2.line(img2, ((width//3)*2,0), ((width//3)*2,height), (255, 0, 0), 1)
    
    cv2.imshow('preview', img2)
    
    if h < (height/2):
        if x != 0:
            if (x + w/2) < (width/3):
                LeftMotor.moveB()
                RightMotor.moveF()   
            elif(x + w/2) > ((width/3)*2):
                LeftMotor.moveF()
                RightMotor.moveB()
            else:
                LeftMotor.moveF()
                RightMotor.moveF()
    else:
        LeftMotor.stop()
        RightMotor.stop()
        
    if cv2.waitKey(1) == ord('q'):
        break

camera.release()
cv2.destroyAllWindows()
print('Interrupted!')
