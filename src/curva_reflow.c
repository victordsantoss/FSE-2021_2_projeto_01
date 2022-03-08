// LIBS
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> //clock(), CLOCKS_PER_SEC e clock_t

// INC
#include "potenciometro.h"
#include "uart.h"
#include "bme280.h"
#include "display.h"
#include "thermometer.h"
#include "pid.h"
#include "gpio.h"
#include "curva_reflow.h"

struct bme280_dev bme_connection;

void curva_reflow_control(int uart_filesystem, struct bme280_dev bme_connection)
{
    system("clear");
    float TI, TR, TE;
    double kp, ki, kd;
    int value = 0, interaction_count = 0, opt, time, time_aux, contador = 0;

    FILE *file;
    file = fopen("./files/curva_reflow.csv", "rt");

    if (file == NULL)
    {
        printf("Erro na abertura do arquivo!");
        return 1;
    }

    size_t len = 100; // valor arbitrário
    char *linha = malloc(len);
    char *time_string = malloc(len);
    char *temp_string = malloc(len);
    int time_vet[100];
    float temp_vet[100];

    while (getline(&linha, &len, file) > 0)
    {
        printf("%s", linha);
        time_string = strtok(linha, ",");
        time_vet[interaction_count] = atoi(time_string);
        temp_string = strtok(NULL, ",");
        temp_vet[interaction_count] = atoi(temp_string);

        // printf("Tempo: %d\n", time_vet[interaction_count]);
        // printf("Temperatura: %2.f\n", temp_vet[interaction_count]);
        interaction_count++;
    }

    interaction_count = 0;
    printf("\n######################## PID ROUTINE - CURVA REFLOW ########################\n");
    pid_setup_constants(20.0, 0.1, 100.0);

    for (int i = 0; i < 10; i++)
    {
        while (sleep(time_vet[i] * 1000))
        {
            TR = temp_vet[i];
            writeUartGet(uart_filesystem, GET_INTERNAL_TEMP);
            TI = readUart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

            double value = pid_control(TI);
            pwm_control(value);

            pid_update_reference(TR);

            TE = get_current_temperature(&bme_connection);
            printf("\tREFLOW TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
            print_display("REFLOW ", TI, TR, TE);
            fprintf(file, "%d,%f,%f,%f\n", interaction_count, TI, TR, TE);

            writeUartSend(uart_filesystem, value);
        };
    }

    // do
    // {
    //     TR = temp_vet[interaction_count];
    //     time = time_vet[interaction_count];
    //     time_aux = time_vet[contador++];

    //     writeUartGet(uart_filesystem, GET_INTERNAL_TEMP);
    //     TI = readUart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    //     double value = pid_control(TI);
    //     pwm_control(value);

    //     pid_update_reference(TR);

    //     TE = get_current_temperature(&bme_connection);
    //     printf("\tUART TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
    //     print_display("UART ", TI, TR, TE);
    //     fprintf(file, "%d,%f,%f,%f\n", interaction_count, TI, TR, TE);

    //     writeUartSend(uart_filesystem, value);

    //     sleep((time_aux - time)*1000);
    //     interaction_count++;
    // } while (interaction_count < 10);

    fclose(file);
    printf("###################################################################################\n");
}
