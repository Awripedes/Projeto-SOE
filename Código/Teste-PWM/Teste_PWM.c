#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define PINO_PWM11 12
<<<<<<< HEAD
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
=======
#define PINO_PWM12 13
#define PINO_PWM21 18
#define PINO_PWM22 19

void SIGINT_Handler(){
    printf("%d: Encerrando\n", getpid());
    gpioHardwarePWM(PINO_PWM11, 0, 0);
    gpioHardwarePWM(PINO_PWM12, 0, 0);
    gpioHardwarePWM(PINO_PWM21, 0, 0);
    gpioHardwarePWM(PINO_PWM22, 0, 0);
>>>>>>> c7b83164b1edde1ddfdc1d6505950abe416bf6c3
    gpioTerminate();
    exit(1);
}

int main(int argc, char** argv){
    if( gpioInitialise() < 0 ){
        printf("%d: Erro ao inicializar pigpio\n", getpid());
        exit(-1);
    }

    signal(SIGINT, SIGINT_Handler);

<<<<<<< HEAD
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
            sendSpeed(0, direcao, i);
            sendSpeed(1, direcao, i);
            usleep(10000);
        }
        for(int i=255; i>35; i--){
            printf("Velocidade atual: %d\n", i);
            sendSpeed(0, direcao, i);
            sendSpeed(1, direcao, i);
            usleep(10000);
        }
        sendSpeed(0, 0, 0);
        direcao = -direcao;
=======
    gpioSetMode(PINO_PWM11, PI_OUTPUT);
    gpioSetMode(PINO_PWM12, PI_OUTPUT);
    gpioSetMode(PINO_PWM21, PI_OUTPUT);
    gpioSetMode(PINO_PWM22, PI_OUTPUT);

    int frequencia = 10;
    int dutyCycle = 50;

    while(1){
        gpioHardwarePWM(PINO_PWM11, frequencia, dutyCycle*10000);
        gpioHardwarePWM(PINO_PWM12, 0, 0);
        gpioHardwarePWM(PINO_PWM21, frequencia, dutyCycle*10000);
        gpioHardwarePWM(PINO_PWM22, 0, 0);
        sleep(1);
        gpioHardwarePWM(PINO_PWM11, 0, 0);
        gpioHardwarePWM(PINO_PWM12, frequencia, dutyCycle*10000);
        gpioHardwarePWM(PINO_PWM21, 0, 0);
        gpioHardwarePWM(PINO_PWM22, frequencia, dutyCycle*10000);
        sleep(1);
>>>>>>> c7b83164b1edde1ddfdc1d6505950abe416bf6c3
    }

    return 0;
}