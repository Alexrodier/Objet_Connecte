const int IN1 = 11;
const int IN2 = 9;
const int IN3 = 8;
const int IN4 = 7;
const int ENA = 6;
const int ENB = 5; 
const int SensorRight = 10;     
const int SensorLeft = 2;

const int SPEED = 220;
const int TIME_TURN = 520;
const int TIME_TURN_BACK = 900;
const int TIME_FORWARD = 700;
const int DISTANCE_STOP = 30;

int Echo = A4;
int Trig = A5;
int Distance = 0;

void setup(){
  Serial.begin(9600);
  pinMode(1,INPUT);
  pinMode(0,OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(SensorRight, INPUT); 
  pinMode(SensorLeft, INPUT); 
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

void run(){
  //while(digitalRead(SensorRight)==0 || digitalRead(SensorLeft)==0){
    if(mesure_Distance() < DISTANCE_STOP){
      brake();
    }else{
      forward();
      if(digitalRead(SensorLeft)==1 && digitalRead(SensorRight)==0){ //Tourne vers la droite
        turn_right();
      }
      if(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==0){ //Tourne vers la gauche
        turn_left();
      }
    }
  //}
  brake();
}

void back(){
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(TIME_TURN_BACK);
  brake();
}

void forward(){
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(TIME_FORWARD);
  brake();
}

void brake(){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turn_left(){
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turn_left_90(){
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  turn_left();
  delay(TIME_TURN);
  brake();
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
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  turn_right();
  delay(TIME_TURN);
  brake();
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

char command = 'a';
void loop(){
  if(Serial.available()){
    command = Serial.read();
    switch(command){
      case 'G':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'S':
        brake();
        break;
      case 'L':
        turn_left_90();
        break;
      case 'R':
        turn_right_90();
        break;
      case 'F':
        forward();
        break;
      break;          
     }
     command = '1';
  }
}
