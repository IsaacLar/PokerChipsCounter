#include <Arduino.h>
#include <LiquidCrystal.h>

//Initialse LCD object with communication pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Pin used to bypass potentiometer usage
const int contrast = 13;

//Switch pins
const int pinOne = A3;
const int pinTwo = A2;
const int pinThree = A1;
const int pinFour = A0;

//Switch state inits
int oneState = 0;
int twoState = 0;
int threeState = 0;
int fourState = 0;

//Function to allow user to enter number of players 
void enterPlayerNums(){
  lcd.clear();
  lcd.print("Number of");
  lcd.setCursor(0,1);
  lcd.print("Players: 2");
  //Listen for input
}

void setup() {
  analogWrite(contrast, 130);
  //Define the LCD as 16x2 screen
  lcd.begin(16, 2);

  Serial.begin(9600);

  //Assign all switch pins as inputs;
  pinMode(pinOne, INPUT);
  pinMode(pinTwo, INPUT);
  pinMode(pinThree, INPUT);
  pinMode(pinFour, INPUT);

  //Beginning message
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("Poker Bot! :D");

  //Wait for user to press button before continuing
  while (true){
    oneState = analogRead(pinOne);
    twoState = analogRead(pinTwo);
    threeState = analogRead(pinThree);
    fourState = analogRead(pinFour);

    //Break loop once any button pressed
    if (oneState > 1000 || twoState > 1000 || threeState > 1000 || fourState > 1000){
      break;
    }

    delay(50);
  }

  enterPlayerNums();
}



void loop() {
  oneState = analogRead(pinOne);
  twoState = analogRead(pinTwo);
  threeState = analogRead(pinThree);
  fourState = analogRead(pinFour);

  if (oneState > 1000){
    Serial.print("One pressed\t");
    Serial.print(oneState);
    Serial.print(" ");
    Serial.print(twoState);
    Serial.print(" ");
    Serial.print(threeState);
    Serial.print(" ");
    Serial.println(fourState);
  } else if (twoState > 1000){
    Serial.print("Two pressed\t");
    Serial.print(oneState);
    Serial.print(" ");
    Serial.print(twoState);
    Serial.print(" ");
    Serial.print(threeState);
    Serial.print(" ");
    Serial.println(fourState);
  } else if (threeState > 1000){
    Serial.print("Three pressed\t");
    Serial.print(oneState);
    Serial.print(" ");
    Serial.print(twoState);
    Serial.print(" ");
    Serial.print(threeState);
    Serial.print(" ");
    Serial.println(fourState);
  } else if (fourState > 1000){
    Serial.print("Four pressed\t");
    Serial.print(oneState);
    Serial.print(" ");
    Serial.print(twoState);
    Serial.print(" ");
    Serial.print(threeState);
    Serial.print(" ");
    Serial.println(fourState);
  } else{
    Serial.print("Nothing pressed\t");
    Serial.print(oneState);
    Serial.print(" ");
    Serial.print(twoState);
    Serial.print(" ");
    Serial.print(threeState);
    Serial.print(" ");
    Serial.println(fourState);
  }
  delay(30);
}