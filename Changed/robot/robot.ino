const int IN1 = 11;
const int IN2 = 9;
const int IN3 = 8;
const int IN4 = 7;
const int ENA = 6;
const int ENB = 5; 
const int SensorRight = 10;
const int SensorCenter = 4;  
const int SensorLeft = 2;

const int SPEED = 140;
const int SPEED_TURN = 200;
const int TIME_FORWARD = 100;
const int DISTANCE_STOP = 20;

int is_runing = 0;
int Echo = A4;
int Trig = A5;
int Distance = 0;
char last_turn = 'L';

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
  pinMode(SensorCenter, INPUT); 
  pinMode(SensorLeft, INPUT); 
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

void back(){
  while(digitalRead(SensorCenter) == 1){
    turn_left();
  }
  brake();
}

void forward(int time_forward){
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(time_forward);
}

void brake(){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turn_left(){
  analogWrite(ENA, SPEED_TURN);
  analogWrite(ENB, SPEED_TURN);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turn_right(){
  analogWrite(ENA, SPEED_TURN);
  analogWrite(ENB, SPEED_TURN);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turn_left_90(){
  while(digitalRead(SensorLeft) == 0 && digitalRead(SensorRight) == 0){
    turn_left();
  }
  while(digitalRead(SensorLeft) == 1 || digitalRead(SensorRight) == 1){
    turn_left();
  }
}

void turn_right_90(){
  while(digitalRead(SensorLeft) == 0 && digitalRead(SensorRight) == 0){
    turn_right();
  }
  while(digitalRead(SensorLeft) == 1 || digitalRead(SensorRight) == 1){
    turn_right();
  }
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

void try_distance(){
  while(mesure_Distance() < DISTANCE_STOP){
    brake();
  }
}

void runing(){
  if(digitalRead(SensorLeft)==1 && digitalRead(SensorRight)==0){ //Tourne vers la gauche 
    turn_left();
    last_turn = 'L';
  }else if(digitalRead(SensorLeft)==1 && digitalRead(SensorRight)==0){ //Tourne vers la droite
    turn_right();
    last_turn = 'R';
  }else if(digitalRead(SensorCenter) == 1){ //Perdu
    Serial.write('W');
    if(last_turn == 'L'){
      turn_left_90();
    }else{
      turn_right_90();
    }
  }else{
    forward(0);
  }
}

char command = 'a';
int has_start = 0;

void loop(){
  
  try_distance();
  
  if(Serial.available()){
    command = Serial.read();
    if((has_start == 0 && command == 'G') || has_start){ //Remove unwanted instruction
      has_start = 1;
      is_runing = 0;
      switch(command){
        case 'G':
          is_runing = 1;
          break;
        case 'B':
          back();
          break;
        case 'S':
          brake();
          has_start = 0;
          Serial.write('F');
          break;
        case 'L':
          turn_left_90();
          break;
        case 'R':
          turn_right_90();
          break;
        case 'F':
          forward(TIME_FORWARD);
          break;
        case 'Z':
          turn_left_90();
          turn_left_90();
          break;
        break;          
      }
    }
  }
  if(is_runing){
    runing();
  }
}
