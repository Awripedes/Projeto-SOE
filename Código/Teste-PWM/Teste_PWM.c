#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define PINO_PWM11 12
#define PINO_PWM12 18
#define PINO_PWM21 13
#define PINO_PWM22 19

#define ESQUERDA 0
#define DIREITA 1
#define PRA_FRENTE 1
#define PRA_TRAS -1

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
    sendSpeed(DIREITA, PRA_TRAS, 0);
    gpioTerminate();
    exit(1);
}

int main(int argc, char** argv){
    if( gpioInitialise() < 0 ){
        printf("%d: Erro ao inicializar pigpio\n", getpid());
        exit(-1);
    }

    signal(SIGINT, SIGINT_Handler);

    int frequencia = 2000;
    int direcao = 1;

    gpioSetMode(PINO_PWM11, PI_OUTPUT);
    gpioSetMode(PINO_PWM12, PI_OUTPUT);
    gpioSetMode(PINO_PWM21, PI_OUTPUT);
    gpioSetMode(PINO_PWM22, PI_OUTPUT);
    gpioSetPWMfrequency(PINO_PWM11, frequencia);
    gpioSetPWMfrequency(PINO_PWM12, frequencia);
    gpioSetPWMfrequency(PINO_PWM21, frequencia);
    gpioSetPWMfrequency(PINO_PWM22, frequencia);

    while(1){
        for(int i=100; i<255; i++){
            printf("Velocidade atual: %d\n", i);
            sendSpeed(ESQUERDA, direcao, i);
            sendSpeed(DIREITA, direcao, i);
            usleep(10000);
        }
        for(int i=255; i>35; i--){
            printf("Velocidade atual: %d\n", i);
            sendSpeed(ESQUERDA, direcao, i);
            sendSpeed(DIREITA, direcao, i);
            usleep(10000);
        }
        direcao = -direcao;
    }

    return 0;
}