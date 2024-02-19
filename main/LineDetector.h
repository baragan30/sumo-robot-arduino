#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <Arduino.h>

#define S1_PIN A0
#define S2_PIN A1
#define S3_PIN A2
#define S4_PIN A3
#define S5_PIN A4
#define CLP_PIN A5

class LineDetector{
  private:
    int line;
  public:
  //initialize Line Detector
    void config();
  //must be read as often as possible
    int read();
  //if the robot is Hitten (front sensor)
    int isHitten();
  //return last readed output  of 5 line sensour
    int isInLine();
    void printLine();
    int relativePosition = 100;
  
};
#endif
