//PID
#include <PID_v1.h>

#include <Servo.h>

Servo myservo;

double Pk1 = 6; 
double Ik1 = 0;
double Dk1 = 0;

double Setpoint1, Input1, Output1, Output1a;    // PID variables
PID PID1(&Input1, &Output1, &Setpoint1, Pk1, Ik1 , Dk1, DIRECT);    // PID Setup

int pot;
int demand = 0;
int velocityDemand = 0;
int command = 1;
int flag = 0;
int end1 = 0;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long previousMotorMillis = 0;    // set up timers
long interval = 10;        // time constant for timer

void setup() {

  pinMode(A0, INPUT);   // steering feedback pot
  pinMode(5, OUTPUT);   // pwms - steering
  pinMode(6, OUTPUT);   // pwms - steering
  Serial.begin(115200);

  PID1.SetMode(AUTOMATIC);              
  PID1.SetOutputLimits(-255, 255);
  PID1.SetSampleTime(10);

  myservo.attach(3);
  myservo.writeMicroseconds(1500);

}

void loop() {

    currentMillis = millis();
    if (currentMillis - previousMillis >= 10) {  // start timed event

        previousMillis = currentMillis;

        if (Serial.available() >0) {
            command = Serial.read() - '0';       // read command                           
            } 
        
        if (command == 1) {  //stop
          velocityDemand = 1500;
          command = 0;
        }
        else if (command == 2) {  // forwards
          velocityDemand = 1550;  
          command = 0;
        }
        else if (command == 3) {  //faster
          velocityDemand = velocityDemand + 50;
          command = 0;
        }
        else if (command == 4) {  //slower
          velocityDemand = velocityDemand - 50;
          command = 0;
        }
        else if (command == 5) {  //left
            demand = 30;
            flag = 1;
            command = 0;

        }
        else if (command == 6) {  //right
            demand = -30;
            flag = 2;
            command = 0;

        }
        else if (command == 7) {  //straight
             demand = 0;
             flag = 0;
             command = 0;

        }
        else if (command == 8) {  //less
            if (flag == 1) { // turning left
              demand = demand - 30;   // turn left some more
            }
            else if (flag == 2) { // turning right
              demand = demand + 30;   // turn right some more
            }
            command = 0;

        }
        else if (command == 9) {  //more
            if (flag == 1) { // turning left
              demand = demand + 30;   // turn left less
            }
            else if (flag == 2) { // turning right
              demand = demand - 30;   // turn right some more
            }
            command = 0;

        }

        myservo.writeMicroseconds(velocityDemand);
  
        pot = analogRead(A0);
        pot = pot - 717;        // get centre point of pot

        demand = constrain(demand,-150,150);    
        Setpoint1 = demand;
                
        Input1 = pot;
        PID1.Compute();

        if (Output1 < 30 && Output1 > -30) {
          previousMotorMillis = currentMillis;
        }

        if (currentMillis - previousMotorMillis > 50) {    // operate normally
             
            if (Output1 > 0) {
              analogWrite(5, Output1);
              analogWrite(6, 0);
            }
            else if (Output1 < 0) {
              Output1a = abs(Output1);
              analogWrite(6, Output1a);
              analogWrite(5, 0);
            }
            else {
              analogWrite(6, 0);
              analogWrite(5, 0);
            }
            
        }

        else {                                              // completely shut down the motor
              analogWrite(6, 0);
              analogWrite(5, 0);      
        }

    }  // end of timed loop

} // end of main loop




