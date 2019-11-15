#include "pitches.h"

// notes in the melody:
int melody[] = {

        NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
        4, 8, 8, 4, 4, 4, 4, 4
};

const int pin_passive = A0;
const int pin_active = 13;
const int pin_led = 7;
const int pushButton = 2;
const int lag = 8;
int buttonState = 0;
int buttonState1 = 0;
int currentState = 0;
int previousState = 0;
int buttonCounter = 0;
int thisNote;
int cons0 = 0;
int cons1 = 0;
bool led_state;
bool button_state = 0;
long long last_note, melody_duration, melody_last_time;
int knockValue;

void setup() {
    Serial.begin(9600);
    pinMode(pushButton, INPUT);
    pinMode(pin_passive, INPUT);
    pinMode(pin_active, OUTPUT);
}

void ResetButton() {
    buttonState = 0;
    buttonState1 = 0;
    currentState = 0;
    previousState = 0;
    buttonCounter = 0;
    cons0 = 0;
    cons1 = 0;

}
bool brk=0;
void CheckButton() {brk=0;
    buttonState = digitalRead(pushButton);
    Serial.println(buttonState);
    if (buttonState == LOW) {
        cons0++;
        if (cons0 >= lag)cons1 = 0;
    } else if (buttonState == HIGH) {
        cons1++;
        if (cons1 >= lag)cons0 = 0;
    }
    if (currentState == LOW && cons1 == lag)currentState = HIGH;
    else if (currentState == HIGH && cons1 == lag)currentState = LOW;
    buttonState1 = currentState;
    if (buttonState1)brk=1;
    Serial.println(buttonState1);
}

void SingPlease() {
    melody_last_time = millis();
    melody_last_time = millis();
    melody_duration = 1000 / 4 * 1.30 * 8;
    last_note = millis();
    for (thisNote = 0; thisNote < 8;) {
        long long noteDuration = 1000 / noteDurations[thisNote];
        long long pauseBetweenNotes = noteDuration * 1.3;

        if (millis() - last_note > pauseBetweenNotes) {
            led_state = 1;
            digitalWrite(pin_led, led_state);
            tone(8, melody[thisNote], noteDuration);
            thisNote++;
            last_note = millis();
        }
        if (millis() - last_note > noteDuration && led_state) {
            led_state = 0;
            digitalWrite(pin_led, led_state);
        }

        CheckButton();
        if(brk==1)break;
    }
}

void loop() {
    knockValue = analogRead(pin_passive);
    if (millis() - melody_last_time > melody_duration)
        if (knockValue > 10) {
            buttonState1 = 0;
            ResetButton();
            while (buttonState1 == 0)
                SingPlease();
        }
}
