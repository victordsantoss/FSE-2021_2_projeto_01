#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdio.h>
#include <stdlib.h>

#include "bme280.h"
#include "bme280_defs.h"

void terminal_control(int uart_filesystem, struct bme280_dev bme_connection);

#endif