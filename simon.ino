const int buttons[] = {13, 12, 11, 10}; //pin numbers for buttons
const int lights[] = {9, 8, 7, 6}; //pin numbers for lights
const int rounds = 20; //the max number of rounds is 20
int sequence[rounds]; //holds the sequence
int userSequence[rounds]; //holds user sequence
int currentRound;
int currentIteration;

//phases of game
int preGame = 0;
int sequencing = 1;
int check = 2;
int compare = 3;
int game;

//implemented debouncing of buttons
//https://www.arduino.cc/en/Tutorial/Debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 500;
int lastButtonState = HIGH;

/*
   sets buttons as inputs
   sets lights as outputs
   initializes the phase of the game (preGame), round, and currentIteration
   Serials were used to debug through the program
*/
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(2));
  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT);
    pinMode(lights[i], OUTPUT);
  }

  currentRound = 1;
  currentIteration = 0;
  pinMode(5, INPUT);
  game = preGame;

}

/*
   loops through the different phases in the game
*/
void loop() {
  if (game == preGame) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        game = sequencing;
      }
    }
  } else if (game == sequencing) {
    generateSequence();
    for (int i = 0; i < currentRound; i++) {
      //Serial.println(currentRound);
      //Serial.print(sequence[i]);
    }
    showSequence();
    Serial.println("showing sequence");
  } else if (game == check) {
    registerClicks();
  } else if (game == compare) {
    currentIteration = 0;
    if (matches()) {
      Serial.println("correct");
      correct();
    } else {
      Serial.println("incorrect");
      endGame();
    }
  }
}

/*
 * method implementing debouncing
 * registers clicks from user and adds to userSequence
 */
void registerClicks() {
  while (currentIteration < currentRound) {
    for (int i = 0; i < 4; i++) {
      if ((millis() - lastDebounceTime) > debounceDelay) {
        Serial.println("*");
        if (digitalRead(buttons[i]) == HIGH) {
          Serial.println("DINGERDONGER");
          userSequence[currentIteration] = i;
          digitalWrite(lights[i], HIGH);
          delay(100);
          digitalWrite(lights[i], LOW);
          lastDebounceTime = millis();
          currentIteration++;
        }
      }
    }
    if (currentIteration >= currentRound) {
      game = compare;
    }
  }
}

/*
 * generates a random sequence and adds to sequence array
 */
void generateSequence() {
  for (int i = currentRound - 1; i < currentRound; i++) {
    sequence[i] = random(4);
  }
}

/*
 * displays the sequence to player using lights
 */
void showSequence() {
  for (int i = 0; i < currentRound; i++) {
    digitalWrite(lights[sequence[i]], HIGH);
    delay(1000);
    digitalWrite(lights[sequence[i]], LOW);
    delay(1000);
  }
  game = check;
}

/*
 * param - empty
 * return boolean to check if the userSequence matches with the randomly generated sequence
 */
boolean matches() {
  Serial.println("checking match");
  boolean doesItMatch = true;
  for (int i = 0; i < currentRound; i++) {
    if (sequence[i] != userSequence[i]) {
      doesItMatch = false;
    }
  }
  memset(userSequence, 0, rounds);
  return doesItMatch;
}

/*
 * flashes green lights for each correct answer
 * increments the currentRound and sets the game phase to sequencing
 */
void correct() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
    delay(700);
  }
  currentRound++;
  game = sequencing;
  delay(1000);
}

/*
 * end game/game over 
 * flashes red lights for incorrect answer
 * returns to preGame phase
 */
void endGame() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(7, HIGH);
    delay(500);
    digitalWrite(7, LOW);
    delay(700);
  }
  game = preGame;
  currentRound = 1;
  Serial.println(game);
}


