#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

int sensorPin[6] = {A0,A1,A2,A3,A4,A5};
int sensorVal[6];
boolean hairpin=0;
boolean BTN = 0;
int lastSensorSum = 0;
boolean lastDir = 0;
const int pinButton = 2;
int buttonState = 1;
int sensorCurrButton=1;
int sensorLastButton=1;
struct{
       int fast,slow;
       }speedValues[4];
void setup()
{
  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  pinMode(pinButton,INPUT_PULLUP);
  Serial.begin(9600);
}

void StartFollowingLine()
{
  double currSensorSum=0;
 bool ok=false;
 int ct=0;
 for(int i=0;i<6;i++)
     {sensorVal[i]=analogRead(sensorPin[i]);
      if(sensorVal[i]>850){ct++;currSensorSum+=i-2.5;}
      Serial.print(sensorVal[i]);
      Serial.print(" ");
     }
     Serial.println(" ");
 currSensorSum/=ct;
  if(ct>0&&currSensorSum<=-1){
 setMotor1(20);
 setMotor2(80);
 lastDir=0;
     }
 else if(ct>0&&currSensorSum>=1){
 setMotor1(80);
 setMotor2(20);
 lastDir=1;
     }
 else if(ct>0){
 setMotor1(120);
 setMotor2(120);
     }
 else {
  if(lastDir==0)
   {setReverseMotor1(60);
    setMotor2(60);
   }
  if(lastDir==1)
    {setMotor1(60);
     setReverseMotor2(60);
    }
 }
 lastSensorSum=currSensorSum;
}

void setMotor1(int motorSpeed)
{motor1.run(FORWARD);
 motor1.setSpeed(motorSpeed);
}
void setMotor2(int motorSpeed)
{motor2.run(FORWARD);
 motor2.setSpeed(motorSpeed);
}
void setReverseMotor1(int motorSpeed)
{motor1.run(BACKWARD);
 motor1.setSpeed(motorSpeed);
}
void setReverseMotor2(int motorSpeed)
{motor2.run(BACKWARD);
 motor2.setSpeed(motorSpeed);
}

bool pressButton() {
    sensorCurrButton = digitalRead(pinButton);
    //Serial.println(sensorCurrButton);
    if (sensorCurrButton == 1 && sensorLastButton == 0) {
        sensorLastButton = 1;
        return true;
    }
    sensorLastButton = sensorCurrButton;
    return false;
}
void iddleState()
{
 
}
void loop()
{
  if(pressButton()==true)BTN=!BTN;
  if(BTN==true)StartFollowingLine();
  else {setMotor1(0);
        setMotor2(0);
       }
}
