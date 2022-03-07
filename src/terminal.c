// LIBS
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// INC
#include "potenciometro.h"
#include "uart.h"
#include "bme280.h"
#include "display.h"
#include "thermometer.h"
#include "pid.h"
#include "gpio.h"

struct bme280_dev bme_connection;

void terminal_control(int uart_filesystem, struct bme280_dev bme_connection)
{
    system("clear");
    float TI, TR, TE;
    double kp, ki, kd;
    int value = 0, interaction_count = 0, opt;

    FILE *file;
    file = fopen("./files/terminal.csv", "w");

    if (file == NULL)
    {
        printf("Erro na abertura do arquivo!");
        return 1;
    }

    printf("\n######################## PID ROUTINE - TERMINAL ########################\n");
    printf("INSIRA O VALOR DE TR: \n");
    scanf("%f", &TR);

    printf("VALORES KP, KI, KD: \n\t1) INSERIR\n\t2) VALORES PADRÕES\n");
    scanf("%d", &opt);

    switch (opt)
    {
    case 1:
        system("clear");
        printf("INSIRA O VALOR DE kp, ki, kd: \n");
        scanf("%lf %lf %lf", &kp, &ki, &kd);
        break;
    case 2:
        system("clear");
        kp = 20.0;
        ki = 0.1;
        kd = 100.0;
        break;
    default:
        system("clear");
        printf("Opção invalida\n");
        initMenu();
        break;
    }

    pid_setup_constants(kp, ki, kd);

    do
    {
        writeUartGet(uart_filesystem, GET_INTERNAL_TEMP);
        TI = readUart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

        double value = pid_control(TI);

        pwm_control(value);

        writeUartGet(uart_filesystem, GET_POTENTIOMETER);
        pid_update_reference(TR);

        TE = get_current_temperature(&bme_connection);
        printf("\tUART TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
        print_display("UART ", TI, TR, TE);
        fprintf(file, "%d,%f,%f,%f", interaction_count, TI, TR, TE);

        interaction_count++;

        writeUartSend(uart_filesystem, value);
    } while (1);
    fclose(file);
    printf("###################################################################################\n");
}
