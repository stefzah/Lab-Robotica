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
const int pinX = A3;
const int pinY = A4;
const int pinSW = A5;
const unsigned long long digitDelay = 2;
const int segSize = 8;
unsigned long long lastDigitTime = 0;
const int noOfDisplays = 4;
const int noOfDigits = 10;
const int thresholdRight = (1024 / 4) * 3;
const int thresholdLeft = (1024 / 4);

int globalNumber = 0;
int lastX;

// segments array, similar to before
int segments[segSize] = {
        pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
        pinD1, pinD2, pinD3, pinD4
};

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
    digitalWrite(digits[num], LOW);
}

void DisplayNumber(int number) {
    int lastDigit;
    int digit = 0;
    if (number == 0) {
        showDigit(digit);
        displayDigit(0, HIGH);
    }
    while (number != 0) {
        if (millis() - lastDigitTime >= digitDelay) {
            lastDigit = number % 10;  // get the last digit
            showDigit(digit);
            lastDigitTime = millis();
            displayDigit(lastDigit, HIGH);
            // increase this delay to see multiplexing in action. At about 100 it becomes obvious
            digit++;            // move to next display
            number = number / 10;
        }
    }
}

bool JoyStickRight() {
    int sensorX = analogRead(A3);
    //Serial.println(sensorX);
    return sensorX > thresholdRight && lastX <= thresholdRight;
}

bool JoyStickLeft() {
    int sensorX = analogRead(A3);
    return sensorX < thresholdLeft && lastX >= thresholdLeft;
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
    DisplayNumber(globalNumber);
    if (JoyStickRight())globalNumber++;
    if (JoyStickLeft())globalNumber--;
    if (globalNumber == -1)globalNumber = 9999;
    if (globalNumber == 10000)globalNumber = 0;
    lastX = analogRead(pinX);
}
