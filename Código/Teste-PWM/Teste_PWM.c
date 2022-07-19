#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define PINO_PWM 18

void SIGINT_Handler(){
    printf("%d: Encerrando\n", getpid());

    gpioTerminate();
    exit(1);
}

int main(int argc, char** argv){
    if( gpioInitialise() < 0 ){
        printf("%d: Erro ao inicializar pigpio\n", getpid());
        exit(-1);
    }

    signal(SIGINT, SIGINT_Handler);

    gpioSetMode(PINO_PWM, PI_OUTPUT);

    int frequencia = 1;
    int dutyCycle = 50;

    while(1){
        gpioHardwarePWM(PINO_PWM, frequencia, dutyCycle*10000);
    }

    return 0;
}