#include "motor.h"
#include "ultrasonic.h"
#include "pump.h"

#define APPROACH_SPEED 0.3
#define SAFE_DISTANCE 12

int8_t getFireDirection(void) {
    uint16_t min_value = 1023;
    int8_t direction = -1;
    
    if(sensor_values[FIRE_SENSOR_RIGHT] < FIRE_THRESHOLD && 
        sensor_values[FIRE_SENSOR_RIGHT] < sensor_values[FIRE_SENSOR_LEFT]) {
        direction = FIRE_SENSOR_RIGHT;
    } 
    else if(sensor_values[FIRE_SENSOR_LEFT] < FIRE_THRESHOLD && 
            sensor_values[FIRE_SENSOR_LEFT] < sensor_values[FIRE_SENSOR_RIGHT]) {
        direction = FIRE_SENSOR_LEFT;
    }
    
    return direction;
}

void adjustRobotDirection(int8_t fireDirection) {
    stopMotors();
    
    switch(fireDirection) {
        case FIRE_SENSOR_RIGHT:
            // Small adjustment for right sensor
            setMotorSpeed(MOTOR_LEFT, 0.2, FORWARD);
            setMotorSpeed(MOTOR_RIGHT, 0.1, BACKWARD);
            _delay_ms(100);  // Short adjustment time
            break;
            
        case FIRE_SENSOR_LEFT:
            // Small adjustment for left sensor
            setMotorSpeed(MOTOR_LEFT, 0.1, BACKWARD);
            setMotorSpeed(MOTOR_RIGHT, 0.2, FORWARD);
            _delay_ms(100);  // Short adjustment time
            break;
    }
    stopMotors();
}

void handleFireDetection(void) {
    stopMotors();
    setPumpState(PUMP_ON);
    
    int8_t fireDirection = getFireDirection();
    if(fireDirection == -1) {
        setPumpState(PUMP_OFF);
        return;
    }
    
    adjustRobotDirection(fireDirection);
    
    // Stay in position and fight fire
    uint8_t fireOutCount = 0;
    while(fireOutCount < 100) {  // Check for 1 second of no fire
        if(getFireDirection() == -1) {
            fireOutCount++;
        } else {
            fireOutCount = 0;
            // Small readjustment if fire moves
            int8_t newDirection = getFireDirection();
            if(newDirection != fireDirection && newDirection != -1) {
                adjustRobotDirection(newDirection);
                fireDirection = newDirection;
            }
        }
        _delay_ms(10);
    }
    
    setPumpState(PUMP_OFF);
}

void checkAndAvoidObstacle(void) {
    uint16_t front_distance = measureDistance();
    uint8_t left_blocked = sensor_values[IR_SENSOR_LEFT] < IR_THRESHOLD;
    uint8_t right_blocked = sensor_values[IR_SENSOR_RIGHT] < IR_THRESHOLD;
    
    if(front_distance < SAFE_DISTANCE) {
        stopMotors();
        moveBackward();
        _delay_ms(500);
        
        moveServo(SERVO_LEFT);
        _delay_ms(200);
        uint16_t leftDistance = measureDistance();
        
        moveServo(SERVO_RIGHT);
        _delay_ms(200);
        uint16_t rightDistance = measureDistance();
        
        moveServo(SERVO_CENTER);
        
        if(left_blocked && right_blocked) {
            turnLeft();
            turnLeft();
        }
        else if(left_blocked || leftDistance < SAFE_DISTANCE) {
            turnRight();
        }
        else if(right_blocked || rightDistance < SAFE_DISTANCE) {
            turnLeft();
        }
        else if(leftDistance > rightDistance) {
            turnLeft();
        }
        else {
            turnRight();
        }
    }

    else if(left_blocked || right_blocked) {
        if(left_blocked && right_blocked) {
            moveBackward();
            _delay_ms(300);
            turnLeft();
            turnLeft();
        }
        else if(left_blocked) {
            setMotorSpeed(MOTOR_LEFT, 0.3, FORWARD);
            setMotorSpeed(MOTOR_RIGHT, 0.1, FORWARD);
        }
        else if(right_blocked) {
            setMotorSpeed(MOTOR_LEFT, 0.1, FORWARD);
            setMotorSpeed(MOTOR_RIGHT, 0.3, FORWARD);
        }
    }
    else {
        moveForward();
    }
}

int main(void) {
    cli();
    
    setupMotors();
    setupTimer1();
    setupServo();
    setupUltrasonic();
    setupSensors();
    setupPump();
    
    sei();
    
    setPumpState(PUMP_OFF);
    moveServo(SERVO_CENTER);
    _delay_ms(1000);
    
    while(1) {
        if(fire_detected) {
            handleFireDetection();
            fire_detected = 0;
            obstacle_detected = 0; 
        } else {
            checkAndAvoidObstacle();
        }
        
        _delay_ms(10);
    }
    
    return 0;
}