import RPi.GPIO as GPIO
from time import sleep

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

while True:
    
    print('etapa1')
    
    LeftMotor.moveF()
    RightMotor.moveF()
    sleep(5)
    
    print('etapa2')
    
    LeftMotor.moveB()
    RightMotor.moveB()
    sleep(5)
    
    print('etapa3')
    
    LeftMotor.moveF()
    RightMotor.moveB()
    sleep(5)
    
    print('etapa4')
    
    LeftMotor.moveB()
    RightMotor.moveF()
    sleep(5)
    
    print('etapa5')

    LeftMotor.stop()
    RightMotor.stop()
    sleep(5)
    
