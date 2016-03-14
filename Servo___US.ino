#include <LiquidCrystal.h>
#include <Servo.h>


int lcdPin[] = {22, 12, 11, 24, 10, 26};
LiquidCrystal lcd(22, 12, 11, 24, 10, 26);

int trigPin[] = {52, 50, 48, 46}; //blue
int echoPin[] = {53, 51, 49, 47}; //green
const int sensors = 4;   //how many sensors there are

char* sensorDebug[] =  {"Right", "Front", "Left", "Back"}; //for debuggin, to determine distances for each sensor

int buttonPin = 37;

bool buttonOn = false;
bool playing = false;

bool getButton(int b = buttonPin) {
  int state = digitalRead(b);
  if (state == HIGH)
    return true;
  else if (state == LOW)
    return false;
}

//SERVO!!!!!

Servo servo;        //initialize a servo object for the connected servo

int angle = 0;
int servoMid = 85;
int servoRight = 20;
int servoLeft = 150;

char* lastTurn;


//SERVO END!!!!

//DISTANCES

long dist[sensors];  //distance for each sensor at current step
long lastDist[sensors];   // distance of each sensor at moment before last step - used for calculations in current step
int sideWallDist = 90; //dist from side wall to side wall at start track
int sideWallDist2 = 45; //-------------||---------------- at middle track
int sideMargin = 1; //margin of error in dist
int distRightLeft = 20; //dist b/w left and right US - to find offset from center
int distFrontBack = 27; //dist b/w front and back US - to find offset from center

//POINTS

int startPoint; //calculated by rear US to calibrate position on map
int turningPoint = 60; //calculated possibly by starting point???
int pickupPoint = 10; //

//STEPS

//the script is run through steps. starts at step 1. when a step is completed, go to next step.
//the steps determine where in the course the robot is
int steps = 1;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  for (int i = 0; i < sensors; i++) {
    pinMode(echoPin[i], INPUT);
    pinMode(trigPin[i], OUTPUT);
    pinMode(lcdPin[i], OUTPUT);

  }

  //SERVO

  servo.attach(9);
  servo.write(servoMid);
  delay(1000);
}


void lcdCode(char* top, char* bottom) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(top);
  lcd.setCursor(0, 1);
  lcd.print(bottom);
  //delay(100);
}

void serialCode(char* string) {
  //Serial.println(string);
  delay(50);
}


void distCheck(int i) {

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:


/*

  digitalWrite(trigPin[i], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin[i], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin[i], LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.



  dist[i] = pulseIn(echoPin[i], HIGH) / 29 / 2;

  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the dist of the
  // object we take half of the dist travelled.

*/

}

void pause(){
  if(!buttonOn){
    playing ? playing = false : playing = true;
    buttonOn = true;
    Serial.print("press");
  }
}

