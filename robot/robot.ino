const int IN1 = 11;
const int IN2 = 9;
const int IN3 = 8;
const int IN4 = 7;
const int ENA = 6;
const int ENB = 5; 
const int SensorRight = 10;     
const int SensorLeft = 2;

const int SPEED = 220;
const int TIME_TURN = 560;
const int TIME_FORWARD = 1500;
const int DISTANCE_STOP = 30;

int Echo = A4;
int Trig = A5;
int Distance = 0;

void setup(){
  Serial.begin(115200);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode(SensorRight, INPUT); 
  pinMode(SensorLeft, INPUT); 
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

void run(){
  if(mesure_Distance() < DISTANCE_STOP){
    brake();
  }else{
    analogWrite(ENA, SPEED);
    analogWrite(ENB, SPEED);
    forward();
    
    if(digitalRead(SensorLeft)==1 && digitalRead(SensorRight)==0){ //Tourne vers la droite
      turn_right();
    }
    if(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==0){ //Tourne vers la gauche
      turn_left();
    }
    if(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==1){  //Demi tour
      back();
    }
  }
}

void back(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void brake(){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turn_left(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turn_left_90(){
  turn_left();
  delay(TIME_TURN);
}

void turn_right(){
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turn_right_90(){
  turn_right();
  delay(TIME_TURN);
}

float mesure_Distance(){
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float distance = pulseIn(Echo, HIGH);
  distance = distance/58;
  return distance;
}

int done = 1; 

void loop(){
  //run();
  /*if(done){
     forward();
     delay(TIME_FORWARD);
     turn_right();
     forward();
     delay(TIME_FORWARD);
     turn_left();
     forward();
     delay(TIME_FORWARD);
     brake();
     done = 0;
  }*/
}
