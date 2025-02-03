#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>
#include <util/delay.h>

#define FORWARD 1
#define BACKWARD -1
#define MOTOR_TOP_SPEED 250
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

// Pin definitions
#define LEFT_CONTROL PD2  // pin 2
#define LEFT_PWM PD5      // pin 5
#define RIGHT_CONTROL PD4 // pin 4
#define RIGHT_PWM PD6     // pin 6

void setupMotors(void);
void setMotorSpeed(int motor, double speed, int direction);
void moveForward(void);
void moveBackward(void);
void turnLeft(void);
void turnRight(void);
void stopMotors(void);

#endif