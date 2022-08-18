#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
// #include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#define PINO_PWM11 12
#define PINO_PWM12 18
#define PINO_PWM21 13
#define PINO_PWM22 19

#define ESQUERDA 0
#define DIREITA 1
#define PRA_FRENTE 1
#define PRA_TRAS -1

// DEFINIR O VALOR QUE SIGNIFICA O CENTRO DA TELA
#define CENTRO_TELA 0

using namespace cv;
using namespace std;

void sendSpeed(int lado, int sentido, int velocidade){
    if(lado==ESQUERDA){
        if(sentido==PRA_FRENTE){
            gpioPWM(PINO_PWM11, velocidade);
            gpioPWM(PINO_PWM12, 0);
        }
        if(sentido==PRA_TRAS){
            gpioPWM(PINO_PWM11, 0);
            gpioPWM(PINO_PWM12, velocidade);
        }
    }
    if(lado==DIREITA){
        if(sentido==PRA_FRENTE){
            gpioPWM(PINO_PWM21, velocidade);
            gpioPWM(PINO_PWM22, 0);
        }
        if(sentido==PRA_TRAS){
            gpioPWM(PINO_PWM21, 0);
            gpioPWM(PINO_PWM22, velocidade);
        }
    }
}

void SIGINT_Handler(){
    printf("%d: Encerrando\n", getpid());
    sendSpeed(ESQUERDA, PRA_FRENTE, 0);
    sendSpeed(DIREITA, PRA_FRENTE, 0);
    gpioTerminate();
    exit(1);
}

void display(Mat &im, Mat &bbox)
{
  int n = bbox.rows;
  for(int i = 0 ; i < n ; i++)
  {
    line(im, Point2i(bbox.at<float>(i,0),bbox.at<float>(i,1)), Point2i(bbox.at<float>((i+1) % n,0), bbox.at<float>((i+1) % n,1)), Scalar(255,0,0), 3);
  }
  imshow("Result", im);
}

int main(int argc, char* argv[])
{
    // Inicializar pigpio
    if( gpioInitialise() < 0 ){
        printf("%d: Erro ao inicializar pigpio\n", getpid());
        exit(-1);
    }
    signal(SIGINT, SIGINT_Handler);

    // Definir variáveis
    int frequencia = 2000;
    int direcao;
    int velocidade;

    // Inicializar pinos PWM
    gpioSetMode(PINO_PWM11, PI_OUTPUT);
    gpioSetMode(PINO_PWM12, PI_OUTPUT);
    gpioSetMode(PINO_PWM21, PI_OUTPUT);
    gpioSetMode(PINO_PWM22, PI_OUTPUT);
    gpioSetPWMfrequency(PINO_PWM11, frequencia);
    gpioSetPWMfrequency(PINO_PWM12, frequencia);
    gpioSetPWMfrequency(PINO_PWM21, frequencia);
    gpioSetPWMfrequency(PINO_PWM22, frequencia);

    // Ler imagem
    Mat inputImage;
    if(argc>1)
        inputImage = imread(argv[1]);
    else
        inputImage = imread("qrcode-learnopencv.jpg");

    // Detectar QRCode
    QRCodeDetector qrDecoder;
    Mat bbox, rectifiedImage;

    std::string data = qrDecoder.detectAndDecode(inputImage, bbox, rectifiedImage);
    if(data.length()>0){
        cout << "Decoded Data : " << data << endl;

        // Mostrar QRCode detectado
        display(inputImage, bbox);
        rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
        imshow("Rectified QRCode", rectifiedImage);
        waitKey(0);

        // COLOCAR VALOR DE POSIÇÃO NESSA VARIAVEL
        int centroQRCode;

        // Posição aumenta da esquerda pra direita
        if(centroQRCode > CENTRO_TELA){
            velocidade = 255;
            sendSpeed(ESQUERDA, PRA_FRENTE, velocidade);
            sendSpeed(DIREITA, PRA_TRAS, velocidade);
        }else if(centroQRCode < CENTRO_TELA){
            velocidade = 255;
            sendSpeed(ESQUERDA, PRA_TRAS, velocidade);
            sendSpeed(DIREITA, PRA_FRENTE, velocidade);
        }
        // Gira motores por 1 segundo e para
        usleep(1000000);
        sendSpeed(ESQUERDA, PRA_FRENTE, 0);
        sendSpeed(DIREITA, PRA_FRENTE, 0);

        waitKey(0);
    }
    else
        cout << "QR Code not detected" << endl;
}