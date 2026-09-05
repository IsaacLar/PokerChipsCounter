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

bool onePressed = false;
bool twoPressed = false;
bool threePressed = false;
bool fourPressed = false;

//Total number of players - default 2
int numOfPlayers = 2;

//Cutsom pixel array for £
byte poundSign[8] = {
  B01100,
  B10010,
  B10000,
  B11100,
  B10000,
  B10010,
  B11110,
  B00000
};

//Player struct to hold player name and current balance
struct Player{
  char name[4];
  long balance;
};

//Null pointer to become array of players
//VARIABLE DATA TYPE - DELETE BETWEEN ROUNDS
Player* players = nullptr;

void selectDealer(){
  lcd.clear();
  lcd.print("Select Dealer:");
  lcd.setCursor(0,1);
  lcd.print("P1: ");
  lcd.print(players[0].name);
  int dealer = 0;

  while (true){
    oneState = analogRead(pinOne);
    twoState = analogRead(pinTwo);
    threeState = analogRead(pinThree);

    //Confirm
    if (oneState < 200 && !onePressed){
      onePressed = true;
      break;
    } else if (oneState > 1000 && onePressed){
      onePressed = false;
      //Previous player
    } else if (twoState > 1000 && !twoPressed){
      dealer--;
      if (dealer < 0){
        dealer = numOfPlayers-1;
      }
      lcd.setCursor(1,1);
      lcd.print(dealer+1);
      lcd.print(": ");
      lcd.print(players[dealer].name);
      twoPressed = true;
    } else if (twoState < 200 && twoPressed){
      twoPressed = false;
    //Next player
    } else if (threeState > 1000 && !threePressed){
      dealer++;
      if (dealer == numOfPlayers){
        dealer = 0;
      }
      lcd.setCursor(1,1);
      lcd.print(dealer+1);
      lcd.print(": ");
      lcd.print(players[dealer].name);
      threePressed = true;
    } else if (threeState < 200 && threePressed){
      threePressed = false;
    }
    delay(30);
  }
  Serial.print("Dealer: ");
  Serial.print(players[dealer].name);
}

