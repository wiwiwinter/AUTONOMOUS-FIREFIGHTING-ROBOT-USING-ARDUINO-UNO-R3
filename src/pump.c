#include "pump.h"

void setupPump(void) {
    DDRD |= (1 << PUMP_PIN);
    PORTD |= (1 << PUMP_PIN);  // Initialize pump in OFF state (relay ON)
}

void setPumpState(uint8_t state) {
    if (state == PUMP_ON) {
        PORTD &= ~(1 << PUMP_PIN);  // LOW to activate pump
    } else {
        PORTD |= (1 << PUMP_PIN);   // HIGH to deactivate pump
    }
}