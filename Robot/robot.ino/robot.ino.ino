const int IN1 = 11;
const int IN2 = 9;
const int IN3 = 8;
const int IN4 = 7;
const int ENA = 6;
const int ENB = 5; 
const int SensorRight = 10;     
const int SensorLeft = 2;
int SL;//state
int SR;
int Echo = A4;  //
int Trig =A5;  //
int Distance = 0;
void setup()
{
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode(SensorRight, INPUT); 
  pinMode(SensorLeft, INPUT); 
  Serial.begin(115200);     
  pinMode(Echo, INPUT);    // 
  pinMode(Trig, OUTPUT);   // 
}
 void run()
{
  analogWrite(ENA, 90);
  analogWrite(ENB, 90);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  if(digitalRead(SensorLeft)==1 && digitalRead(SensorRight)==0){
    while(digitalRead(SensorLeft)==1 && digitalRead(SensorRight)==0){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    delay(100);
  }
  if(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==0){
    while(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==0){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
      delay(100);
  }
  if(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==1){
    while(digitalRead(SensorRight)==1 && digitalRead(SensorLeft)==1){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENA, 200);
        analogWrite(ENB, 200);
    }
    delay(100);
  }
}
void brake()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void spin_left()       
{
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay (550);
}
void spin_right()
{
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay (550);
}
void back()
{
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay (1800);
  brake();
}
//==========================================================
void Distance_test() 
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance= Fdistance/58;
  Serial.print("Distance:");
  Serial.println(Fdistance);
  Distance = Fdistance;
}
void loop()
{ 
    //Distance_test();
 run();
}
/*Serial.print("Left : ");
Serial.println(digitalRead(SensorLeft));
Serial.print("Right : ");
Serial.println(digitalRead(SensorRight));*/
