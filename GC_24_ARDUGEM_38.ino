#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <EEPROM.h>

#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

#define BLACK 0x0000
#define RED 0x001F
#define BLUE 0xF800
#define GREEN 0x07E0
#define YELLOW 0x07FF
#define PURPLE 0xF81F
#define CYAN 0xFFE0
#define WHITE 0xFFFF

#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define X A0
#define Y A1
#define address 0
// #define JB 8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
float p = 3.1415926;
float root2 = 1.41421356237;
byte ammo = 7;
byte asteroid_speed[9] = {100, 100, 100, 100, 100, 100, 100, 100, 100};
byte asteroid_speed_reset[9] = {100, 100, 100, 100, 100, 100, 100, 100, 100};
byte asteroid_posn_reset[9][2]{{0, 0}, {64, 0}, {128, 0}, {128, 64}, {128, 128}, {64, 128}, {0, 128}, {0, 64}, {0, 0}};
byte asteroid_posn[9][2]{{0, 0}, {64, 0}, {128, 0}, {128, 64}, {128, 128}, {64, 128}, {0, 128}, {0, 64}, {0, 0}};
bool asteroid_presence[9] = {1, 1, 0, 0, 0, 0, 0, 0, 0};
bool check[9] = {1, 1, 0, 0, 0, 0, 0, 0, 0};
bool flag[5] = {0, 1, 1, 1, 1};
byte asteroid_increment[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
bool diagonal_checker = 1;
bool axis_checker = 1;
byte sum = 1;
bool shield_flag = 0;
bool shield_ = 0;
unsigned long pt_scatter = 0;
unsigned long pt_shield[2] = {0, 0};
unsigned long pt_asteroids[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool reload_flag = 0;
unsigned long pt_reload = 0;
byte score = 0;
bool reload_indicator = 0;
byte dir()
{
  int x = analogRead(X);
  int y = analogRead(Y);
  if (x > 500 && x < 510 && y > 550)
  {
    return 1;
  }
  else if (x > 550 && y > 550)
  {
    return 2;
  }
  else if (y > 500 && y < 520 && x > 550)
  {
    return 3;
  }
  else if (y < 500 && x > 520)
  {
    return 4;
  }
  else if (x > 500 && x < 520 && y < 500)
  {
    return 5;
  }
  else if (x < 500 && y < 500)
  {
    return 6;
  }
  else if (x < 500 && y > 500 && y < 520)
  {
    return 7;
  }
  else if (x < 500 && y > 550)
  {
    return 8;
  }
  else
  {
    return 0;
  }
}
byte point(byte x)
{
  if (dir() == 1)
  {
    tft.drawLine(64, 64 - x, 64, 64, RED);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 1;
  }
  else if (dir() == 2)
  {
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, RED);
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 2;
  }
  else if (dir() == 3)
  {
    tft.drawLine(64 + x, 64, 64, 64, RED);
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 3;
  }
  else if (dir() == 4)
  {
    tft.drawLine(64 + (x / root2), 64 + (x / root2), 64, 64, RED);
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 4;
  }
  else if (dir() == 5)
  {
    tft.drawLine(64, 64 + x, 64, 64, RED);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 5;
  }
  else if (dir() == 6)
  {
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, RED);
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    return 6;
  }
  else if (dir() == 7)
  {
    tft.drawLine(64 - x, 64, 64, 64, RED);
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 7;
  }
  else if (dir() == 8)
  {
    tft.drawLine(64 - (x / root2), 64 - (x / root2), 64, 64, RED);
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK);
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK);
    return 8;
  }
  else
  {
    tft.drawLine(64, 0, 64, 128, BLACK);
    tft.drawLine(0, 64, 128, 64, BLACK);
    tft.drawLine(0, 0, 128, 128, BLACK);
    tft.drawLine(0, 128, 128, 0, BLACK);
    tft.drawLine(64 + (x / root2), 64 - (x / root2), 64, 64, BLACK); // case2
    tft.drawLine(64 - (x / root2), 64 + (x / root2), 64, 64, BLACK); // case6
    return 0;
  }
}
void setup()
{
  Serial.begin(9600);
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
  pinMode(E, INPUT);
  pinMode(F, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  // pinMode(JB, INPUT);
  tft.initR(INITR_BLACKTAB); // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);

  pinMode(A5, INPUT);
  randomSeed(analogRead(A5)); // random seed

  // EEPROM.update(address,0);/*to reset Highscore*/
  startUp();
  tft.fillScreen(ST7735_BLACK);
  stars();
  scoreSetup();
  playerSetup();
  ammoSetup();
  scoreDisplay(score);
  ammoDisplay(ammo);
}
void stars()
{
  for (int i = 0; i <= 128; i += random(15, 25))
  {
    for (int j = 0; j <= 128; j += random(15, 25))
    {
      if (random(0, 2))
      {
        tft.drawCircle(i, j, random(0, 2), random(0, 65536));
      }
    }
  }
}

void scoreSetup()
{
  tft.setTextSize(1);
  tft.setTextColor(CYAN);
  tft.setCursor(75, 5);
  tft.print("Score:");
}
void ammoSetup()
{
  tft.setTextSize(1);
  tft.setTextColor(CYAN);
  tft.setCursor(86, 123);
  tft.print("Ammo:");
}
void playerSetup()
{
  tft.fillCircle(64, 64, 5, PURPLE);
}
void scoreDisplay(byte k)
{
  tft.fillRect(110, 5, 18, 7, BLACK);
  tft.setTextColor(CYAN);
  tft.setCursor(110, 5);
  tft.print(k);
}
void ammoDisplay(byte a)
{
  tft.fillRect(115, 123, 5, 9, BLACK);
  tft.setTextColor(CYAN);
  tft.setCursor(115, 123);
  tft.print(a);
}
void startScreenSetup()
{
  tft.fillRect(4, 4, 120, 120, RED);
  tft.fillRect(6, 6, 116, 116, BLACK);
  stars();
  tft.setTextColor(CYAN);
  tft.setCursor(18, 10);
  tft.setTextSize(2);
  tft.print("ASTEROID");

  tft.setTextColor(PURPLE);
  tft.setCursor(24, 30);
  tft.setTextSize(2);
  tft.print("SHOOTER");
}
void startUp()
{
  startScreenSetup();

  while (digitalRead(E) && digitalRead(F))
  {

    tft.setTextColor(WHITE);
    tft.setCursor(18, 50);
    tft.setTextSize(1);
    tft.print("Press F to start.");

    tft.setTextColor(WHITE);
    tft.setCursor(18, 60);
    tft.print("Press E to-");
    tft.setCursor(60, 70);
    tft.print("-see rules");

    tft.setTextSize(1);
    tft.setCursor(8, 90);
    tft.print("HIGH");
    tft.setCursor(8, 100);
    tft.print("SCORE:");
    tft.setCursor(60, 90);
    tft.setTextSize(2);
    tft.print(EEPROM.read(address));
  }

  int f = 0;
  while (digitalRead(F))
  {
    if (f == 0)
    {
      tft.fillScreen(ST7735_BLACK);
      stars();
      tft.setTextColor(WHITE);
      tft.setCursor(4, 4);
      tft.setTextSize(1);
      tft.print("Rules:");
      tft.setCursor(4, 14);
      tft.print("1. Use the joystick to move the pointer.");
      tft.setCursor(4, 34);
      tft.print("2. Press C to shoot.");
      tft.setCursor(4, 44);
      tft.print("3. Press B to Reload.");
      tft.setCursor(4, 54);
      tft.print("4. Press A to use");
      tft.setCursor(4, 64);
      tft.print("SCATTER.");
      tft.setCursor(4, 74);
      tft.print("5. Press D to use");
      tft.setCursor(4, 84);
      tft.print("SHIELD.");
      tft.setCursor(4, 94);
      tft.print("6. Press F in game");
      tft.setCursor(4, 104);
      tft.print("to pause");
      tft.setCursor(4, 114);
      tft.print("7. Press F to");
      tft.setCursor(4, 124);
      tft.print("Continue to play");
      f++;
    }
  }
  pt_scatter = millis();   // scatter reset
  pt_shield[0] = millis(); // shield reset
  pt_shield[1] = millis(); // shield reset
}
void pause(byte currentScore, byte highScore)
{
  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(4, 4, 120, 120, RED);
  tft.fillRect(6, 6, 116, 116, BLACK);
  stars();
  tft.setTextColor(CYAN);
  tft.setCursor(30, 10);
  tft.setTextSize(2);
  tft.print("PAUSED");

  tft.setTextColor(WHITE);
  tft.setCursor(7, 50);
  tft.setTextSize(1);
  tft.print("Press F to continue");
  tft.setTextSize(1);

  tft.setCursor(8, 70);
  tft.print("CURRENT");
  tft.setCursor(8, 80);
  tft.print("SCORE:");
  tft.setCursor(60, 70);
  tft.setTextSize(2);
  tft.print(currentScore);

  tft.setTextSize(1);
  tft.setCursor(8, 90);
  tft.print("HIGH");
  tft.setCursor(8, 100);
  tft.print("SCORE:");
  tft.setCursor(60, 90);
  tft.setTextSize(2);

  tft.print(EEPROM.read(address));

  delay(100);
  while (digitalRead(F))
  {
  }
  tft.fillScreen(ST7735_BLACK);
  stars();
  scoreSetup();
  playerSetup();
  ammoSetup();
  scoreDisplay(score);
  ammoDisplay(ammo);
}
void GameOver(byte currentScore, byte highScore)
{

  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(4, 4, 120, 120, RED);
  tft.fillRect(6, 6, 116, 116, BLACK);
  stars();
  tft.setTextColor(CYAN);
  tft.setCursor(40, 10);
  tft.setTextSize(2);
  tft.print("GAME");

  tft.setTextColor(PURPLE);
  tft.setCursor(40, 30);
  tft.setTextSize(2);
  tft.print("OVER");

  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.setCursor(8, 70);
  tft.print("CURRENT");
  tft.setCursor(8, 80);
  tft.print("SCORE:");
  tft.setCursor(60, 70);
  tft.setTextSize(2);
  tft.print(currentScore);
  if (currentScore > highScore)
  {
    EEPROM.update(address, currentScore);
  }
  score = 0;
  ammo = 7;
  tft.setTextSize(1);
  tft.setCursor(8, 90);
  tft.print("HIGH");
  tft.setCursor(8, 100);
  tft.print("SCORE:");
  tft.setCursor(60, 90);
  tft.setTextSize(2);
  tft.print(EEPROM.read(address));

  tft.setTextSize(1);
  tft.setCursor(10, 110);
  tft.print("Press E to restart");
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      asteroid_posn[j][i] = asteroid_posn_reset[j][i];
    }
  }
  for (int i = 1; i <= 8; i++)
  {
    asteroid_speed[i] = asteroid_speed_reset[i];
  }
  asteroid_presence[0] = 1;
  asteroid_presence[1] = 1;
  asteroid_presence[2] = 0;
  asteroid_presence[3] = 0;
  asteroid_presence[4] = 0;
  asteroid_presence[5] = 0;
  asteroid_presence[6] = 0;
  asteroid_presence[7] = 0;
  asteroid_presence[8] = 0;
  check[0] = 1;
  check[1] = 1;
  check[2] = 0;
  check[3] = 0;
  check[4] = 0;
  check[5] = 0;
  check[6] = 0;
  check[7] = 0;
  check[8] = 0;
  diagonal_checker = 1;
  axis_checker = 1;
  sum = 1;
  while (digitalRead(E))
  {
  }
  delay(100);
  setup();
}

