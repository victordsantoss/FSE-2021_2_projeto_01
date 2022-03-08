#ifndef _CURVA_REFLOW_H_
#define _CURVA_REFLOW_H_

#include <stdio.h>
#include <stdlib.h>

#include "bme280.h"
#include "bme280_defs.h"

void curva_reflow_control(int uart_filesystem, struct bme280_dev bme_connection);

#endif