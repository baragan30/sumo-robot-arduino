#include "HardwareTester.h"
#include "Servo.h"
#include "Actions.h"


[[noreturn]] void HardwareTester::testMotorDirectionForwardBackward(){
    motor.config();
    Serial.begin(9600);
    while(true){
        for (int direction = -100 ; direction <= 100; direction+=20){
            for (int speed = 100 ;speed!=-300 ; speed -=200){
                Serial.print("d = ");
                Serial.print(direction);
                Serial.print("   s = ");
                Serial.println(speed);
                motor.move(direction , speed);
                delay(1000);
                motor.stop();
                Serial.println("-------------------------------------------------");
                delay(1000);
            }
        }
    }
}

[[noreturn]] void HardwareTester::testMotorShortcuts(){
    Motor motor;
    motor.config();
    int d = 3000;
    int d2 = 1000;
    while(true){
        motor.forceRight();
        delay(d);
        motor.stop();
        delay(d2);
        motor.slowRight();
        delay(d);
        motor.stop();
        delay(d2);
        motor.forward();
        delay(d);
        motor.stop();
        delay(d2);
        motor.backward();
        delay(d);
        motor.stop();
        delay(d2);
        motor.slowLeft();
        delay(d);
        motor.stop();
        delay(d2);
        motor.forceLeft();
        delay(d);
        motor.stop();
        delay(d2);

    }
}

[[noreturn]] void HardwareTester::testMotorDirectionForward(){
    motor.config();
    Serial.begin(9600);
    int speed = 100;
    int d = 100;
    Servo servo;
    servo.attach(A1);
    while(true){
        for (int direction = -100 ; direction < 100; direction+=1){
            Serial.print("d = ");
            Serial.println(direction);
            motor.move(direction , speed);
            delay(d);
        }
        for (int direction = 100 ; direction > -100; direction-=1){
            Serial.print("d = ");
            Serial.println(direction);
            motor.move(direction , speed);
            delay(d);
        }
        servo.write(9);

    }
}
[[noreturn]] void HardwareTester::testMotorAdjustTimeRotate(){

    //1500 seems perfect
    motor.config();
    Serial.begin(9600);
    while(true){
        for(int time = 1300; time <= 1600; time +=100){
            motor.move(100,100);
            delay(time);
            Serial.println(time);
            motor.stop();
            delay(1000);
        }
    }
}

[[noreturn]] void HardwareTester::testMotorSpeed(){
    motor.config();
    Serial.begin(9600);
    while(1){
        for (int speed = -100 ; speed <= 100; speed+=10){
            motor.move(0,speed);
            Serial.println(speed);
            delay(100);
            motor.stop();
            delay(200);

        }
        for (int speed = 100 ; speed >= -100; speed-=10){
            motor.move(0,speed);
            Serial.println(speed);
            delay(100);
            motor.stop();
            delay(200);
        }
    }
}

[[noreturn]] void HardwareTester::testMotorDirectionMinimumspeed(){
    motor.config();
    Serial.begin(9600);
    while(true){
        for (int direction = -100 ; direction <= 100; direction+=10){
            motor.move(direction,1);
            Serial.println(direction);
            delay(1000);
            motor.stop();
            delay(500);
        }
        for (int direction = 100 ; direction >= -100; direction-=10){
            motor.move(direction,1);
            Serial.println(direction);
            delay(1000);
            motor.stop();
            delay(500);
        }
    }
}

[[noreturn]] void HardwareTester::testMotorMinimumSpeedBackwardForward(){
    motor.config();
    Serial.begin(9600);
    while(true){
        motor.move(0,1);
        delay(1000);
        motor.stop();
        delay(500);
        motor.move(0,-1);
        delay(1000);
        motor.stop();
        delay(500);
    }
}

[[noreturn]] void HardwareTester::testObstaclesDetector(){
    Serial.begin(9600);
    obsDec.config();
    obsDec.setState(SCAN);
    long clock = millis();
    int i = 0 ;
    ObstaclaDetectorZone obs[4] = {FRONT_ZONE,EXTREME_LEFT_ZONE,EXTREM_RIGHT_ZONE,ALL_ZONE};
    obsDec.setState(SCAN,obs[i]);
    while(true){
//        if(obsDec.getPosition() == 0){
//
//        }
        if(millis() - clock  > 4000){
            clock = millis();
            obsDec.setState(SCAN,obs[i]);
            i = (i + 1 ) % 4;
            obsDec.printMeasurements();
        }
        obsDec.detect();
    }
}

[[noreturn]] void HardwareTester::testEnemyDetection() {
    Serial.begin(9600);
    obsDec.config();
    obsDec.setState(SCAN);
    long clock = millis();
    obsDec.setState(ENEMY_DETECTION);
    while(true){
        if(millis() - clock  > 1000){
            obsDec.printEnemyInfo();
            clock = millis();
        }
        obsDec.detect();
    }
}

[[noreturn]] void HardwareTester::testLineDetector(){
    Serial.begin(9600);
    lineDec.config();
    while(true){
        lineDec.read();
        lineDec.printLine();
        delay(200);
    }
}

[[noreturn]] void HardwareTester::testAction(){
    Serial.begin(9600);
    StackAction stack;
    for(int i = 0 ; i < 20 ; i ++){
        auto *action = new Action_Clock(100,100,i);
        action->print();

        if(!stack.push(action)){
            delete action;
        }
    }
    delay(1000);
    while(!stack.isEmpty()){
        Action_Clock*action;
        stack.pop(&action);
        action->reverse();
        action->print();
    }
    while(true);
}
