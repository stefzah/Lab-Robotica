#include <LiquidCrystal.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int pinX = A5;
const int pinY = A4;
const int pinButton = 13;
const int thresholdRight = 512 + 200;
const int thresholdLeft = 512 - 200;
const int thresholdUp = 512 + 200;
const int thresholdDown = 512 - 200;
const int gameTime = 10000;
const int levelTime = 2000;
int globalMode = 0;
int sensorCurrX = 512;
int sensorLastRX = 512;
int sensorLastLX = 512;
int sensorCurrY = 512;
int sensorLastUY = 512;
int sensorLastDY = 512;
int sensorCurrButton = 0;
int sensorLastButton = 1;
struct player {
    int score = 0;
    int level = 0;
    String name = "Unknown";
} currPlayer, bestPlayer;
String Name;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

void setup() {
// set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    Serial.begin(9600);
    Enter(globalMode);
}

void printMenu() {
    lcd.setCursor(1, 0);
    lcd.print("Start");

    lcd.setCursor(10, 0);
    lcd.print("Setup");

    lcd.setCursor(1, 1);
    lcd.print("HighScore");
}

void printHighScore() {
    lcd.setCursor(0, 0);
    lcd.print(bestPlayer.name);
    lcd.print(" ");
    lcd.print(bestPlayer.score);
}

void printSetup() {
    lcd.setCursor(1, 0);
    lcd.print("Name: ");
    lcd.setCursor(1, 1);
    lcd.print("Level: 0");

}

void printStart() {
    lcd.setCursor(0, 0);
    lcd.print("Lives:3");
    lcd.setCursor(8, 0);
    lcd.print("Level:");
    unsigned long long currentTime = millis();
    int level;
    while (millis() - currentTime < 10000) {
        level = currPlayer.level + (millis() - (currentTime)) / 2000;
        //Serial.println(level);
        currPlayer.score = level * 3;
        lcd.setCursor(14, 0);
        lcd.print(level);
        lcd.setCursor(0, 1);
        lcd.print("Score:");
        lcd.print(currPlayer.score);
    }
    if (currPlayer.score > bestPlayer.score)bestPlayer = currPlayer;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Congratulations,");
    lcd.setCursor(0, 1);
    lcd.print("Press the button");
}

bool joyRight() {
    sensorCurrX = analogRead(pinX);
    if (sensorCurrX > thresholdRight && sensorLastRX <= thresholdRight) {
        sensorLastRX = sensorCurrX;
        return true;
    }
    sensorLastRX = sensorCurrX;
    return false;
}

bool joyLeft() {
    sensorCurrX = analogRead(pinX);
    if (sensorCurrX < thresholdLeft && sensorLastLX >= thresholdLeft) {
        sensorLastLX = sensorCurrX;
        return true;
    }
    sensorLastLX = sensorCurrX;
    return false;
}

bool joyUp() {
    sensorCurrY = analogRead(pinY);
    if (sensorCurrY > thresholdUp && sensorLastUY <= thresholdUp) {
        sensorLastUY = sensorCurrY;
        return true;
    }
    sensorLastUY = sensorCurrY;
    return false;
}

bool joyDown() {
    sensorCurrY = analogRead(pinY);
    if (sensorCurrY < thresholdDown && sensorLastDY >= thresholdDown) {
        sensorLastDY = sensorCurrY;
        return true;
    }
    sensorLastDY = sensorCurrY;
    return false;
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

void selectMenu(int selectMode) {
    if (selectMode == 3)selectMode = 0;
    if (selectMode == -1)selectMode = 2;
    if (selectMode == 0) {
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(9, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(" ");
    }
    if (selectMode == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(9, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");
    }
    if (selectMode == 2) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(9, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");
    }
    if (joyLeft() == 1)selectMode--;
    if (joyRight() == 1)selectMode++;
    globalMode = selectMode;
}

void selectSetup(int selectMode) {
    if (selectMode == 2)selectMode = 0;


    if (selectMode == -1)selectMode = 1;
    if (selectMode == 0) {
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");
    }
    if (selectMode == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");
    }
    if (joyLeft() == 1)selectMode--;
    if (joyRight() == 1)selectMode++;

    if (selectMode == 1 && joyUp() == 1) {
        currPlayer.level++;
        if (currPlayer.level == 10)currPlayer.level = 9;
        lcd.setCursor(8, 1);
        lcd.print(currPlayer.level);
    }
    if (selectMode == 1 && joyDown() == 1) {
        currPlayer.level--;
        if (currPlayer.level == -1)currPlayer.level = 0;
        lcd.setCursor(8, 1);
        lcd.print(currPlayer.level);
    }
    if (selectMode == 0) {
        char c = (char) Serial.read();
        if (c >= 'A' && c < 'z') {
            Name += c;
            currPlayer.name = Name;
        }
        if (c == '\n') {
            lcd.setCursor(7, 0);
            lcd.print("       ");
            lcd.setCursor(7, 0);
            lcd.print(currPlayer.name);
            Name.remove(0, Name.length());
        }
    }
    globalMode = selectMode;
}

void Enter(int mode) {
    if (mode == 0) {
        lcd.clear();
        printMenu();
        globalMode = 0;
        while (pressButton() == 0)
            selectMenu(globalMode);
        Enter(globalMode + 1);
    }

    if (mode == 1) {
        lcd.clear();
        printStart();
        while (pressButton() == 0) {}
        Enter(0);
    }
    if (mode == 2) {
        lcd.clear();
        printSetup();
        globalMode = 0;
        while (pressButton() == 0)
            selectSetup(globalMode);

        Enter(0);
    }
    if (mode == 3) {
        lcd.clear();
        printHighScore();
        while (pressButton() == 0) {}
        Enter(0);
    }
}


void loop() {

}
