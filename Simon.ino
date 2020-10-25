int numOfLeds = 4; // number of leds
int numOfButtons = 4; // number of buttons
int ledPins[] = {12, 11, 10, 9}; // pins on arduino we will be using for our leds
int buttonPins[] = {7, 6, 5, 4}; // pins on arduino we will be using for out buttons
bool start = false; // game state flag
bool gameOver = false; // game state flag
int numOfElementsInArr = 0; // useful for array operations, e.g., looping and assigning 
int currentGameState[100] = {}; // array for tracking game state, i.e., order in which leds blink

int buttonStates[] = {0, 0, 0, 0};
int delayTime = 400; // time in ms to turn on and turn off led

void setup() {


  Serial.begin(9600);

  // initializes leds for output 
  
  for (int i = 0; i < numOfLeds; i++){
    pinMode(ledPins[i], OUTPUT);
  }

  // initializes buttons for input

  for (int i = 0; i < numOfButtons; i++){
    pinMode(buttonPins[i], INPUT);
  }
}

// starts a new game 

void newGame(){

  Serial.print("Game Start");
  start = true;
  pickNextColor();

}

/* randomly selects next led to add to our pattern and increments counter that 
 *  stores index of last element
*/

void pickNextColor(){

  currentGameState[numOfElementsInArr] = ledPins[random(numOfLeds)];
  numOfElementsInArr += 1;
  
}

/*
 * utility function that ouputs current game state to console
 */

void displayArr(){
  for (int i = 0; i < numOfElementsInArr; i++){
    Serial.print(currentGameState[i]);
    Serial.print(',');
  }
}

/*
 * loops over game state array (random sequence of our 4 output pins)
 * turning corresponding leds on and off until last element is reached
 * then waits for user input
 */

void playRound(){
  
  for (int i = 0; i < numOfElementsInArr; i++){
    digitalWrite(currentGameState[i], HIGH);
    delay(delayTime);
    digitalWrite(currentGameState[i], LOW);
    delay(delayTime);
  }

  /*
   * waits for player input and checks to see if they get the pattern right
   * if player gets pattern wrong, game ends and game over effect plays
   */

  userInput();
}


void userInput(){
  int lastInput = NULL;
  int counter = 0;
  while (counter < numOfElementsInArr){
    for (int i = 0; i < numOfButtons; i++){
      buttonStates[i] = digitalRead(buttonPins[i]);
    }
    for (int i = 0; i < numOfButtons; i++){
      if (buttonStates[i] == HIGH){
        digitalWrite(ledPins[i], HIGH);
        delay(300);
        lastInput = ledPins[i];
        digitalWrite(ledPins[i], LOW);
        counter += 1;
      } 
    }
       if (lastInput != NULL and currentGameState[counter-1] != lastInput){
      gameOver = true;
      gameOverChime();
      return;
    } 
  }
}

/*
 * blinks all leds 3 times if player loses
 */

void gameOverChime(){
  for (int i = 0; i < 3; i++){
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[2], HIGH);
  digitalWrite(ledPins[3], HIGH);
  delay(500);
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);
  delay(500);
}
}

/*
 * starts a new game
 */

void reset(){
  for (int i = 0; i < numOfButtons; i++){
    buttonStates[i] = digitalRead(buttonPins[i]);
    if (buttonStates[i] == HIGH and gameOver == true){
      start = false;
      gameOver = false;
      numOfElementsInArr = 0;
    }
  }
  
}


void loop() {

  

  while (start == false){
    newGame();
  }

  while (gameOver == false){
    delay(500);
    playRound();
    pickNextColor();
  }

  reset();
  
}
