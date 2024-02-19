## SumoRoboticToy (College Project)
#### Sumo Robotic Toy is a robot that remove objects from a ring. The robot adjusts its speed and direction, decides in which direction to search for objects and checks that it has not left the ring, all at the same time

### Here You can see what this software does :

https://www.youtube.com/shorts/d9uUJHC6v_s

https://www.youtube.com/shorts/YJgPBfXASk8

#### This software is written for Arduino Uno board
The robot electronic componnents are :
- Arduino Uno
- 5 Channel Line Tracking Mode with bumper, TRCR5000
- Servomotor SG90 180 degree
- Ultrasonic sensor module - distance detector HC-SR04
- double H-bridge L298N V1
- 4x 6V DC motors

#### The software is structured as follow :
- Motor : hardware abstraction for 4 motors + H-bridge (allows the developer to go in any direction and at any speed).
- LineDetector : hardware abstraction for TRCR5000 (allows the developer see if the robot it's hitten and  where is the line if it's under robot.
- Obstacle Detector : hardware abstaction for servomotor and ultrasonic sensor. Allow developer to see where are the obstacles and how how far are them.
- Driver : use all software components and control robot states and actions.
- Action : allows the developer to initiate a move action that has a direction, speed, and time, without stopping the robot from doing other things like looking for obstacles.
- HardwareTester : allow the developer to test easy if all electronic components are connected and functionate properly.
