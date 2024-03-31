![DIsplay2](https://github.com/Coolexguy/ArduGem-GC-2024/assets/164471213/eb2b21fe-af4b-438f-9f7a-cf8809d7c8e9)


# Video Demonstration:-
https://github.com/Coolexguy/GC_24_ARDUGEM_38/assets/164471213/b0cc7219-a155-4c07-a2fb-3406df0440ac

# Asteroid Shooter :-
We developed it for  ArduGem GC'24(IIT Bhubaneswar's inter-branch competition).

# Description:-
+ This project is based on the concept of Arcade style survival games.
+ It is a game of pace, accuracy and reflexes.
+ This game is written such that it works even on the lowest end Hardware(Arduino UNO).
+ The game's design and the fast paced nature makes it immersive and engaging.
# Dependecies:-
+ <SPI.h>
+ <Adafruit_GFX.h>
+ <Adafruit_ST7735.h>
+ <EEPROM.h>

# Control Scheme:-
+ The game starts with a startup screen having it's Name and Highscore of the game. 
+ Press '**F**' to start the game.
+ Pressing '**E**' will take you to the rules page.
+ Asteroids will be spawning randomly from different directions in the game and the player shall aim and shoot the asteroids to destroy them.
+ Use the joystick to start aiming at the spawning asteroids.
+ Press '**B**' to shoot the asteroid.
+ Only **ONE** asteroid can be shot down at a time.
+ The player has to shoot down the asteroids in order to survive,there is an aim assist to indicate the direction of fire.
+ When the player runs out of ammo, press the button '**B**' on the Joystick Shield to reload.
+ The player turns **Red** in order to indicate the reloading sequence.
### Powerups:-
+ Press '**A**' to use **Scatter Shot** which shoots in all possible directions.
+ Every 25 seconds the player can see a blue little circle on the left corner,which indicates **SCATTER SHOT**.
+ Press '**D**' to activate **SHIELD**.
+ Every 20 seconds the player can see a little cyan circle on the left corner above the blue circle which indicates **SHIELD**
+ The **SHIELD** lasts for 5 seconds which makes the player invincible.

# Implementation of High Score:-
+ The high score data is being held in EEPROM(Electrically Erasable Programmable Read-Only Memory).
+ This requies the dependency <EEPROM.h>
+ This allows us for the high score to be maintained on the device even if the power is lost as the data is being returned to the EEPROM.

# Circuit and it's Components:-
![WhatsApp Image 2024-03-28 at 18 45 41_9ac40f2a](https://github.com/Coolexguy/ArduGem-GC-2024/assets/164471213/b7f124eb-9f84-4dfe-99d7-363cd5b28971)
### Arduino UNO:-
+ It is the brains of the operation.
+ We will mainly be using SPI protocol to communicate with the display.
+ The Arduino's digital pins **2** to **7** are being used for the push button switches.
+ The analog pin **A0** and **A1** will be taking input from the joystick using Arduino's 10 bit ADC.
+ The Arduino is being powered through the USB-A port.

### ST7735 TFT LCD Display:-
+ This display uses SPI protocol.
+ <Adafruit_GFX.h> <Adafruit_ST7735.h>  <SPI.h> These are the three dependecies used for the display.

### Joystick Shield:-
+ The Joystick Shield consists majorly of two parts:-
  joystick module and push buttons.
+ The joystick module gives two analog value pertaining to both the axes through which we can understand the direction joystick is being pointed to.
+ The joystick switch button(The button which activates when you press the joystick) is not being used as the Joystick Shield connects that button to digital pin 8 but it is being used for the SPI protocol for the display.

### Push button switches:-
+ There are six push button switches labeled from '**A**' to '**F**'.
+ The joystick module uses pull up resistors to pull the switches to a logical **HIGH**.
+ When the switches are activated, they are connected to the ground to pull it to logical **LOW**.
