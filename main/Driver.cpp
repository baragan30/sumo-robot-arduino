#include "Driver.h"

#define MAX_EXPLORE_TIME 7000
#define MAX_ROTATE_TIME 250
#define safety_to_speed(x) (((int)(x+ SPEED_RESOLUTION - 1))/SPEED_RESOLUTION*SPEED_RESOLUTION)

void Driver::config(State state){
  lineDec.config();
  motor.config();
  obsDec.config();
  this->state = ENGAGE;
  switch (state)
  {
  case ENGAGE:
    obsDec.setState(ENEMY_DETECTION);
    break;
  case ESCAPE_LINE:
    obsDec.setState(ENEMY_DETECTION);
    break;
  default:
    break;
  }
  actionClock = NULL;
}

//These function will find the opponent and will try to take him out of the ring
//If it meets the end of the ring, it will go into escape line mode
void Driver::engage() {
    lineDec.read();
    if(lineDec.isInLine()){
        motor.backward();
        state = ESCAPE_LINE;
        return;
     }

    obsDec.detect();
    const int engageDistance = 7;
   
    int enemyPosition = obsDec.enemyPosition;//angle of enemy
    switch (enemyPosition) {
        case 0 ... 39:                 //Force Right
            motor.forceRight();
            break;
        case 40 ... 69:                //Slow Right
            if(obsDec.enemyDistance <= engageDistance)
                motor.slowRight();
            else
                motor.right();
            break;
        case 70 ... 110:
            motor.forward();
            break;
        case 111 ... 140:             //Force Left
            if(obsDec.enemyDistance <= engageDistance)
               motor.slowLeft();
            else
                motor.left();
            break;
        case 141 ... 180:             // Slow Left
            motor.forceLeft();
            break;
        default:                      //-1
            motor.move(0,40);//move slow forward
            break;
    }
}


void Driver::escapeLine() {
    lineDec.read();
    if(actionClock != NULL ){
        if(actionClock->isDone()){
            if(actionClock->speed == -100){
                int linPos = lineDec.relativePosition;
                stopCurentAction();
                
                switch (linPos) {
                    case -4 :
                        setNewAction(turn120around,-100,100);
                        obsDec.move(180);
                        break;
                    case -3 ... -1:
                        setNewAction(turn150around,-100,100);
                        obsDec.move(180);
                        break;
                    case 0:
                        setNewAction(turn165around,100,100);
                        obsDec.move(0);
                    case 1 ... 3:
                        setNewAction(turn150around,100,100);
                        obsDec.move(0);
                        break;
                    case 4:
                        setNewAction(turn120around,100,100);
                        obsDec.move(0);
                        break;
                    default:
                        setNewAction(turn180around,100,100);
                        obsDec.move(0);
                        break;
                }
                return;
            }
            if(actionClock->direction != 0 ){
                stopCurentAction();
                setNewAction(200,0,100);//move forward
                return;
            }

            if(!lineDec.isInLine()){
                stopCurentAction();
                state = ENGAGE;
                return;
            }
        }else{
            if(actionClock->direction == 0 && actionClock->speed == 100)
                obsDec.detect();
        }
    }else{//if we don't have an action set
        setNewAction(200,0,-100);
    }

}

void Driver::drive(){


      switch(state){
      case ENGAGE:
          engage();
          break;
      case ESCAPE_LINE:
          escapeLine();
          break;
        default://STOP
         motor.stop();
         break;
      };
}


void Driver::print() {
    Serial.print("state = ");
    Serial.print(state);
    Serial.print(" ,actionState =  ");

    Serial.println(actionState);
    Serial.print("safety = ");
    Serial.print(obsDec.getSafety());
    Serial.print(" ,line = ");
    lineDec.printLine();

    Serial.print(" ,stackTime = ");
    Serial.print(stack.getPassedTime());
    Serial.print(" ,stackIndex = ");
    Serial.println(stack.index);
    Serial.println();

}
bool Driver::setNewAction(long time, int direction, int speed) {
    actionClock = new Action_Clock(time,direction,speed);
    motor.move(actionClock->direction,actionClock->speed);
    return true;
}

void  Driver::stopCurentAction() {
    motor.stop();
    if (actionClock != NULL){
        delete actionClock;
        actionClock = NULL;
    }
}

