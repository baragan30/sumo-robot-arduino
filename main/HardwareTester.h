#ifndef HARDWARE_TESTER_H
#define HARDWARE_TESTER_H
#include <Arduino.h>
#include "ObstaclesDetector.h"
#include "Motor.h"
#include "LineDetector.h"

class HardwareTester{
private:
    LineDetector lineDec;
    ObstaclesDetector obsDec;
    Motor motor;
public:

    [[noreturn]] void testMotorShortcuts();
    [[noreturn]] void testMotorDirectionForwardBackward();


    [[noreturn]] void testMotorDirectionForward();
    [[noreturn]] void testMotorAdjustTimeRotate();

    [[noreturn]] void testMotorSpeed();

    [[noreturn]] void testMotorDirectionMinimumspeed();

    [[noreturn]] void testMotorMinimumSpeedBackwardForward();

    [[noreturn]] void testObstaclesDetector();

    [[noreturn]] void testEnemyDetection();

    [[noreturn]] void testLineDetector();

    [[noreturn]] void testAction();


};

#endif
