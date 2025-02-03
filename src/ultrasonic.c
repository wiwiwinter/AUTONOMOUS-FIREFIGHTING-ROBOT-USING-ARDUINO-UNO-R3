#include "ultrasonic.h"

volatile uint16_t sensor_values[4] = {1023, 1023, 1023, 1023};
volatile uint8_t adc_channel = 0;
volatile uint8_t fire_detected = 0;
volatile uint8_t obstacle_detected = 0;

ISR(ADC_vect) {
    sensor_values[adc_channel] = ADC;
    
    // Check for fire (only on fire sensors)
    if((adc_channel == FIRE_SENSOR_RIGHT || adc_channel == FIRE_SENSOR_LEFT) && 
        ADC < FIRE_THRESHOLD) {
        fire_detected = 1;
    }
    
    // Check for obstacles (only on IR sensors)
    if((adc_channel == IR_SENSOR_RIGHT || adc_channel == IR_SENSOR_LEFT) && 
        ADC < IR_THRESHOLD) {
        obstacle_detected = 1;
    }
    
    // Move to next channel
    adc_channel = (adc_channel + 1) % 4;
    ADMUX = (ADMUX & 0xF0) | adc_channel;
    
    // Start next conversion
    ADCSRA |= (1 << ADSC);
}

void setupTimer1(void) {
    TCCR1A = 0;
    TCCR1B = 0;
    ICR1 = 40000;
    TCCR1A |= (1 << COM1B1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
}

void setupServo(void) {
    DDRB |= (1 << SERVO_PIN);
    OCR1B = SERVO_CENTER;
}

void setupUltrasonic(void) {
    DDRB |= (1 << TRIGGER_PIN);
    PORTB &= ~(1 << TRIGGER_PIN);
    DDRB &= ~(1 << ECHO_PIN);
}

void setupSensors(void) {
    // Setup ADC with interrupt
    ADMUX = (1 << REFS0);  // AVCC reference
    ADCSRA = (1 << ADEN) |  // Enable ADC
             (1 << ADIE) |  // Enable Interrupt
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Prescaler 128
    
    // Start first conversion
    ADCSRA |= (1 << ADSC);
}

uint16_t measureDistance(void) {
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIGGER_PIN);
    
    uint16_t timeout = 0;
    while (!(PINB & (1 << ECHO_PIN))) {
        timeout++;
        if (timeout > 60000) return 65535;
    }
    
    TCNT1 = 0;
    timeout = 0;
    
    while (PINB & (1 << ECHO_PIN)) {
        timeout++;
        if (timeout > 60000) return 65535;
    }
    
    return (TCNT1 / 148);
}

void moveServo(uint16_t position) {
    OCR1B = position;
    _delay_ms(500);
}