void loop() {

  play();

  if(getButton())
    pause();
  else
    buttonOn = false;

  playing ? Serial.println("ON") : Serial.println("OFF");
  

  
  lcd.begin(16, 2);


  //GO TOWARDS FRONT WALL

  if (steps == 1) {

    //Engage both thrusters

    //Use right (0), front (1), and left (2)

    distCheck(0);
    distCheck(1);


    if (dist[0] <= sideWallDist / 2 - distRightLeft / 2 - sideMargin) {
      lcdCode("Too close right", "Engage right");
      serialCode("Too close right");


      servo.write(servoLeft);
      lastTurn = "left";

    }
    else if (dist[0] >= sideWallDist / 2 - distRightLeft / 2 + sideMargin) {
      lcdCode("Too close left", "Engage left");
      serialCode("Too close left");

      servo.write(servoRight);
    }
    else {
      lcdCode("Carry on", "");
      serialCode("Carry on");

      servo.write(servoMid);

      if (dist[1] < turningPoint) {
        for (int i = 0; i < sensors; i++) {
          distCheck(i);
          lastDist[i] = dist[i];
        }
        steps++;
        lcdCode("Turn Step", "");
        serialCode("Turn Step");
        delay(500);
      }

    }

  }
  //TURN 90 DEGREES


  else if (steps == 2) {
    //Engage Right thruster possibly reverse left


    distCheck(0);
    distCheck(1);


    lcdCode("Right thruster", "");
    serialCode("Right thruster");

    servo.write(servoLeft);

    delay(100);


    if (dist[0] <= lastDist[0] && dist[1] > lastDist[1] && dist[0] <= 2000 && dist[1] <= 2000) {
      lcdCode("Forward motion", "");
      serialCode("Forward motion");
      for (int i = 0; i < sensors; i++) {
        distCheck(i);
        lastDist[i] = dist[i];
      }
      delay(100);
      steps++;
    }

  }



  //GO TO CENTER OF COURSE


  else if (steps == 3) {
    //Engage both thrusters

    distCheck(0);

    if (dist[0] < lastDist[0] - sideMargin) {
      serialCode("Too close right");

      servo.write(servoLeft);

    }
    else if (dist[0] > lastDist[0] + sideMargin) {
      serialCode("Too close left");

      servo.write(servoRight);
    }
    else {
      serialCode("Carry on");

      servo.write(servoMid);

    }

    if (dist[1] == dist[3] + turningPoint - lastDist[0] || dist[1] == dist[3] + 1 + turningPoint - lastDist[0] || dist[1] == dist[3] - 1 + turningPoint - lastDist[0]) {
      lcdCode("Turn Step", "");
      serialCode("Turn Step");
      for (int i = 0; i < sensors; i++) {
        distCheck(i);
        lastDist[i] = dist[i];
      }
      delay(500);
      steps++;
    }

  }


  //TURN 90 DEGREES

  else if (steps == 4) {

    //INITIATE TURNING SEQUENCE

    distCheck(3);

    servo.write(servoLeft);

    if ((dist[3] + (distFrontBack / 2)) <= (lastDist[0] + (distRightLeft / 2))) {
      serialCode("Next Step");
      for (int i = 0; i < sensors; i++) {
        distCheck(i);
        lastDist[i] = dist[i];
      }
      steps++;
      delay(1000);
    }

  }

  //FORWARD UNTIL SIDE WALLS

  else if (steps == 5) {

    //Thruster initialization phase


    //DEBUG
    if (lastDist[3] == 0) {
      lastDist[3] = dist[3];
    }
    //END

    distCheck(3);

    if (dist[3] == turningPoint) {
      serialCode("COOL");
      for (int i = 0; i < sensors; i++) {
        distCheck(i);
        lastDist[i] = dist[i];
      }
      steps++;
      delay(3000);
    }
  }

  //FORWARD UNTIL PICKUP

  else if (steps == 6) {

    distCheck(0);
    distCheck(1);

    if (dist[0] < sideWallDist2 / 2 - distRightLeft / 2 - sideMargin) {
      lcdCode("Too close right", "Engage right");
      serialCode("Too close right");
    }
    else if (dist[0] > sideWallDist2 / 2 - distRightLeft / 2 + sideMargin) {
      lcdCode("Too close left", "Engage left");
      serialCode("Too close left");
    }
    else {
      lcdCode("Carry on", "");
      serialCode("Carry on");
    }

    if (dist[1] < pickupPoint) {
      //STOP
      Serial.println("HOLD UP");
      delay(1500);
      for (int i = 0; i < sensors; i++) {
        distCheck(i);
        lastDist[i] = dist[i];
      }
      steps++;
      delay(1500);
    }
  }

  //TURN 180 DEGREES

  else if (steps == 7) {

    distCheck(3);

    //ENGAGE RIGHT THRUSTER (POSSIBLY REVERSE LEFT)

    serialCode("Right thruster");

    if (dist[1] > 50 && dist[1] < 2000) {
      serialCode("Engage thrusters");
      steps++;
    }

  }

  //DEBUG DISTANCE
  else if (steps == 420) {

    servo.write(servoRight);


    for (int i = 0; i < sensors; i++) {
      distCheck(i);
      Serial.print(dist[i]);
      Serial.print(" ");
      Serial.print(sensorDebug[i]);
      Serial.print("    ");
    }
    Serial.println("");
    delay(200);
  }
  else if (steps == 421) {

    servo.write(servoMid);

    for (int i = 0; i < sensors; i++) {
      distCheck(i);
      Serial.print(dist[i]);
      Serial.print(" ");
      Serial.print(sensorDebug[i]);
      Serial.print("    ");
    }
    Serial.println("");
    delay(200);
  }

  //DEBUG DISTANCE
  else if (steps == 422) {

    servo.write(servoLeft);

    for (int i = 0; i < sensors; i++) {
      distCheck(i);
      Serial.print(dist[i]);
      Serial.print(" ");
      Serial.print(sensorDebug[i]);
      Serial.print("    ");
    }
    Serial.println("");
    delay(200);
  }

}