//Function to enter how much money each player starts with
void enterStartingAllowance(){
  lcd.clear();
  lcd.print("Starting ");
  lcd.write(byte(0));
  lcd.print("000100");
  lcd.setCursor(13,1);
  lcd.print("-");

  //Store allowance as array so each number can be changed individually
  int startingAllowance[6] = {0,0,0,1,0,0};
  //Start on the 1
  int effectedNum = 3;

  bool numEntered = false;

  while(!numEntered){
    oneState = analogRead(pinOne);
    twoState = analogRead(pinTwo);
    threeState = analogRead(pinThree);
    fourState = analogRead(pinFour);

    //Button 1 = Confirm
    if (oneState > 1000 && !onePressed){
      //Check to ensure entered number !0
      bool inputAllowed = false;
      for (int i = 0; i < 6; i++){
        if (startingAllowance[i] != 0){
          inputAllowed = true;
          break;
        }
      }
      onePressed = true;
      //Only allow continuation if input checked
      if (inputAllowed){
        numEntered = true;
      }
    } else if (oneState < 200 && onePressed){
      onePressed = false;
    //Button 2 = Increment value
    } else if (twoState > 1000 && !twoPressed){
      //Increment character value
      startingAllowance[effectedNum]++;
      //Loop round if necessary
      if (startingAllowance[effectedNum] > 9){
        startingAllowance[effectedNum] = 0;
      }
      //Update LCD
      lcd.setCursor(10+effectedNum,0);
      lcd.print(startingAllowance[effectedNum]);
      twoPressed = true;
    } else if (twoState < 200 && twoPressed){
      twoPressed = false;
    //Button 3 = Decrement value
    } else if (threeState > 1000 && !threePressed){
      //Decrement character value
      startingAllowance[effectedNum]--;
      //Loop round if necessary
      if (startingAllowance[effectedNum] < 0){
        startingAllowance[effectedNum] = 9;
      }
      //Update LCD
      lcd.setCursor(10+effectedNum, 0);
      lcd.print(startingAllowance[effectedNum]);
      threePressed = true;
    } else if (threeState < 200 && threePressed){
      threePressed = false;
    //Button 4 = Move right
    } else if (fourState > 1000 && !fourPressed){
      //Clear the current dash
      lcd.setCursor(10+effectedNum,1);
      lcd.print(" ");
      //Increment dash position
      effectedNum++;
      //Loop back if necessary
      if (effectedNum > 5){
        effectedNum = 0;
      }
      //Draw the new dash position
      lcd.setCursor(10+effectedNum, 1);
      lcd.print("-");
      fourPressed = true;
    } else if (fourState < 200 && fourPressed){
      fourPressed = false;
    }
    delay(30);
  }

  //Convert integer array into a single number
  long allowanceAsNum = 0;
    for (int j = 0; j < 6; j++){
      allowanceAsNum = allowanceAsNum * 10 + startingAllowance[j]; 
    }

  //Save players starting allowances
  for (int i = 0; i < numOfPlayers; i++){
    players[i].balance = allowanceAsNum;
  }

  for (int i = 0; i < numOfPlayers; i++){
    Serial.print(players[i].name);
    Serial.print("\t");
    Serial.println(players[i].balance);
  }
  selectDealer();
}

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
    
    //Button 1 = Confirm
    if (oneState > 1000 && !onePressed){
      onePressed = true;
      break;
    } else if (oneState < 200 && onePressed){
      onePressed = false;
      //Button 2 = Increase
    } else if(twoState > 1000 && !twoPressed){
      //Increment number of players
      numOfPlayers++;
      //Limit max number of players to 9
      if (numOfPlayers > 9){
        numOfPlayers = 9;
      }
      //Update LCD screen
      lcd.setCursor(9,1);
      lcd.print(numOfPlayers);
      twoPressed = true;
    //Reset button press
    } else if(twoState < 200 && twoPressed){
      twoPressed = false;
    //Button 3 = Decrease
    }else if(threeState > 1000 && !threePressed){
      //Decrement number of players
      numOfPlayers--;
      //Limit minimum number of players to 2
      if (numOfPlayers < 2){
        numOfPlayers = 2;
      }
      //Update LCD screen
      lcd.setCursor(9,1);
      lcd.print(numOfPlayers);
      threePressed = true;
    //Reset button press
    } else if (threeState < 200 && threePressed){
      threePressed = false;
    } 
    delay(30);
  }

  //PLAYER NAME ENTRY

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
      if (oneState > 1000 && !onePressed){
        onePressed = true;
        playerNameEntered = true;
      } else if (oneState < 200 && onePressed){
        onePressed = false;
      //Button 2 = Increment value
      } else if (twoState > 1000 && !twoPressed){
        //Increment character value
        name[effectedChar]++;
        //Loop round if necessary
        if (name[effectedChar] > 90){
          name[effectedChar] = 65;
        }
        //Update LCD
        lcd.setCursor(9+effectedChar,0);
        lcd.print((char)name[effectedChar]);
        twoPressed = true;
      } else if (twoState < 200 && twoPressed){
        twoPressed = false;
      //Button 3 = Decrement value
      } else if (threeState > 1000 && !threePressed){
        //Decrement character value
        name[effectedChar]--;
        //Loop round if necessary
        if (name[effectedChar] < 65){
          name[effectedChar] = 90;
        }
        //Update LCD
        lcd.setCursor(9+effectedChar, 0);
        lcd.print((char)name[effectedChar]);
        threePressed = true;
      } else if (threeState < 200 && threePressed){
        threePressed = false;    
      //Button 4 = Move right
      } else if (fourState > 1000 && !fourPressed){
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
        fourPressed = true;
      } else if (fourState < 200 && fourPressed){
        fourPressed = false;
      }
      delay(30);
    }
    //Pass the player "name to the struct"
    for (int j = 0; j < 3; j++){
      players[i].name[j] = (char)name[j];
    }
    //Add null terminator to the end of player name
    players[i].name[3] = '\0';    
  }

  enterStartingAllowance();
}

void setup() {
  analogWrite(contrast, 130);
  //Define the LCD as 16x2 screen
  lcd.begin(16, 2);

  //Store £ at index 0
  lcd.createChar(0, poundSign);

  Serial.begin(9600);

  //Assign all switch pins as inputs;
  pinMode(pinOne, INPUT);
  pinMode(pinTwo, INPUT);
  pinMode(pinThree, INPUT);
  pinMode(pinFour, INPUT);

  lcd.clear();
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
  }

  enterPlayerNums();
}


bool errorMessagePrinted = false;
void loop() {
  if (!errorMessagePrinted){
    lcd.clear();
    lcd.print("Something went");
    lcd.setCursor(0,1);
    lcd.print("wrong :/");
    errorMessagePrinted = true;
  }
}