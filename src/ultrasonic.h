#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERVO_PIN   PB2    // OC1B - Pin 10
#define TRIGGER_PIN PB4    // Pin 12
#define ECHO_PIN    PB5    // Pin 13

#define SERVO_RIGHT  850   
#define SERVO_CENTER 2650  
#define SERVO_LEFT   4700  

// Sensor arrangement
#define IR_SENSOR_RIGHT     0   // A0 - Right IR sensor
#define FIRE_SENSOR_RIGHT   1   // A1 - Right fire sensor
#define FIRE_SENSOR_LEFT    2   // A2 - Left fire sensor
#define IR_SENSOR_LEFT      3   // A3 - Left IR sensor

// Detection Thresholds
#define FIRE_THRESHOLD      350  // For fire sensors
#define IR_THRESHOLD       100  // For IR sensors - lower means obstacle detected
#define FIRE_STOP_THRESHOLD 150

extern volatile uint16_t sensor_values[4];  // 4 sensors
extern volatile uint8_t adc_channel;
extern volatile uint8_t fire_detected;
extern volatile uint8_t obstacle_detected;

void setupTimer1(void);
void setupServo(void);
void setupUltrasonic(void);
void setupSensors(void);
uint16_t measureDistance(void);
void moveServo(uint16_t position);

#endif