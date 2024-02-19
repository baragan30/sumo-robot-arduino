#include "ObstaclesDetector.h"

//Read the distance from ultrasonic to the closest object in cm

void ObstaclesDetector::config(){
    myservo.attach(SERVO_PIN);
    digitalWrite(SONIC_TRIGG, LOW);
    pinMode(SONIC_ECHO,INPUT);
    pinMode(SONIC_TRIGG,OUTPUT);
    myservo.write(90);
    action.reset(0,0,0);
    degreeMin = 0;
    degreeMax = 0;

}
void ObstaclesDetector::setState(ObstaclesDetectorState state){
    this->state = state;
    switch (state) {
        case ENEMY_DETECTION:
            action.reset(0,SPEED_FIND_ENEMY,SG90_MILISECONDS_GRAD);
            break;
        default:
            break;
    }
}

void ObstaclesDetector::move(int position) {
    myservo.write(position);
}
ObstaclesDetectorState ObstaclesDetector::getState() {
    return state;
}
void ObstaclesDetector::setState(ObstaclesDetectorState st, ObstaclaDetectorZone zone){
    this->state = st;
    switch (zone) {
        case ALL_ZONE:
            degreeMin = ALL_ZONE_MIN;
            degreeMax = ALL_ZONE_MAX;
            break;
        case EXTREME_LEFT_ZONE:
            degreeMin = EXTREME_LEFT_ZONE_MIN;
            degreeMax = EXTREME_LEFT_ZONE_MAX;
            break;
        case EXTREM_RIGHT_ZONE:
            degreeMin = EXTREME_RIGHT_ZONE_MIN;
            degreeMax = EXTREME_RIGHT_ZONE_MAX;
            break;
        case FRONT_ZONE:
            degreeMin = FRONT_ZONE_MIN;
            degreeMax = FRONT_ZONE_MAX;
            break;
        default:
            degreeMin = 90;
            degreeMax = 90;
            break;
    }
    for(int i = 0 ; i < NR_MEASUREMENTS; i ++){
        if (degree_measure[i] >= degreeMin && degree_measure[i] <= degreeMax){
            measurements[i] = -1;
        }
    }
    safety = -1;
}
int ObstaclesDetector::getSafety() {
    return safety;
}
void ObstaclesDetector::detect(){
    if (action.isUnfolding())  // stop
        return;
    switch (state) {
        case SCAN:      //SCAN
            scan();
            break;
        case ENEMY_DETECTION:
            scanForEnemy();
            break;
        default:   //IDLE
            break;
    }
}
void ObstaclesDetector::printEnemyInfo() {
    Serial.print(enemyPosition);
    Serial.print(" ");
    Serial.println(enemyDistance);
}

void ObstaclesDetector::printMeasurements(){
    Serial.print(state);
    Serial.print(" ");
    Serial.print(degreeMin);
    Serial.print(" ");
    Serial.println(degreeMax);
    for(int i = 0 ; i < NR_MEASUREMENTS; i ++){
        Serial.print(degree_measure[i]);
        if (degree_measure[i] < 10)
            Serial.print("   ");
        else if (degree_measure[i] < 100)
            Serial.print("  ");
        else
            Serial.print(" ");
    }
    Serial.println();
    for(int i = 0 ; i < NR_MEASUREMENTS; i ++){
        Serial.print(measurements[i]);
        if (measurements[i] < 10)
            Serial.print("   ");
        else if (measurements[i] < 100){
            Serial.print("  ");
        }else{
            Serial.print(" ");
        }

    }
    Serial.println();
    for(int i = 0 ; i < NR_MEASUREMENTS; i ++){
        int res = computeSafety(i);
        Serial.print(res);
        if (res < 10)
            Serial.print("   ");
        else if (res < 100){
            Serial.print("  ");
        }else{
            Serial.print(" ");
        }
    }
    Serial.println();
    Serial.println(safety);
    Serial.println();
}



// -------------- PRIVATE FUNCTIONS -------------------------------

