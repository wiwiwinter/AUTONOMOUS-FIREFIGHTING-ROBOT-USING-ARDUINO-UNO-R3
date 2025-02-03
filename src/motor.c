#include "motor.h"

void setupMotors(void) {
    DDRD |= (1 << LEFT_CONTROL) | (1 << LEFT_PWM) |
            (1 << RIGHT_CONTROL) | (1 << RIGHT_PWM);
    
    // Configure Timer0 for Fast PWM mode
    TCCR0A = (1 << WGM01) | (1 << WGM00) |
             (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = (1 << CS01) | (1 << CS00);
    
    OCR0A = 0;
    OCR0B = 0;
}

void setMotorSpeed(int motor, double speed, int direction) {
    uint8_t pwm_value = (uint8_t)(speed * MOTOR_TOP_SPEED);
    
    if (motor == MOTOR_LEFT) {
        OCR0B = pwm_value * 1.02;
        if (direction == FORWARD) {
            PORTD |= (1 << LEFT_CONTROL);
        } else {
            PORTD &= ~(1 << LEFT_CONTROL);
        }
    } else if (motor == MOTOR_RIGHT) {
        OCR0A = pwm_value * 1.08;
        if (direction == FORWARD) {
            PORTD &= ~(1 << RIGHT_CONTROL);
        } else {
            PORTD |= (1 << RIGHT_CONTROL);
        }
    }
}

void moveForward(void) {
    setMotorSpeed(MOTOR_LEFT, 0.3, FORWARD);
    setMotorSpeed(MOTOR_RIGHT, 0.3, FORWARD);
}

void moveBackward(void) {
    setMotorSpeed(MOTOR_LEFT, 0.15, BACKWARD);
    setMotorSpeed(MOTOR_RIGHT, 0.15, BACKWARD);
}

void turnLeft(void) {
    setMotorSpeed(MOTOR_LEFT, 0.4, BACKWARD);
    setMotorSpeed(MOTOR_RIGHT, 0.4, FORWARD);
    _delay_ms(500);
    stopMotors();
}

void turnRight(void) {
    setMotorSpeed(MOTOR_LEFT, 0.4, FORWARD);
    setMotorSpeed(MOTOR_RIGHT, 0.4, BACKWARD);
    _delay_ms(500);
    stopMotors();
}

void stopMotors(void) {
    setMotorSpeed(MOTOR_LEFT, 0, FORWARD);
    setMotorSpeed(MOTOR_RIGHT, 0, FORWARD);
    _delay_ms(100);
}