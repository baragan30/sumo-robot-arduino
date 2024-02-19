#ifndef DRIVER_H
#define DRIVER_H

#include "ObstaclesDetector.h"
#include "Motor.h"
#include "LineDetector.h"
#include "Arduino.h"
#include "Actions.h"

#define MAX_EXPLORE_TIME 7000
#define MAX_ROTATE_TIME 250
#define SPEED_RESOLUTION 10

enum State {STOP,EXPLORE,INLINE,ENGAGE,ESCAPE_LINE};



class Driver{
  private:

    static const int turn360around = 1500;
    static const int turn180around = turn360around/2;
    static const int turn165around = turn360around*11/24;
    static const int turn150around = turn360around/2;
    static const int turn120around = turn360around/3;
    static const int turn90around = turn360around/4;
    static const int turn60around = turn360around/6;
    static const int turn45around = turn360around/8;
    static const int turn30around = turn360around/12;
    LineDetector lineDec;
    ObstaclaDetectorZone preferedZone ;
    ObstaclesDetector obsDec;
    Motor motor;
    StackAction stack;
    State state;
    Action actionState;
    Action_Clock * actionClock ;

    void explore();
    void stopCurentAction();
    void engage();
    void escapeLine();
    bool setNewAction(long time, int direction, int speed);
    bool setAndPushNewAction(long time, int direction, int speed);

  public:
    void config(State state);
    void drive();
    void print();


    void epilepsie();
};

#endif
