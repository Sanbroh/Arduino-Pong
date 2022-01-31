#include <LedControl.h>

const int DIN = 11;
const int CS = 12;
const int CLK = 13;
const int buzzer = 5;
LedControl lc=LedControl(DIN, CLK, CS,0);

int VRx = A0;
int VRy = A1;
int SW = 8;
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

bool arr[8][8];
int posX = 0;
int posY = 4;

int enemyX = 0;
int enemyY = 4;

int ballX = 4;
int ballY = 4;
int ballvX = 1;
int ballvY = random(-1,1);

void setup() {
  Serial.begin(9600);
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  pinMode(buzzer, OUTPUT);
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      arr[i][j] = false;
    }
  }

  while (ballvY == 0) {
    ballvY = random(-1,1);
  }
}

void(* resetFunc) (void) = 0;
  
void loop(){
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = digitalRead(SW);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, -512, 512);
  
  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print(" | Button: ");
  Serial.println(SW_state);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      arr[i][j] = false;
    }
  }
  arr[posX][posY] = true;
  arr[posX][posY+1] = true;
  arr[posX][posY-1] = true;

  enemyY = ballY;

  if (enemyY > 6) {
    enemyY = 6;
  } else if (enemyY < 1) {
    enemyY = 1;
  }
  arr[7][enemyY] = true;
  arr[7][enemyY+1] = true;
  arr[7][enemyY-1] = true;

  if (mapX < -10) {
    posY++;
  } else if (mapX > 10) {
    posY--;
  }

  if (posY > 6) {
    posY = 6;
  } else if (posY < 1) {
    posY = 1;
  }

  if (arr[ballX+ballvX][ballY] == true) {
    ballvX *= -1;
    tone(buzzer, 1000);
    delay(50);
    noTone(buzzer);
  } else {
    ballY += ballvY;
    ballX += ballvX;
  }
  
  if (ballY > 7) {
    ballY = 7;
    ballvY *= -1;
  } else if (ballY < 0) {
    ballY = 0;
    ballvY *= -1;
  }

  if (ballX > 6) {
    for(int i = 0; i < 3; i++) {
      lc.setLed(0, ballX, ballY-ballvY, true);
      tone(buzzer, 500);
      delay(100);
      lc.setLed(0, ballX, ballY-ballvY, false);
      noTone(buzzer);
      delay(100);
    }
    for(int j=0;j<8;j++){
     for(int i=0;i<8;i++){
       lc.setLed(0,i,j,true);
       delay(20);
       lc.setLed(0,i,j,false);
     }
    } 
    resetFunc();
  } else if (ballX < 1) {
     for(int i = 0; i < 3; i++) {
      lc.setLed(0, ballX, ballY-ballvY, true);
      tone(buzzer, 500);
      delay(100);
      lc.setLed(0, ballX, ballY-ballvY, false);
      noTone(buzzer);
      delay(100);
    }
     for(int j=0;j<8;j++){
     for(int i=0;i<8;i++){
       lc.setLed(0,i,j,true);
       delay(20);
       lc.setLed(0,i,j,false);
     }
    } 
    resetFunc();
  }

  lc.clearDisplay(0);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (arr[i][j] || (ballX == i && ballY == j)) {
        lc.setLed(0, i, j, true);
      }
    }
  }
  delay(100);
}
