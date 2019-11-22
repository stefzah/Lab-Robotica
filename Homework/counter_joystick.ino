const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int pinX = A1;
const int pinY = A3;
const int pinSW = A5;
const unsigned long digitDelay = 5;
const int segSize = 8;
unsigned  long lastDigitTime = 0;
const int noOfDisplays = 4;
const int noOfDigits = 10;
const int thresholdRight = (1024 / 4) * 3;
const int thresholdLeft = (1024 / 4);
const int thresholdUp = (1024 / 4) * 3;
const int thresholdDown = (1024 / 4);
const int thresholdSW=0;
const unsigned long blinkDelay=200;
int lockedStatus = 0;
int globalNumber = 0;
int globalDigit = 0;
int NUMBERS[]={0,0,0,0};
int lastXR;
int lastXL;
int lastX;
int lastYR;
int lastYL;
int lastY;
int last1SW;
int lastSW;
unsigned long lastBlink=0;
int blinkStatus=0;
// segments array, similar to before
int segments[segSize] = {
        pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
        pinD1, pinD2, pinD3, pinD4
};
int Digit[]={0,0,0,0};
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
        {1, 1, 1, 1, 1, 1, 0}, // 0
        {0, 1, 1, 0, 0, 0, 0}, // 1
        {1, 1, 0, 1, 1, 0, 1}, // 2
        {1, 1, 1, 1, 0, 0, 1}, // 3
        {0, 1, 1, 0, 0, 1, 1}, // 4
        {1, 0, 1, 1, 0, 1, 1}, // 5
        {1, 0, 1, 1, 1, 1, 1}, // 6
        {1, 1, 1, 0, 0, 0, 0}, // 7
        {1, 1, 1, 1, 1, 1, 1}, // 8
        {1, 1, 1, 1, 0, 1, 1}  // 9
};


void displayDigit(byte digit, byte decimalPoint) {
    for (int i = 0; i < segSize - 1; i++) {
        digitalWrite(segments[i], digitMatrix[digit][i]);
    }

    // write the decimalPoint to DP pin
    digitalWrite(segments[segSize - 1], decimalPoint);
}

// activate the display no. received as param
void showDigit(int num) {
    for (int i = 0; i < noOfDisplays; i++) {
        digitalWrite(digits[i], HIGH);
    }
    if(num!=globalDigit||blinkStatus==0||lockedStatus==0)digitalWrite(digits[num], LOW);
     else digitalWrite(digits[num], HIGH);
}

void DisplayNumber(int numbers[]) {
    int lastDigit;
    int digit = 0;
    while (digit<4) {
        if (millis() - lastDigitTime >= digitDelay) {
            lastDigit = numbers[digit];  // get the last digit
            showDigit(digit);
            lastDigitTime = millis();
            displayDigit(lastDigit, HIGH);
            // increase this delay to see multiplexing in action. At about 100 it becomes obvious
            digit++;            // move to next display
            //number = number / 10;
        }
    }
}

bool JoyStickRight() {
    int sensorX = analogRead(A1);
    //Serial.println(sensorX);
    lastXR=sensorX;
    return sensorX > thresholdRight && lastX <= thresholdRight;
}

bool JoyStickLeft() {
    int sensorX = analogRead(A1);
    lastXL=sensorX;
    return sensorX < thresholdLeft && lastX >= thresholdLeft;
}

bool JoyStickUp() {
    int sensorY = analogRead(A3);
    //Serial.println(sensorX);
    lastYR=sensorY;
    return sensorY > thresholdUp && lastY <= thresholdUp;
}

bool JoyStickDown() {
    int sensorY = analogRead(A3);
    lastYL=sensorY;
    return sensorY < thresholdDown && lastY >= thresholdDown;
}

int lastPress=0;
bool JoyStickPress() {
    int sensorSW = digitalRead(1);
    last1SW=sensorSW;
    Serial.println(sensorSW);
    //if(sensorSW<=thresholdSW && lastSW > thresholdSW){Serial.println(sensorSW);Serial.println(lastSW);Serial.println(" ");}
    if(lastPress==1&&sensorSW==0 && lastSW == 0)return 0;
    if(lastPress==0&&sensorSW==0 && lastSW == 0){lastPress=1;return 1;}
    else {lastPress=0;return 0;}
}

void blinkGlobalDigit()
{if(millis()-lastBlink>blinkDelay){lastBlink=millis();blinkStatus=1-blinkStatus;}
 //digitalWrite(digits[globalDigit],blinkStatus);
}
void setAllONExcept(int digit)
{
 for (int i = 0; i < noOfDisplays; i++) {
       if(digit!=i) digitalWrite(digits[i], LOW);
    }
}

void setAllONE()
{
 for (int i = 0; i < noOfDisplays; i++) {
       digitalWrite(digits[i], LOW);
    }
}
void setup() {
    for (int i = 0; i < segSize - 1; i++) {
        pinMode(segments[i], OUTPUT);
    }
    for (int i = 0; i < noOfDisplays; i++) {
        pinMode(digits[i], OUTPUT);
    }
    pinMode(pinX, INPUT);
    pinMode(pinY, INPUT);
    pinMode(pinSW, INPUT);

    Serial.begin(9600);

}


void loop() {
   if(lockedStatus==1)
   {
    DisplayNumber(NUMBERS);
    //setAllONExcept(globalDigit);
    //displayDigit(0,0);
    
     lastX=lastXR;
    if (JoyStickRight())globalDigit--;
     lastX=lastXL;
    if (JoyStickLeft())globalDigit++;
    
    if (globalDigit==-1)globalDigit=3;
    if (globalDigit==4)globalDigit=0;
    blinkGlobalDigit();
    //Serial.println(globalDigit);
   }
   else if(lockedStatus==2)
   {
    DisplayNumber(NUMBERS);
    //setAllONExcept(globalDigit);
    //displayDigit(0,0);
    

    
    lastY=lastYR;
    if (JoyStickUp())NUMBERS[globalDigit]--;
    lastY=lastYL;
    if (JoyStickDown())NUMBERS[globalDigit]++;
    
    if(NUMBERS[globalDigit]==10)NUMBERS[globalDigit]=0;
    if(NUMBERS[globalDigit]==-1)NUMBERS[globalDigit]=9;
    
    if (globalDigit==-1)globalDigit=3;
    if (globalDigit==4)globalDigit=0;
    blinkGlobalDigit();
    //Serial.println(globalDigit);
   }
   else 
   {//setAllONE();
    DisplayNumber(NUMBERS);
   }
   
    
   
    if (globalNumber == -1)globalNumber = 9999;
    if (globalNumber == 10000)globalNumber = 0;
    lastSW=last1SW;
    if(JoyStickPress()){lockedStatus++;if(lockedStatus==3)lockedStatus=0;}
    //Serial.println(blinkStatus);
    //Serial.println(globalDigit);
    Serial.println(lastSW);
}
