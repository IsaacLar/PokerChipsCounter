#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>

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

int numOfPlayers = 2;

//Player struct to hold player name and current balance
struct Player{
  char name[4];
  int balance;
};

//Null pointer to become array of players
//VARIABLE DATA TYPE - DELETE BETWEEN ROUNDS
Player* players = nullptr;

//Function to allow user to enter number of players 
void enterPlayerNums(){
  lcd.clear();
  lcd.print("Number of");
  lcd.setCursor(0,1);
  lcd.print("Players: ");
  lcd.print(numOfPlayers);
  delay(300);
  //Listen for input
  while (true){
    oneState = analogRead(pinOne);
    twoState = analogRead(pinTwo);
    threeState = analogRead(pinThree);
    fourState = analogRead(pinFour);

    //Button 2 = Increase
    if(twoState > 1000){
      //Increment number of players
      numOfPlayers++;
      //Limit max number of players to 9
      if (numOfPlayers > 9){
        numOfPlayers = 9;
      }
      //Update LCD screen
      lcd.setCursor(9,1);
      lcd.print(numOfPlayers);
    //Button 3 = Decrease
    } else if(threeState > 1000){
      //Decrement number of players
      numOfPlayers--;
      //Limit minimum number of players to 2
      if (numOfPlayers < 2){
        numOfPlayers = 2;
      }
      //Update LCD screen
      lcd.setCursor(9,1);
      lcd.print(numOfPlayers);
    //Button 4 = continue
    } else if(fourState > 1000){
      break;
    }
    delay(100);
  }

  //Define a new Player array of size (whatever the user entered)
  players = new Player[numOfPlayers];

  //65 = A, 90 = Z
  //Loop through for each player
  for (int i = 0; i < numOfPlayers; i++){
    //Initialise name as AAA
    int name[3] = {65, 65, 65};
    //Variable which holds which character is currently being edited - inits at character one
    int effectedChar = 0;
    lcd.clear();
    //LCD OUTPUT:  P{i} Name: AAA
    lcd.print("P");
    lcd.print(i+1);
    lcd.print(" Name: ");
    lcd.print((char)name[0]);
    lcd.print((char)name[1]);
    lcd.print((char)name[2]);
    lcd.setCursor(9,1);
    //Cursor to show which letter is being changed
    lcd.print("-");
    bool playerNameEntered = false;

    while (!playerNameEntered){
      oneState = analogRead(pinOne);
      twoState = analogRead(pinTwo);
      threeState = analogRead(pinThree);
      fourState = analogRead(pinFour);
      //Button 1 = Confirm
      if (oneState > 1000){
        playerNameEntered = true;
      //Button 2 = Increment value
      } else if (twoState > 1000){
        //Increment character value
        name[effectedChar]++;
        //Loop round if necessary
        if (name[effectedChar] > 90){
          name[effectedChar] = 65;
        }
        //Update LCD
        lcd.setCursor(9+effectedChar,0);
        lcd.print((char)name[effectedChar]);

      //Button 3 = Decrement value
      } else if (threeState > 1000){
        //Decrement character value
        name[effectedChar]--;
        //Loop round if necessary
        if (name[effectedChar] < 65){
          name[effectedChar] = 90;
        }
        //Update LCD
        lcd.setCursor(9+effectedChar, 0);
        lcd.print((char)name[effectedChar]);
    
      //Button 4 = Move right
      } else if (fourState > 1000){
        //Clear the current dash
        lcd.setCursor(9+effectedChar,1);
        lcd.print(" ");
        //Increment dash position
        effectedChar++;
        //Loop back if necessary
        if (effectedChar > 2){
          effectedChar = 0;
        }
        //Draw the new dash position
        lcd.setCursor(9+effectedChar, 1);
        lcd.print("-");
      }
      delay(150);
    }
    //Pass the player "name to the struct"
    for (int j = 0; j < 3; j++){
      players[i].name[j] = (char)name[j];
    }
    //Add null terminator to the end of player name
    players[i].name[3] = '\0';    
  }
  
  for (int i = 0; i < numOfPlayers; i++){
    Serial.println(players[i].name);
  }
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