#include "Motor.h"

void Motor::config(){
   digitalWrite(leftP0, 0);
   digitalWrite(leftP1, 0);
   digitalWrite(rightP0, 0);
   digitalWrite(rightP1, 0);
   
   pinMode (leftP0, OUTPUT);
   pinMode (leftP1, OUTPUT);
   pinMode (rightP0, OUTPUT);
   pinMode (rightP1, OUTPUT);
}

/*
 * Transform a number between 0 and PMAX in a number between POW_MIN and POWMAX
 * transposesSpeed(0) = 0
 * transposesSpeed(1) ~= POW_MIN    putin mai mare
 * transposesSpeed(PMAX) = POW_MAX
 */
int Motor::transposesSpeed(int procent){
  if (procent == 0) 
    return 0;
//    Serial.print(procent);
//    Serial.print(" ");
//    Serial.println( POW_MIN + procent*(POW_MAX - POW_MIN)/PMAX);
  return POW_MIN + procent*(POW_MAX - POW_MIN)/PMAX;
}

void Motor::moveWheels(int direction ,int speed,int P0, int P1){

      if (direction <= DIR_STAGE){
          speed = transposesSpeed( speed * (DIR_STAGE - direction) / DIR_STAGE );
         //f(0) = POW_MAX ,f(DIR_STAGE - 1)=POW_MIN, f(DIR_STAGE) = 0
         digitalWrite(P0, 0);
         analogWrite(P1,speed);
      }
      else{
        //f(DIR_STAGE) = 0 ,f(DIR_STAGE +1)= POW_MIN, f(PMAX) = POW_MAX
         speed = transposesSpeed( speed * (direction - DIR_STAGE) / (PMAX - DIR_STAGE) );
         digitalWrite(P1,0);
         analogWrite(P0,speed);
      }
  
}

void Motor::stop(){
   digitalWrite(leftP0, 0);
   digitalWrite(leftP1, 0);
   digitalWrite(rightP0, 0);
   digitalWrite(rightP1, 0);
}
void Motor::forward(){
   digitalWrite(leftP0, 0);
   digitalWrite(leftP1, 1);
   digitalWrite(rightP0, 0);
   digitalWrite(rightP1, 1);
}
void Motor::backward(){
   digitalWrite(leftP0, 1);
   digitalWrite(leftP1, 0);
   digitalWrite(rightP0, 1);
   digitalWrite(rightP1, 0);
}
void Motor::forceLeft(){
   digitalWrite(leftP0, 1);
   digitalWrite(leftP1, 0);
   digitalWrite(rightP0, 0);
   digitalWrite(rightP1, 1);
}
void Motor::forceRight(){
   digitalWrite(leftP0, 0);
   digitalWrite(leftP1, 1);
   digitalWrite(rightP0, 1);
   digitalWrite(rightP1, 0);
}

void Motor::left(){
    move(-60,100);
}
void Motor::right(){
    move(60,100);
}
void Motor::slowLeft(){
    move(-58,100);
}
void Motor::slowRight(){
    move(58,100);
}
void Motor::move(int direction, int speed){

    if (speed >= 0){
      if(direction >= 0){//to right
        digitalWrite(leftP0, 0);
        analogWrite(leftP1, transposesSpeed(speed)); //max power at left engine
        moveWheels(direction,speed,rightP0,rightP1);
      }else{
        digitalWrite(rightP0, 0);
        analogWrite(rightP1, transposesSpeed(speed)); //max power at right engine
        moveWheels(-1*direction,speed,leftP0,leftP1);//positive direction
      }
    }else{
      speed *=-1;//pozitive direction
      if(direction >= 0){//to right
        digitalWrite(leftP1, 0);
        analogWrite(leftP0, transposesSpeed(speed)); //max power at left engine
        moveWheels(direction,speed,rightP1,rightP0);
      }else{
        digitalWrite(rightP1, 0);
        analogWrite(rightP0, transposesSpeed(-speed)); //max power at left engine
        moveWheels(-direction,speed,leftP1,leftP0);//pozitive direction
      }
    }
      
    
}
