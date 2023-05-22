#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_TEMPLATE_ID "TMPL6_JYtHvuu"
#define BLYNK_TEMPLATE_NAME "car"
#define BLYNK_AUTH_TOKEN "dXQNgnroCOjeYdfBZXbKnFap_zsgm4GN"

char auth[] = "dXQNgnroCOjeYdfBZXbKnFap_zsgm4GN";
char ssid[] = "q";
char pass[] = "111111111";

BlynkTimer timer;

const int IN1 = 4;
const int IN2 = 5;
const int ENA = 18;
const int IN3 = 12;
const int IN4 = 13;
const int ENB = 19;

const int LINE1 = 14;
const int LINE2 = 27;
const int LINE3 = 21;
const int LINE4 = 22;
const int LINE5 = 23;
int a = 0;
const byte SpMaxL=250;
const byte SpMaxR=250;
int speed, frequency = 1000;
BLYNK_WRITE(V5) {
  if(a == 0)
    a = 1;
  else
    a = 0;
}
BLYNK_WRITE(V0) {
  speed = param.asInt();
}
BLYNK_WRITE(V1) {
  digitalWrite(IN2, param.asInt());
  digitalWrite(IN4, param.asInt());
  ledcWrite(0, speed);
  ledcWrite(1, speed);
  Serial.print("di thang");
}
BLYNK_WRITE(V2) {
  digitalWrite(IN1, param.asInt());
  digitalWrite(IN3, param.asInt());
  ledcWrite(0, speed);
  ledcWrite(1, speed);
  Serial.print("di lui");
}
BLYNK_WRITE(V3) {
  digitalWrite(IN2, param.asInt());
  digitalWrite(IN3, param.asInt());
  ledcWrite(0, speed);
  ledcWrite(1, speed);
  Serial.print("re trai");
}
BLYNK_WRITE(V4) {
  digitalWrite(IN1, param.asInt());
  digitalWrite(IN4, param.asInt());
  ledcWrite(0, speed);
  ledcWrite(1, speed);
  Serial.print("re phai");
}
void doline()
{
  switch (trangThai5CamBien())
  {
    case 200:
      //Tien(SpMaxL,SpMaxR-150);
      Lui(SpMaxL,SpMaxR);
      //Rephai();
      break;
    case 0:
      Tien(SpMaxL,SpMaxR);
      break;
    case 1:
      Tien(SpMaxL-10,SpMaxR);
      break;
    case 2:
      Tien(SpMaxL-20,SpMaxR);
      break;
    case 3:
      Tien(0,SpMaxR);
      break;
    case -1:
      Tien(SpMaxL,SpMaxR-10);
      break;
    case -2:
      Tien(SpMaxL,SpMaxR-20);
      break;
    case -3:
      Tien(SpMaxL,0);
      break;
    default:
      digitalWrite(6, 0);
      digitalWrite(7, 0);
      digitalWrite(8, 0);
      digitalWrite(9, 0);
      break;
  }
}

int trangThai5CamBien ()
{
  /*
      Âm là lệch phải
      Dương là lệch trái
      Mức độ lệch giảm tăng dần : 1 2 3
  */
  int sens1 = digitalRead(LINE1);
  int sens2 = digitalRead(LINE2);
  int sens3 = digitalRead(LINE3);
  int sens4 = digitalRead(LINE4);
  int sens5 = digitalRead(LINE5);
  switch (lechvach5mat())
  {
    case -3:
      return -3;
      break;
    case -2:
      return -2;
      break;
    case -1:
      return -1;
      break;
    case 0:
      if ( (abs(!sens1) + abs(!sens2) + abs(!sens3) + abs(!sens4) + abs(!sens5)) == 0)
        return 200;
      else
        return 0;
      break;
    case 1:
      return 1;
      break;
    case 2:
      return 2;
      break;
    case 3:
      return 3;
      break;

  }
}
int lechvach5mat()
{

  int sens1 = digitalRead(LINE1);
  int sens2 = digitalRead(LINE2);
  int sens3 = digitalRead(LINE3);
  int sens4 = digitalRead(LINE4);
  int sens5 = digitalRead(LINE5);
  int lechvach = -2 * !sens1 - !sens2 + !sens4 + 2 * !sens5;
  switch (lechvach)
  {
    case -3:// cái này là do 2 mắt biên đều chạm vạch
      return -2;
      break;
    case -2:
      return -3;
      break;
    case -1:
      return -1;
      break;
    case 0:
      return 0;
      break;
    case 1:
      return 1;
      break;
    case 2:
      return 3;
      break;
    case 3:
      return 2;
      break;

  }
}
void Tien( byte speedL, byte speedR)
{
  ledcWrite(0, speedL);
  ledcWrite(1, speedR);
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  Serial.print("tien");
}

void Lui(byte SpeedL, byte SpeedR)
{
  ledcWrite(0, SpeedL);
  ledcWrite(1, SpeedR);
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  Serial.print("lui");
}
void setup(){
  Serial.begin(115200);

  // Configure PWM channels for ENA and ENB pins
  ledcSetup(0, frequency, 8);
  ledcSetup(1, frequency, 8);

  // Map PWM channels to ENA and ENB pins
  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);

  // Set initial speeds to zero
  ledcWrite(0, 0);
  ledcWrite(1, 0);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
}

void loop(){
  Blynk.run();
  if(a == 1){
    doline();
  }
}