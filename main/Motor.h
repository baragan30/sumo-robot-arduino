#ifndef MOTOR
#define MOTOR

#include <Arduino.h>

// Pins of left motor
#define leftP0 3
#define leftP1 11
// Pins of right motor
#define rightP0 5
#define rightP1 6

//After this number of direction the oposite motor it stats moving backward (see move function)
#define DIR_STAGE 60
#define PMAX 100  // procent maxim (see move function)

#define POW_MIN 100  
#define POW_MAX 255

enum Side       {RIGHT, LEFT};
enum Directions {AHEAD, BEHIND};

class Motor{
  private :
    int transposesSpeed(int procent);
    void moveWheels(int direction ,int speed,int P0, int P1);
  public :
  //initialize Motor
    void config();
    void forward();
    void backward();
    void forceLeft();
    void forceRight();
    void slowLeft();
    void slowRight();
    void left();
    void right();
    /*
    direction must be between(-PMAX and PMAx) (negative value for left and positive value for right)
    speed must be between(-PMAX and PMAX) (negative value for backward and positive value for forward)
    */
    void move(int direction, int speed);
    void stop();


};

#endif
