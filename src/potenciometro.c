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

void potenciometro_control(int uart_filesystem, struct bme280_dev bme_connection)
{
  system("clear");
  float TI, TR, TE;
  int value = 0, interaction_count = 0;

  FILE *file;
  file = fopen("./files/potenciometro.csv", "w");

  if(file == NULL){
    printf("Erro na abertura do arquivo!");
    return 1;
  }

  printf("\n######################## PID ROUTINE - POTENCIOMETRO ########################\n");
  pid_setup_constants(20.0, 0.1, 100.0);

  do
  {
    writeUartGet(uart_filesystem, GET_INTERNAL_TEMP);
    TI = readUart(uart_filesystem, GET_INTERNAL_TEMP).float_value;

    double value = pid_control(TI);

    pwm_control(value);

    writeUartGet(uart_filesystem, GET_POTENTIOMETER);
    TR = readUart(uart_filesystem, GET_POTENTIOMETER).float_value;

    pid_update_reference(TR);

    TE = get_current_temperature(&bme_connection);
    printf("\tUART TI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
    print_display("UART ", TI, TR, TE);
    fprintf(file, "%d,%f,%f,%f\n", interaction_count, TI, TR, TE);

    interaction_count++;

    writeUartSend(uart_filesystem, value);
  } while (1);
  fclose(file);
  printf("###################################################################################\n");
}