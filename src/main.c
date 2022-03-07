// LIBS
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// INC
#include "bme280.h"
#include "potenciometro.h";
#include "pid.h"
#include "display.h"
#include "gpio.h"
#include "thermometer.h"
#include "uart.h"
#include "terminal.h"

struct bme280_dev bme_connection;
int uart_filesystem;

void shutdown_program()
{
  system("clear");
  printf("Programa encerrado\n");
  turn_resistance_off();
  turn_fan_off();
  closeUart(uart_filesystem);
  exit(0);
}

void initProgram()
{
  wiringPiSetup();
  turn_resistance_off();
  turn_fan_off();
  connect_display();
  bme_connection = connect_bme();
  uart_filesystem = connectUart();
  system("clear");
}

void initMenu()
{
  int control;

  printf("TIPO DE CONTROLE: \n\t1) TERMINAL\n\t2) POTENCIOMETRO\n\t3) ARQUIVO DE CURVA\n");
  scanf("%d", &control);

  switch (control)
  {
  case 1:
    system("clear");
    terminal_control(uart_filesystem, bme_connection);
    break;
  case 2:
    system("clear");
    potenciometro_control(uart_filesystem, bme_connection);
    break;
  case 3:
    system("clear");
  default:
    system("clear");
    printf("Opção invalida\n");
    initMenu();
    break;
  }
}

int main()
{
  initProgram();
  signal(SIGINT, shutdown_program);
  printf(
      "\n"
      " ######################## TRABALHO 1 - FUNDAMENTOS DE SISTEMAS EMBARCADOS (2021/2) ########################"
      "\n\n");
  initMenu();
  return 0;
}