void ObstaclesDetector::scan (){
    int curentDegrees = myservo.read();
    int index = getIndex();

//    if is in position of measuring
    if(index != -1){
//        measure and  update safety and delay time
        measurements[index] = ultrasonicRead();
        computeSafety();
        action.speed = computeDelayTime();
    }
//    Set actionState.direction
    if (action.direction == 0){
        action.reset(0,1,computeDelayTime());
    }else{
        int nextDegree =curentDegrees + action.direction;
        if(nextDegree < degreeMin ) {
            action.direction  = 1;
        }
        else if (nextDegree > degreeMax ){
            action.direction = -1;
        }
        myservo.write(nextDegree);
        action.restart(delayTime);
    }
}

void ObstaclesDetector::actualizeEnemyPosition(int pos, int dist) {
    if(pos == enemyPosition){
        enemyDistance = dist;
    }else{
        if(dist <= enemyDistance  ) {

            if(dist > MAX_ENEMY_DISTANCE) {
                enemyPosition = -1;
                degreeMin = 0;
                degreeMax = 180;
                return;
            }
            else{
                enemyPosition = pos;
                enemyDistance = dist;
            }

        }
    }
//    if pos != -1
    int deviation = max(MIN_DEVIATION,MAX_DEVIATION * (float)(enemyDistance-MIN_ENEMY_DISTANCE) / (MAX_ENEMY_DISTANCE-MIN_ENEMY_DISTANCE));
    degreeMin = max(0,enemyPosition - deviation);
    degreeMax = min(180,enemyPosition + deviation);

}
void ObstaclesDetector::scanForEnemy(){
    int curentDegrees = myservo.read();
    int distance = ultrasonicRead();
    actualizeEnemyPosition(curentDegrees,distance);

    int nextDegree = curentDegrees + action.direction;
    if(nextDegree <= degreeMin ) {
        action.direction  = SPEED_FIND_ENEMY;
    }else
    if (nextDegree >= degreeMax ){
        action.direction = -SPEED_FIND_ENEMY;
    }
    myservo.write(nextDegree);
    action.restart(SPEED_FIND_ENEMY*SG90_MILISECONDS_GRAD);
}

int ObstaclesDetector::computeSafety() {
    int minSafety = 100;
    for (int i = 0 ; i < NR_MEASUREMENTS; i ++ ){
        int measure = computeSafety(i);
        if(measure < minSafety)
            minSafety = measure;
    }
    safety = minSafety;
    return safety;
}
int ObstaclesDetector::computeSafety(int i) {
    double lenght = measurements[i];
    if(lenght == -1)
        return -1;
    //degree is always between 0 and 90
    if(degree_measure[i] < degreeMin || degree_measure[i] > degreeMax){
        return 100;
    }
    if (degree_measure[i] > 90){
        i = NR_MEASUREMENTS - i-1;
    }
    if(lenght <= CRITICAL_DISTANCES[i] )
        return 0;
    if (lenght >= SAFETY_DISTANCES[i])
        return 100;
    return (int) (100*(lenght-CRITICAL_DISTANCES[i]) /(SAFETY_DISTANCES[i]-CRITICAL_DISTANCES[i]) );
}
int ObstaclesDetector::getIndex(){
    for(int i = 0 ; i < NR_MEASUREMENTS; i ++){
        int g = myservo.read();
        if(degree_measure[i] == g){
            return i;
        }
    }
    return -1;
}
int ObstaclesDetector::computeDelayTime() {
    if(safety == -1 ){
        delayTime = (int)(SG90_MILISECONDS_GRAD + (float)(SG90_MILISECONDS_GRAD)*50*MAX_X_CLOCK_SERVO/100.0);
        return delayTime;
    }
    delayTime = (int)(SG90_MILISECONDS_GRAD + (float)(SG90_MILISECONDS_GRAD)*safety*MAX_X_CLOCK_SERVO/100.0);
    return delayTime;
}
//-----------Ultrasonic functions ------------
int ObstaclesDetector::ultrasonicRead() {
    digitalWrite(SONIC_TRIGG, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONIC_TRIGG, LOW);
    long duration = pulseIn(SONIC_ECHO, HIGH);//miliseconds
    return (int) (duration * SPEED_OF_SOUND / 20000);
}
