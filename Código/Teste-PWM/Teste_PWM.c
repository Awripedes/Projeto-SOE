#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define PINO_PWM11 12
#define PINO_PWM12 13
#define PINO_PWM21 18
#define PINO_PWM22 19

void SIGINT_Handler(){
    printf("%d: Encerrando\n", getpid());
    gpioHardwarePWM(PINO_PWM11, 0, 0);
    gpioHardwarePWM(PINO_PWM12, 0, 0);
    gpioHardwarePWM(PINO_PWM21, 0, 0);
    gpioHardwarePWM(PINO_PWM22, 0, 0);
    gpioTerminate();
    exit(1);
}

int main(int argc, char** argv){
    if( gpioInitialise() < 0 ){
        printf("%d: Erro ao inicializar pigpio\n", getpid());
        exit(-1);
    }

    signal(SIGINT, SIGINT_Handler);

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
    }

    return 0;
}