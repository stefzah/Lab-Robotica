#include <LiquidCrystal.h>
#include <LedControl.h>
LedControl lc = LedControl(12, 11, 10, 1);
const int RS = 7;
const int enable = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int pinX = A5;
const int pinY = A4;
const int pinButton = 13;
const int pinButton0 = A3;
const int pinButton1 = A2;
const int pinButton2 = A1;
const int pinButton3 = A0;
const int thresholdRight = 512 + 200;
const int thresholdLeft = 512 - 200;
const int thresholdUp = 512 - 200;
const int thresholdDown = 512 + 200;
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
int sensorCurrButton0 = 0;
int sensorLastButton0 = 1;
int sensorCurrButton1 = 0;
int sensorLastButton1 = 1;
int sensorCurrButton2 = 0;
int sensorLastButton2 = 1;
int sensorCurrButton3 = 0;
int sensorLastButton3 = 1;
unsigned long long startTime = 0;
unsigned long long currTime = 0;
unsigned long long currTIME = 0;
unsigned long long prevTIME = 0;
unsigned long long currBotTIME = 0;
unsigned long long prevBotTIME = 0;
unsigned long long deltaTIME = 500;
int gameMode = 0;
int pozx = 0;
int pozy = 0;
struct game {
    String p1_name = "Player";
    String p2_name = "BOT";
    int p1_score = 0;
    int p2_score = 0;
    int p1_poz = 3;
    int p2_poz = 3;
    int ball_pozx = 3;
    int ball_pozy = 3;
    int ball_velx = 0;
    int ball_vely = -1;
    double ball_speed = 1;
    double bot_speed = 1;
    int last_loser = 0;
    int level = 1;
} Game;
struct player {
    int score = 0;
    int level = 1;
    int rounds = 0;
    String name = "Unknown";
} currPlayer, bestPlayer, firstPlayer, secondPlayer;
String Name;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
void setup() {
// set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    Serial.begin(9600);
    lc.shutdown(0, false);//first driver
    lc.setIntensity(0, 2);//intensity from 0-15
    lc.clearDisplay(0);//clears the display on the first driver
    pinMode(pinButton, INPUT_PULLUP);
    pinMode(pinButton0, INPUT_PULLUP);
    pinMode(pinButton1, INPUT_PULLUP);
    pinMode(pinButton2, INPUT_PULLUP);
    pinMode(pinButton3, INPUT_PULLUP);
    //EEPROM.get(0,bestPlayer);
    printP();
    while(pressButton()==0)
    {
      printWelcome();
    }
    
    Enter(globalMode); 
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

bool pressButton0() {
    sensorCurrButton0 = digitalRead(pinButton0);
    //Serial.println(sensorCurrButton);
    if (sensorCurrButton0 == 1 && sensorLastButton0 == 0) {
        sensorLastButton0 = 1;
        return true;
    }
    sensorLastButton0 = sensorCurrButton0;
    return false;
}

bool pressButton1() {
    sensorCurrButton1 = digitalRead(pinButton1);
    //Serial.println(sensorCurrButton);
    if (sensorCurrButton1 == 1 && sensorLastButton1 == 0) {
        sensorLastButton1 = 1;
        return true;
    }
    sensorLastButton1 = sensorCurrButton1;
    return false;
}

bool pressButton2() {
    sensorCurrButton2 = digitalRead(pinButton2);
    //Serial.println(sensorCurrButton);
    if (sensorCurrButton2 == 1 && sensorLastButton2 == 0) {
        sensorLastButton2 = 1;
        return true;
    }
    sensorLastButton2 = sensorCurrButton2;
    return false;
}

bool pressButton3() {
    sensorCurrButton3 = digitalRead(pinButton3);
    //Serial.println(sensorCurrButton);
    if (sensorCurrButton3 == 1 && sensorLastButton3 == 0) {
        sensorLastButton3 = 1;
        return true;
    }
    sensorLastButton3 = sensorCurrButton3;
    return false;
}


void printP()
{
 lc.clearDisplay(0);
 bool matrix[8][8] = {
{1, 1, 1, 1, 1, 1, 1, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 1, 1, 1, 1, 0, 1},
{1, 0, 1, 0, 0, 1, 0, 1},
{1, 0, 1, 1, 1, 1, 0, 1},
{1, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 1, 0, 0, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1}
};
 for(int i=0;i<8;i++)
    {for(int j=0;j<8;j++)
       lc.setLed(0,i,j,matrix[i][j]);
    }
}

void printWelcome()
{
 lcd.setCursor(0,0);
 lcd.print("   WELCOME TO   ");
 lcd.setCursor(0,1);
 lcd.print("   P  O  N  G   ");

  
}
void printMenu() {
    lcd.setCursor(1, 0);
    lcd.print("Start");

    lcd.setCursor(11, 0);
    lcd.print("Setup");

    lcd.setCursor(1, 1);
    lcd.print("HighScore");

    lcd.setCursor(12, 1);
    lcd.print("Info");
}

void printHighScore() {
    lcd.setCursor(0, 0);
    if(bestPlayer.score==0)
    {lcd.print("Nobody beated");
     lcd.setCursor(0, 1);
     lcd.print("the MIGHTY BOT");
    }
    else{
    lcd.print(bestPlayer.name);
    lcd.print(" beated");
    lcd.setCursor(0, 1);
    if(bestPlayer.score==1)lcd.print("Easy ");
    if(bestPlayer.score==2)lcd.print("Medium ");
    if(bestPlayer.score==3)lcd.print("Hard ");
    lcd.print("3:");
    lcd.print(bestPlayer.rounds-3);
    }
}
void printInfo() {
    lcd.setCursor(0, 0);
    lcd.print(" PONG by Stefan");
    lcd.setCursor(0, 1);
    lcd.print("@ UNIBUCRobotics");
}
void PrepareGame() {
    Game.ball_pozx = 4;
    Game.ball_pozy = 3;
    Game.p1_poz = 3;
    Game.p2_poz = 3;
    Game.ball_speed = 1;
    Game.ball_velx = 0;
    Game.ball_vely = 1;
    Game.bot_speed = 1.2 + (double) (currPlayer.level - 1) / 2.5;
}

void printWall(int x, int y) {
    for (int i = 0; i < x; i++)
        lc.setLed(0, i, y, 0);
    lc.setLed(0, x, y, 1);
    lc.setLed(0, x + 1, y, 1);
    lc.setLed(0, x + 2, y, 1);
    for (int i = x + 3; i < 8; i++)
        lc.setLed(0, i, y, 0);
}

void printBall(int x, int y) {
    lc.setLed(0, x, y, 1);
    lc.setLed(0, x, y - 1, 0);
    lc.setLed(0, x, y + 1, 0);
    lc.setLed(0, x - 1, y + 1, 0);
    lc.setLed(0, x + 1, y + 1, 0);
    lc.setLed(0, x - 1, y - 1, 0);
    lc.setLed(0, x + 1, y - 1, 0);
}

void moveBall() {
    Game.ball_pozx += Game.ball_velx;
    Game.ball_pozy += Game.ball_vely;
}

void moveBOT() {
    if (Game.p2_poz + 1 < Game.ball_pozx)Game.p2_poz++;
    if (Game.p2_poz == 6) Game.p2_poz = 5;
    else if (Game.p2_poz + 1 > Game.ball_pozx)Game.p2_poz--;
    if (Game.p2_poz == -1) Game.p2_poz = 0;
}

int startGame(bool BOT) {
    printWall(Game.p1_poz, 0);
    printWall(Game.p2_poz, 7);
    printBall(Game.ball_pozx, Game.ball_pozy);
    delay(1000);
    startTime = millis();
    currTime = millis();
    prevTIME = currTime - startTime;
    prevBotTIME = currTime - startTime;
    while (Game.ball_pozy >= 0 && Game.ball_pozy <= 7) {
        currTime = millis();
        currBotTIME = currTime - startTime;
        if (pressButton0() == 1)Game.p1_poz--;
        if (Game.p1_poz == -1) Game.p1_poz = 0;
        if (pressButton1() == 1)Game.p1_poz++;
        if (Game.p1_poz == 6) Game.p1_poz = 5;
        if (BOT == 0) {
            if (pressButton2() == 1)Game.p2_poz--;
            if (Game.p2_poz == -1) Game.p2_poz = 0;
            if (pressButton3() == 1)Game.p2_poz++;
            if (Game.p2_poz == 6) Game.p2_poz = 5;
            prevBotTIME = currBotTIME;
        } else if ((double) currBotTIME > (double) prevBotTIME + (double) deltaTIME / Game.bot_speed) {
            moveBOT();
            prevBotTIME = currBotTIME;
        }
        printWall(Game.p1_poz, 0);
        printWall(Game.p2_poz, 7);
        printBall(Game.ball_pozx, Game.ball_pozy);
        currTime = millis();
        currTIME = currTime - startTime;
        if ((double) currTIME > (double) prevTIME + (double) deltaTIME / Game.ball_speed) {
            moveBall();
            if (Game.ball_pozx == -1) {
                Game.ball_pozx = 1;
                Game.ball_velx = 1;
            }
            if (Game.ball_pozx == 8) {
                Game.ball_pozx = 6;
                Game.ball_velx = -1;
            }
            if (Game.ball_pozy == 1 && Game.p1_poz <= Game.ball_pozx && Game.p1_poz + 2 >= Game.ball_pozx) {
                Game.ball_vely = 1;
                if (Game.ball_pozx == Game.p1_poz)Game.ball_velx = millis() % 3 - 1;
                else {
                    Game.ball_velx = 0 + millis() % 2;
                    if (Game.ball_velx == 0)Game.ball_velx = -1;
                }
                Game.ball_speed += 0.1;
            }
            if (Game.ball_pozy == 6 && Game.p2_poz <= Game.ball_pozx && Game.p2_poz + 2 >= Game.ball_pozx) {
                Game.ball_vely = -1;
                if (Game.ball_pozx == Game.p1_poz)Game.ball_velx = millis() % 3 - 1;
                else {
                    Game.ball_velx = 0 + millis() % 2;
                    if (Game.ball_velx == 0)Game.ball_velx = -1;
                }
                Game.ball_speed += 0.1;
            }
            prevTIME = currTIME;
        }
        //Serial.println("DAAAA");
        //lc.setLed(0, pozx, pozy, 1);
        //delay(5000);
    }
    if (Game.ball_pozy == -1)Game.p2_score++;
    else Game.p1_score++;
    return 0;
}

void printScore() {
    lcd.setCursor(1, 0);
    lcd.print(Game.p1_name);
    lcd.print(" ");
    lcd.print(Game.p1_score);
    lcd.setCursor(1, 1);
    lcd.print(Game.p2_name);
    lcd.print(" ");
    lcd.print(Game.p2_score);
}

void printLiveScore() {
    printScore();
    while (true) {
        if (Game.p1_score == 3) {
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print(Game.p1_name);
            lcd.print(" wins!");
            lcd.setCursor(1, 1);
            lcd.print("Congratulations!");
            currPlayer.score = currPlayer.level;
            currPlayer.rounds = Game.p1_score + Game.p2_score;
            if (bestPlayer.score < currPlayer.score){bestPlayer = currPlayer;}
            if(bestPlayer.score == currPlayer.score && bestPlayer.rounds > currPlayer.rounds)bestPlayer = currPlayer;
            break;
        } else if (Game.p2_score == 3) {
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print(Game.p2_name);
            lcd.print(" wins!");
            lcd.setCursor(1, 1);
            if (Game.p2_name != "BOT")
                lcd.print("Congratulations!");
            else lcd.print("Game Over :(");
            break;
        }
        if (Game.p2_name != "BOT") {
            lc.clearDisplay(0);
            PrepareGame();
            startGame(false);
            printScore();
        } else {
            lc.clearDisplay(0);
            PrepareGame();
            startGame(true);
            printScore();
        };
    }
    printP();
    delay(2000);
    while (pressButton() == 0) printP();
    
    Enter(0);
}

void printSetup() {
    lcd.setCursor(1, 0);
    lcd.print("Name: ");
    lcd.setCursor(7, 0);
    lcd.print(currPlayer.name);
    lcd.setCursor(1, 1);
    lcd.print("Level: ");
    lcd.setCursor(8, 1);
    if (currPlayer.level == 1)lcd.print("EASY  ");
    if (currPlayer.level == 2)lcd.print("MEDIUM");
    if (currPlayer.level == 3)lcd.print("HARD  ");
}
void printSetup2(){
  lcd.setCursor(1, 0);
  lcd.print("1st:");
  if(firstPlayer.name=="Unknown")lcd.print("Player1");
  else lcd.print(firstPlayer.name);
  lcd.setCursor(1, 1);
  lcd.print("2nd:");
  if(secondPlayer.name=="Unknown")lcd.print("Player2");
  else lcd.print(secondPlayer.name);
}

void printStart() {
    lcd.setCursor(1, 0);
    lcd.print("Play  vs  Bot");
    lcd.setCursor(1, 1);
    lcd.print("Play  vs  Human");
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


void selectStart(int selectMode) {
    if (selectMode == 2)selectMode = 0;
    if (selectMode == -1)selectMode = 1;
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
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");
    }
    if (joyLeft() == 1)selectMode--;
    if (joyRight() == 1)selectMode++;
    globalMode = selectMode;
}

void selectMenu(int selectMode) {
    if (selectMode == 4)selectMode = 0;
    if (selectMode == -1)selectMode = 2;
    if (selectMode == 0) {
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(10, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(11, 1);
        lcd.print(" ");
    }
    if (selectMode == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(10, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(11, 1);
        lcd.print(" ");
    }
    if (selectMode == 2) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(10, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.setCursor(11, 1);
        lcd.print(" ");
    }
    if (selectMode == 3) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(10, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(11, 1);
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
        if (currPlayer.level == 4)currPlayer.level = 1;
        lcd.setCursor(8, 1);
        if (currPlayer.level == 1)lcd.print("EASY  ");
        if (currPlayer.level == 2)lcd.print("MEDIUM");
        if (currPlayer.level == 3)lcd.print("HARD  ");
    }
    if (selectMode == 1 && joyDown() == 1) {
        currPlayer.level--;
        if (currPlayer.level == 0)currPlayer.level = 3;
        lcd.setCursor(8, 1);
        if (currPlayer.level == 1)lcd.print("EASY  ");
        if (currPlayer.level == 2)lcd.print("MEDIUM");
        if (currPlayer.level == 3)lcd.print("HARD  ");
    }
    if (selectMode == 0) {
        char c = (char) Serial.read();
        if (c >= 'A' && c < 'z') {
            Name += c;
            currPlayer.name = Name;
        }
        if (c == '\n') {
            lcd.setCursor(7, 0);
            lcd.print("          ");
            lcd.setCursor(7, 0);
            lcd.print(currPlayer.name);
            Name.remove(0, Name.length());
        }
    }
    globalMode = selectMode;
}
void selectSetup2(int selectMode) {
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
    if(selectMode==0)
    {
        char c = (char) Serial.read();
        if (c >= 'A' && c < 'z') {
            Name += c;
            firstPlayer.name = Name;
        }
        if (c == '\n') {
            lcd.setCursor(5, 0);
            lcd.print("           ");
            lcd.setCursor(5, 0);
            lcd.print(firstPlayer.name);
            Name.remove(0, Name.length());
        }
    }
    if(selectMode==1)
    {
        char c = (char) Serial.read();
        if (c >= 'A' && c < 'z') {
            Name += c;
            secondPlayer.name = Name;
        }
        if (c == '\n') {
            lcd.setCursor(5, 1);
            lcd.print("           ");
            lcd.setCursor(5, 1);
            lcd.print(secondPlayer.name);
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
        globalMode = 0;
        while (pressButton() == 0)
              {selectStart(globalMode);     
               if(pressButton1()==1){globalMode=-5;break;}
              }   
        Enter(globalMode + 5);
    }
    if (mode == 2) {
        lcd.clear();
        printSetup();
        globalMode = 0;
        while (pressButton() == 0 && pressButton1() == 0)
            selectSetup(globalMode);
        Enter(0);
    }
    if (mode == 3) {
        lcd.clear();
        printHighScore();
        globalMode=0;
        while (pressButton() == 0 && pressButton1() == 0)printHighScore();
        Enter(0);
    }
    if (mode == 4) {
        lcd.clear();
        printInfo();
        globalMode=0;
        while (pressButton() == 0 && pressButton1() == 0)printInfo();
        Enter(0);
    }
    
    if (mode == 5) {
        lcd.clear();
        Game.p1_name = currPlayer.name;
        Game.p2_name = "BOT";
        Game.p1_score = 0;
        Game.p2_score = 0;
        printLiveScore();
    }
    if (mode == 6) {
        lcd.clear();
        printSetup2();
        globalMode = 0;
        while (pressButton() == 0)
              selectSetup2(globalMode);
        Game.p1_name = firstPlayer.name;
        Game.p2_name = secondPlayer.name;
        Game.p1_score = 0;
        Game.p2_score = 0;
        lcd.clear();
        printLiveScore();
    }
}


void loop() {

}
