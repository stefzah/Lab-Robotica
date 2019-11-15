
const int analogInPin1 = A0;
const int analogInPin2 = A2;
const int analogInPin3 = A5;

const int analogOutPin1 = 6;
const int analogOutPin2 = 9;
const int analogOutPin3 = 11;

int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;

int outputValue1 = 0;
int outputValue2 = 0;
int outputValue3 = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    sensorValue1 = analogRead(analogInPin1);
    sensorValue2 = analogRead(analogInPin2);
    sensorValue3 = analogRead(analogInPin3);

    Serial.print(" sensor1 = ");
    Serial.println(sensorValue1);
    outputValue1 = map(sensorValue1, 0, 1023, 0, 255);
    analogWrite(analogOutPin1, outputValue1);

    Serial.print(" sensor2 = ");
    Serial.println(sensorValue2);
    outputValue2 = map(sensorValue2, 0, 1023, 0, 255);
    analogWrite(analogOutPin2, outputValue2);


    Serial.print(" sensor3 = ");
    Serial.println(sensorValue3);
    outputValue3 = map(sensorValue3, 0, 1023, 0, 255);
    analogWrite(analogOutPin3, outputValue3);
}