byte asteroids(byte t[9], bool checker[9], byte increment[9], bool sh)
{

  if (checker[1] && millis() - pt_asteroids[1] >= t[1])
  {
    asteroid_posn[1][1] += increment[1];
    pt_asteroids[1] = millis();
    tft.fillCircle(64, asteroid_posn[1][1] - increment[1], 3, BLACK);
    tft.fillCircle(64, asteroid_posn[1][1], 3, YELLOW);
  }
  if (asteroid_posn[1][1] >= 55)
  {
    tft.fillCircle(64, asteroid_posn[1][1], 3, BLACK);
    asteroid_posn[1][1] = asteroid_posn_reset[1][1];
    asteroid_presence[1] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 1;
  }
  if (checker[2] && millis() - pt_asteroids[2] >= t[2])
  {
    asteroid_posn[2][1] += increment[2];
    asteroid_posn[2][0] -= increment[2];
    pt_asteroids[2] = millis();
    tft.fillCircle(asteroid_posn[2][0] + increment[2], asteroid_posn[2][1] - increment[2], 3, BLACK);
    tft.fillCircle(asteroid_posn[2][0], asteroid_posn[2][1], 3, YELLOW);
  }
  if (asteroid_posn[2][0] <= 70)
  {
    tft.fillCircle(asteroid_posn[2][0], asteroid_posn[2][1], 3, BLACK);
    asteroid_posn[2][0] = asteroid_posn_reset[2][0];
    asteroid_posn[2][1] = asteroid_posn_reset[2][1];
    asteroid_presence[2] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 2;
  }

  if (checker[3] && millis() - pt_asteroids[3] >= t[3])
  {
    asteroid_posn[3][0] -= increment[3];
    pt_asteroids[3] = millis();
    tft.fillCircle(asteroid_posn[3][0] + increment[3], 64, 3, BLACK);
    tft.fillCircle(asteroid_posn[3][0], 64, 3, YELLOW);
  }
  if (asteroid_posn[3][0] <= 73)
  {
    tft.fillCircle(asteroid_posn[3][0], 64, 3, BLACK);
    asteroid_posn[3][0] = asteroid_posn_reset[3][0];
    asteroid_presence[3] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 3;
  }

  if (checker[4] && millis() - pt_asteroids[4] >= t[4])
  {
    asteroid_posn[4][1] -= increment[4];
    asteroid_posn[4][0] -= increment[4];
    pt_asteroids[4] = millis();
    tft.fillCircle(asteroid_posn[4][0] + increment[4], asteroid_posn[4][1] + increment[4], 3, BLACK);
    tft.fillCircle(asteroid_posn[4][0], asteroid_posn[4][1], 3, YELLOW);
  }
  if (asteroid_posn[4][1] <= 70)
  {
    tft.fillCircle(asteroid_posn[4][0], asteroid_posn[4][1], 3, BLACK);
    asteroid_posn[4][0] = asteroid_posn_reset[4][0];
    asteroid_posn[4][1] = asteroid_posn_reset[4][1];
    asteroid_presence[4] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 4;
  }

  if (checker[5] && millis() - pt_asteroids[5] >= t[5])
  {
    asteroid_posn[5][1] -= increment[5];
    pt_asteroids[5] = millis();
    tft.fillCircle(64, asteroid_posn[5][1] + increment[5], 3, BLACK);
    tft.fillCircle(64, asteroid_posn[5][1], 3, YELLOW);
  }
  if (asteroid_posn[5][1] <= 75)
  {
    tft.fillCircle(64, asteroid_posn[5][1], 3, BLACK);
    asteroid_posn[5][1] = asteroid_posn_reset[5][1];
    asteroid_presence[5] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 5;
  }

  if (checker[6] && millis() - pt_asteroids[6] >= t[6])
  {
    asteroid_posn[6][1] -= increment[6];
    asteroid_posn[6][0] += increment[6];
    pt_asteroids[6] = millis();
    tft.fillCircle(asteroid_posn[6][0] - increment[6], asteroid_posn[6][1] + increment[6], 3, BLACK);
    tft.fillCircle(asteroid_posn[6][0], asteroid_posn[6][1], 3, YELLOW);
  }
  if (asteroid_posn[6][1] <= 70)
  {
    tft.fillCircle(asteroid_posn[6][0], asteroid_posn[6][1], 3, BLACK);
    asteroid_posn[6][0] = asteroid_posn_reset[6][0];
    asteroid_posn[6][1] = asteroid_posn_reset[6][1];
    asteroid_presence[6] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 6;
  }

  if (checker[7] && millis() - pt_asteroids[7] >= t[7])
  {
    asteroid_posn[7][0] += increment[7];
    pt_asteroids[7] = millis();
    tft.fillCircle(asteroid_posn[7][0] - increment[7], 64, 3, BLACK);
    tft.fillCircle(asteroid_posn[7][0], 64, 3, YELLOW);
  }
  if (asteroid_posn[7][0] >= 54)
  {
    tft.fillCircle(asteroid_posn[7][0], 64, 3, BLACK);
    asteroid_posn[7][0] = asteroid_posn_reset[7][0];
    asteroid_presence[7] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 7;
  }

  if (checker[8] && millis() - pt_asteroids[8] >= t[8])
  {
    asteroid_posn[8][1] += increment[8];
    asteroid_posn[8][0] += increment[8];
    pt_asteroids[8] = millis();
    tft.fillCircle(asteroid_posn[8][0] - increment[8], asteroid_posn[8][1] - increment[8], 3, BLACK);
    tft.fillCircle(asteroid_posn[8][0], asteroid_posn[8][1], 3, YELLOW);
  }
  if (asteroid_posn[8][1] >= 57)
  {
    tft.fillCircle(asteroid_posn[8][0], asteroid_posn[8][1], 3, BLACK);
    asteroid_posn[8][0] = asteroid_posn_reset[8][0];
    asteroid_posn[8][1] = asteroid_posn_reset[8][1];
    asteroid_presence[8] = 0;
    if (!sh)
      GameOver(score, EEPROM.read(address));
    return 8;
  }
}
bool reload(int t_reload)
{
  if (!digitalRead(B) && !reload_flag && ammo == 0)
  {
    reload_flag = 1;
    pt_reload = millis();
  }
  if (millis() - pt_reload >= t_reload && reload_flag)
  {
    ammo = 7;
    reload_flag = 0;
    ammoDisplay(ammo);
  }
  if (reload_flag && !reload_indicator)
  {
    tft.fillCircle(64, 64, 5, RED);
    reload_indicator = 1;
  }
  else if (!reload_flag && reload_indicator)
  {
    tft.fillCircle(64, 64, 5, PURPLE);
    reload_indicator = 0;
  }
  if (ammo == 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
int shooter(int direction, bool r)
{
  if (digitalRead(C) == 0 && r)
  {

    if (direction == 1)
    {
      tft.drawLine(64, 64, 64, 0, RED);
      delay(150);
      tft.drawLine(64, 64, 64, 0, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 1;
    }
    else if (direction == 2)
    {
      tft.drawLine(64, 64, 128, 0, RED);
      delay(150);
      tft.drawLine(64, 64, 128, 0, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 2;
    }
    else if (direction == 3)
    {
      tft.drawLine(64, 64, 128, 64, RED);
      delay(150);
      tft.drawLine(64, 64, 128, 64, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 3;
    }
    else if (direction == 4)
    {
      tft.drawLine(64, 64, 128, 128, RED);
      delay(150);
      tft.drawLine(64, 64, 128, 128, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 4;
    }
    else if (direction == 5)
    {
      tft.drawLine(64, 64, 64, 128, RED);
      delay(150);
      tft.drawLine(64, 64, 64, 128, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 5;
    }
    else if (direction == 6)
    {
      tft.drawLine(64, 64, 0, 128, RED);
      delay(150);
      tft.drawLine(64, 64, 0, 128, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 6;
    }
    else if (direction == 7)
    {
      tft.drawLine(64, 64, 0, 64, RED);
      delay(150);
      tft.drawLine(64, 64, 0, 64, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 7;
    }
    else if (direction == 8)
    {
      tft.drawLine(64, 64, 0, 0, RED);
      delay(150);
      tft.drawLine(64, 64, 0, 0, BLACK);
      ammo--;
      ammoDisplay(ammo);
      return 8;
    }
  }
  else
  {
    return NULL;
  }
}
bool collision(byte k)
{
  if (k == 1 && asteroid_presence[1])
  {
    // tft.fillCircle(64, asteroid_posn[1][1], 3, RED);
    tft.fillCircle(64, asteroid_posn[1][1], 3, BLACK);
    asteroid_posn[1][1] = asteroid_posn_reset[1][1];
    asteroid_presence[1] = 0;
    score++;
    return 1;
  }
  else if (k == 2 && asteroid_presence[2])
  {
    tft.fillCircle(asteroid_posn[2][0], asteroid_posn[2][1], 3, BLACK);
    asteroid_posn[2][0] = asteroid_posn_reset[2][0];
    asteroid_posn[2][1] = asteroid_posn_reset[2][1];
    asteroid_presence[2] = 0;
    score++;
    return 1;
  }
  else if (k == 3 && asteroid_presence[3])
  {
    tft.fillCircle(asteroid_posn[3][0], 64, 3, BLACK);
    asteroid_posn[3][0] = asteroid_posn_reset[3][0];
    asteroid_presence[3] = 0;
    score++;
    return 1;
  }
  else if (k == 4 && asteroid_presence[4])
  {
    tft.fillCircle(asteroid_posn[4][0], asteroid_posn[4][1], 3, BLACK);
    asteroid_posn[4][0] = asteroid_posn_reset[4][0];
    asteroid_posn[4][1] = asteroid_posn_reset[4][1];
    asteroid_presence[4] = 0;
    score++;
    return 1;
  }
  else if (k == 5 && asteroid_presence[5])
  {
    tft.fillCircle(64, asteroid_posn[5][1], 3, BLACK);
    asteroid_posn[5][1] = asteroid_posn_reset[5][1];
    asteroid_presence[5] = 0;
    score++;
    return 1;
  }
  else if (k == 6 && asteroid_presence[6])
  {
    tft.fillCircle(asteroid_posn[6][0], asteroid_posn[6][1], 3, BLACK);
    asteroid_posn[6][0] = asteroid_posn_reset[6][0];
    asteroid_posn[6][1] = asteroid_posn_reset[6][1];
    asteroid_presence[6] = 0;
    score++;
    return 1;
  }
  else if (k == 7 && asteroid_presence[7])
  {
    tft.fillCircle(asteroid_posn[7][0], 64, 3, BLACK);
    asteroid_posn[7][0] = asteroid_posn_reset[7][0];
    asteroid_presence[7] = 0;
    score++;
    return 1;
  }
  else if (k == 8 && asteroid_presence[8])
  {
    tft.fillCircle(asteroid_posn[8][0], asteroid_posn[8][1], 3, BLACK);
    asteroid_posn[8][0] = asteroid_posn_reset[8][0];
    asteroid_posn[8][1] = asteroid_posn_reset[8][1];
    asteroid_presence[8] = 0;
    score++;
    return 1;
  }
  else
  {
    return 0;
  }
}

bool scatter(int t)
{
  if ((millis() - pt_scatter >= t))
  {
    tft.drawCircle(5, 124, 3, BLUE);
    if (!digitalRead(A))
    {
      tft.drawLine(64, 64, 64, 0, BLUE);
      tft.drawLine(64, 64, 128, 0, BLUE);
      tft.drawLine(64, 64, 128, 64, BLUE);
      tft.drawLine(64, 64, 128, 128, BLUE);
      tft.drawLine(64, 64, 64, 128, BLUE);
      tft.drawLine(64, 64, 0, 128, BLUE);
      tft.drawLine(64, 64, 0, 64, BLUE);
      tft.drawLine(64, 64, 0, 0, BLUE);
      delay(150);
      tft.drawLine(64, 64, 64, 0, BLACK);
      tft.drawLine(64, 64, 128, 0, BLACK);
      tft.drawLine(64, 64, 128, 64, BLACK);
      tft.drawLine(64, 64, 128, 128, BLACK);
      tft.drawLine(64, 64, 64, 128, BLACK);
      tft.drawLine(64, 64, 0, 128, BLACK);
      tft.drawLine(64, 64, 0, 64, BLACK);
      tft.drawLine(64, 64, 0, 0, BLACK);
      if (asteroid_presence[1])
      {
        tft.fillCircle(64, asteroid_posn[1][1], 3, BLACK);
        asteroid_posn[1][1] = asteroid_posn_reset[1][1];
        asteroid_presence[1] = 0;
        score++;
      }
      if (asteroid_presence[2])
      {
        tft.fillCircle(asteroid_posn[2][0], asteroid_posn[2][1], 3, BLACK);
        asteroid_posn[2][0] = asteroid_posn_reset[2][0];
        asteroid_posn[2][1] = asteroid_posn_reset[2][1];
        asteroid_presence[2] = 0;
        score++;
      }
      if (asteroid_presence[3])
      {
        tft.fillCircle(asteroid_posn[3][0], 64, 3, BLACK);
        asteroid_posn[3][0] = asteroid_posn_reset[3][0];
        asteroid_presence[3] = 0;
        score++;
      }
      if (asteroid_presence[4])
      {
        tft.fillCircle(asteroid_posn[4][0], asteroid_posn[4][1], 3, BLACK);
        asteroid_posn[4][0] = asteroid_posn_reset[4][0];
        asteroid_posn[4][1] = asteroid_posn_reset[4][1];
        asteroid_presence[4] = 0;
        score++;
      }
      if (asteroid_presence[5])
      {
        tft.fillCircle(64, asteroid_posn[5][1], 3, BLACK);
        asteroid_posn[5][1] = asteroid_posn_reset[5][1];
        asteroid_presence[5] = 0;
        score++;
      }
      if (asteroid_presence[6])
      {
        tft.fillCircle(asteroid_posn[6][0], asteroid_posn[6][1], 3, BLACK);
        asteroid_posn[6][0] = asteroid_posn_reset[6][0];
        asteroid_posn[6][1] = asteroid_posn_reset[6][1];
        asteroid_presence[6] = 0;
        score++;
      }
      if (asteroid_presence[7])
      {
        tft.fillCircle(asteroid_posn[7][0], 64, 3, BLACK);
        asteroid_posn[7][0] = asteroid_posn_reset[7][0];
        asteroid_presence[7] = 0;
        score++;
      }
      if (asteroid_presence[8])
      {
        tft.fillCircle(asteroid_posn[8][0], asteroid_posn[8][1], 3, BLACK);
        asteroid_posn[8][0] = asteroid_posn_reset[8][0];
        asteroid_posn[8][1] = asteroid_posn_reset[8][1];
        asteroid_presence[8] = 0;
        score++;
      }
      pt_scatter = millis();
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    tft.drawCircle(5, 124, 3, BLACK);
    return 0;
  }
}
bool shield(int shieldActivation, int shieldDuration)
{
  if ((millis() - pt_shield[0] >= shieldActivation))
  {
    tft.drawCircle(5, 115, 3, CYAN);
    shield_flag = 1;
  }
  else
  {
    tft.drawCircle(5, 115, 3, BLACK);
  }

  if (!digitalRead(D) && shield_flag)
  {
    shield_flag = 0;
    tft.drawCircle(64, 64, 7, CYAN);
    shield_ = 1;
    pt_shield[0] = millis(); //
    pt_shield[1] = millis();
  }
  if ((millis() - pt_shield[1] >= shieldDuration))
  {
    tft.drawCircle(64, 64, 7, BLACK);
    shield_ = 0;
  }
  return shield_;
}
void random_spawn()
{
  byte random_check[9] = {random(0, 2), random(0, 2), random(0, 2), random(0, 2), random(0, 2), random(0, 2), random(0, 2), random(0, 2), random(0, 2)};
  byte a = random(1, 9);
  asteroid_presence[a] = 1;
  if (asteroid_presence[1] && random_check[1] == 1)
  {
    random_check[1] == 0;
  }
  if (asteroid_presence[2] && random_check[2] == 1)
  {
    random_check[2] == 0;
  }
  if (asteroid_presence[3] && random_check[3] == 1)
  {
    random_check[3] == 0;
  }
  if (asteroid_presence[4] && random_check[4] == 1)
  {
    random_check[4] == 0;
  }
  if (asteroid_presence[5] && random_check[5] == 1)
  {
    random_check[5] == 0;
  }
  if (asteroid_presence[6] && random_check[6] == 1)
  {
    random_check[6] == 0;
  }
  if (asteroid_presence[7] && random_check[7] == 1)
  {
    random_check[7] == 0;
  }
  if (asteroid_presence[8] && random_check[8] == 1)
  {
    random_check[8] == 0;
  }
}
void diagonal_spawn()
{
  if (asteroid_presence[2] == 0 && asteroid_presence[4] == 0 && asteroid_presence[6] == 0 && asteroid_presence[8] == 0 && diagonal_checker == 1)
  {
    asteroid_presence[2] = 1;
    asteroid_presence[4] = 1;
    asteroid_presence[6] = 1;
    asteroid_presence[8] = 1;
    diagonal_checker = 0;
  }
  else if (diagonal_checker == 0)
  {
    axis_spawn();
  }
}
void axis_spawn()
{
  if (asteroid_presence[1] == 0 && asteroid_presence[3] == 0 && asteroid_presence[5] == 0 && asteroid_presence[7] == 0 && axis_checker == 1)
  {
    asteroid_presence[1] = 1;
    asteroid_presence[3] = 1;
    asteroid_presence[5] = 1;
    asteroid_presence[7] = 1;
    axis_checker = 0;
  }
  else if (asteroid_presence[1] == 0 && asteroid_presence[3] == 0 && asteroid_presence[5] == 0 && asteroid_presence[7] == 0 && axis_checker == 0)
  {
    random_spawn();
  }
}
void one_by_one()
{
  if (asteroid_presence[1] == 0 && check[1] == 1)
  {
    asteroid_presence[2] = 1;
    check[1] = 0;
    check[2] = 1;
  }
  else if (asteroid_presence[2] == 0 && check[2] == 1)
  {
    asteroid_presence[3] = 1;
    check[2] = 0;
    check[3] = 1;
  }
  else if (asteroid_presence[3] == 0 && check[3] == 1)
  {
    asteroid_presence[4] = 1;
    check[3] = 0;
    check[4] = 1;
  }
  else if (asteroid_presence[4] == 0 && check[4] == 1)
  {
    asteroid_presence[5] = 1;
    check[4] = 0;
    check[5] = 1;
  }
  else if (asteroid_presence[5] == 0 && check[5] == 1)
  {
    asteroid_presence[6] = 1;
    check[5] = 0;
    check[6] = 1;
  }
  else if (asteroid_presence[6] == 0 && check[6] == 1)
  {
    asteroid_presence[7] = 1;
    check[6] = 0;
    check[7] = 1;
  }
  else if (asteroid_presence[7] == 0 && check[7] == 1)
  {
    asteroid_presence[8] = 1;
    check[7] = 0;
    check[8] = 1;
  }
  else if (asteroid_presence[8] == 0 && check[8] == 1)
  {
    sum = 0;
    diagonal_spawn();
    check[8] = 0;
    check[0] = 1;
  }
  if (sum == 0)
  {
    diagonal_spawn();
  }
}
void diamonds_1()
{
  if (asteroid_presence[1] == 0 && asteroid_presence[3] == 0 && asteroid_presence[5] == 0 && asteroid_presence[7] == 0)
  {
    for (int i = 0; i <= 8; i++)
    {
      asteroid_speed[i] = 60;
    }
    asteroid_presence[1] = 1;
    asteroid_presence[3] = 1;
    asteroid_presence[5] = 1;
    asteroid_presence[7] = 1;
  }
}
void diamonds_2()
{
  if (asteroid_presence[2] == 0 && asteroid_presence[4] == 0 && asteroid_presence[6] == 0 && asteroid_presence[8] == 0)
  {
    for (int i = 0; i <= 8; i++)
    {
      asteroid_speed[i] = 60;
    }
    asteroid_presence[2] = 1;
    asteroid_presence[4] = 1;
    asteroid_presence[6] = 1;
    asteroid_presence[8] = 1;
  }
}
void wall_1()
{
  if (asteroid_presence[8] == 0 && asteroid_presence[1] == 0 && asteroid_presence[2] == 0)
  {
    int b = random(60, 80);
    for (int i = 1; i <= 2; i++)
    {
      asteroid_speed[i] = b;
    }
    asteroid_speed[8] = b;
    asteroid_presence[8] = 1;
    asteroid_presence[1] = 1;
    asteroid_presence[2] = 1;
  }
}
void wall_2()
{
  if (asteroid_presence[8] == 0 && asteroid_presence[7] == 0 && asteroid_presence[6] == 0)
  {
    int b = random(40, 60);
    for (int i = 6; i <= 8; i++)
    {
      asteroid_speed[i] = b;
    }
    asteroid_presence[8] = 1;
    asteroid_presence[7] = 1;
    asteroid_presence[6] = 1;
  }
}
void wall_3()
{
  if (asteroid_presence[6] == 0 && asteroid_presence[5] == 0 && asteroid_presence[4] == 0)
  {
    int b = random(40, 60);
    for (int i = 4; i <= 6; i++)
    {
      asteroid_speed[i] = b;
    }
    asteroid_presence[5] = 1;
    asteroid_presence[4] = 1;
    asteroid_presence[6] = 1;
  }
}
void wall_4()
{
  if (asteroid_presence[2] == 0 && asteroid_presence[3] == 0 && asteroid_presence[4] == 0)
  {
    int b = random(40, 60);
    for (int i = 2; i <= 4; i++)
    {
      asteroid_speed[i] = b;
    }
    asteroid_presence[2] = 1;
    asteroid_presence[4] = 1;
    asteroid_presence[3] = 1;
  }
}
void Runtime()
{
  if (score <= 220)
  {
    if (score <= 79)
    {
      one_by_one();
    }
    if (score % 10 == 0 && score != 120 && score != 80)
    {
      for (int i = 0; i <= 8; i++)
      {
        asteroid_speed[i] = random(60, 100);
      }
      if (score > 140)
      {
        for (int i = 0; i <= 8; i++)
        {
          asteroid_speed[i] = random(40, 80);
        }
      }
    }
    if (score >= 80 & score <= 89)
    {
      if (flag[1] == 1)
      {
        diamonds_1();
        flag[1] = 0;
      }
      else if (flag[2] == 1)
      {
        diamonds_2();
      }
    }
    if (score >= 90)
    {
      one_by_one();
    }
    if (score >= 120 && score <= 132)
    {
      byte a = random(1, 5);
      if (a == 1)
        wall_1();
      else if (a == 2)
        wall_2();
      else if (a == 3)
        wall_3();
      else if (a == 4)
        wall_4();
    }
    if (score > 132)
    {
      one_by_one();
    }
  }
  else
  {
    if (!flag[0])
    {
      for (int i = 0; i <= 8; i++)
      {
        asteroid_speed[i] = 50;
      }
      flag[0] = 1;
    }
    random_spawn();
  }
}
void loop()
{
  // Serial.println(EEPROM.read(address));
  if (digitalRead(F) == 0)
  {
    pause(score, EEPROM.read(address));
    delay(100);
  }
  if (digitalRead(E) == 0)
  {
    GameOver(score, EEPROM.read(address));
    delay(100);
  }
  // Serial.println(shooter(point(15)));
  if (collision(shooter(point(15), reload(500))) || scatter(20000))
  {
    scoreDisplay(score);
  }
  Runtime();

  // Serial.println(shield(2000, 1000));
  asteroids(asteroid_speed, asteroid_presence, asteroid_increment, shield(20000, 5000));
}
