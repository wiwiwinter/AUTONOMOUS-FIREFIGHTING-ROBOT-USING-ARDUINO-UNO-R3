#ifndef PUMP_H
#define PUMP_H

#include <avr/io.h>
#include <util/delay.h>

#define PUMP_PIN PD7    // Pin 7 for pump control
#define PUMP_ON 0       // Inverted logic - LOW activates pump
#define PUMP_OFF 1      // Inverted logic - HIGH deactivates pump

void setupPump(void);
void setPumpState(uint8_t state);

#endif