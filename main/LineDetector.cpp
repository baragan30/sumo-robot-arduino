#include "LineDetector.h"

#define CLP ((int) 0x000020)
void LineDetector::config(){
  pinMode(S1_PIN,INPUT);
  pinMode(S2_PIN,INPUT);
  pinMode(S3_PIN,INPUT);
  pinMode(S4_PIN,INPUT);
  pinMode(S5_PIN,INPUT);
  pinMode(CLP_PIN,INPUT);
}
 

int LineDetector::read(){
  int clp = digitalRead(CLP_PIN);
  int s1 = !digitalRead(S1_PIN);// 1 if hit the line, 0 if not
  int s2 = !digitalRead(S2_PIN);
  int s3 = !digitalRead(S3_PIN);
  int s4 = !digitalRead(S4_PIN);
  int s5 = !digitalRead(S5_PIN);
  line = ((clp <<5) | (s1 <<4) | (s2<<3) | (s3 << 2) | (s4 <<1 ) | s5) & 0x00003F;

    if(isInLine()) {
        int nr = s1+ s2 + s3 + s4 + s5;
//        if(relativePosition == 100)
                relativePosition = ((s1-s5)*4 + (s2-s4)*2)/nr;
    }else{
        relativePosition = 100;
    }
  return line;
}
void LineDetector::printLine(){
    Serial.print("line = ");
    Serial.print(line);
    Serial.print(", relativePosition = ");
    Serial.print(relativePosition);
    Serial.print(", isInLine = ");
    int is = isInLine();
    Serial.println(is);
}

int LineDetector::isHitten(){
  return line & CLP;
}
int LineDetector::isInLine(){
  return line & (~CLP);
}
