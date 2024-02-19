#include <Arduino.h>
#include "Driver.h"
#include "HardwareTester.h"

Driver driver;
HardwareTester tester;
Motor motor;
int mod = 1000;

void setup(){
//    tester.testMotorAdjustTimeRotate();
//    tester.testMotorShortcuts();
  //  tester.testMotorDirectionForward();
  //  tester.testEnemyDetection();-
//    tester.testLineDetector();
//    tester.testMotorDirectionMinimumspeed();
    Serial.begin(9600);

    driver.config(ENGAGE);

}


void loop(){
    driver.drive();
}
