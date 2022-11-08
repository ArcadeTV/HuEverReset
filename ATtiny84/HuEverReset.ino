/* 
ATtiny84 ----------------
                _____
       VCC  01-|°    |-14 GND
            02-|     |-13 ENABLE (MiniDin 7)
            03-|     |-12 D3: Run, Left (MiniDin 5)
            04-|     |-11 D2: Select, Down (MiniDin 4)
            05-|     |-10 D1: II, Right (MiniDin 3)
            06-|     |-09 SELECT (MiniDin 6)
     /RESET 07-|_____|-08 D0: I, Up (MiniDin 2)

low:   0xE2
high:  0xD7
ext.:  0xFF
lock:  0xFF

-------------------------

Description:
Source Code for the ATtiny84 avr of the project "HuEverReset".
https://github.com/ArcadeTV/HuEverReset/
The avr is listening for joypad inputs.
When the button-combo I+II+RUN+SELECT is held for 2 seconds,
an active-low reset signal is sent from pin 7.

*/
bool testOnArduino = true;  // For development purposes set this to true.
                            // Useful for testing outside a real PC Engine console.
                            // Pins 9 (SELECT) and 13 (ENABLE) will become OUTPUTS and will always be LOW!

const byte CTRL2 = 8;       // D0: I, Up
const byte CTRL3 = 10;      // D1: II, Right
const byte CTRL4 = 11;      // D2: Select, Down
const byte CTRL5 = 12;      // D3: Run, Left
const byte CTRL6 = 9;       // SELECT
const byte CTRL7 = 13;      // ENABLE

const byte RESET = 7;  // RESET OUT (active low)

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;  // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;

byte prevKeyState = HIGH;  // combo is active low

bool resetInProgress = false;
bool comboActive = false;

void setup() {
  Serial.begin(115200);
  Serial.flush();
  Serial.println("Initialized!\n");

  pinMode(RESET, INPUT);  // init RESET pin as input
  pinMode(CTRL2, INPUT);
  pinMode(CTRL3, INPUT);
  pinMode(CTRL4, INPUT);
  pinMode(CTRL5, INPUT);

  if (testOnArduino) {
    pinMode(CTRL6, OUTPUT);
    pinMode(CTRL7, OUTPUT);
  } else {
    pinMode(CTRL6, INPUT);
    pinMode(CTRL7, INPUT);
  }

  digitalWrite(CTRL6, LOW);
  digitalWrite(CTRL7, LOW);
}


byte checkButtonCombo() {

  byte result;

  if (testOnArduino) {
    byte button_I = digitalRead(CTRL2);
    byte button_II = digitalRead(CTRL3);
    byte button_SELECT = digitalRead(CTRL4);
    byte button_RUN = digitalRead(CTRL5);
    result = (button_I | button_II | button_SELECT | button_RUN);
  } else {
    byte button_I = digitalRead(CTRL2);
    byte button_II = digitalRead(CTRL3);
    byte button_SELECT = digitalRead(CTRL4);
    byte button_RUN = digitalRead(CTRL5);
    byte line_SELECT = digitalRead(CTRL6);
    byte line_ENABLE = digitalRead(CTRL7);
    result = (button_I | button_II | button_SELECT | button_RUN | line_SELECT | line_ENABLE);
  }
  return result;
}


// called when button is kept pressed for less than 2 seconds
void shortKeyPress() {
  Serial.println(" -> Combo too short!");
}


// called when button is kept pressed for more than 2 seconds
void longKeyPress() {
  Serial.println(" -> RESET has been triggered!");
}


// called when key goes from not pressed to pressed
void keyPress() {
  Serial.flush();
  Serial.print("Combo detected!");
  longKeyPressCount = 0;
  comboActive = true;
}


// called when key goes from pressed to not pressed
void keyRelease() {
  Serial.flush();
  Serial.print("\nCombo released.");

  if (longKeyPressCount >= longKeyPressCountMax) {
    longKeyPress();
  } else {
    shortKeyPress();
  }
  comboActive = false;
}

void performReset() {
  Serial.println("\nRESETTING!");
  pinMode(RESET, OUTPUT);
  delay(2000);
  pinMode(RESET, INPUT);
}

void loop() {
  // key management section
  if (millis() - keyPrevMillis >= keySampleIntervalMs) {
    keyPrevMillis = millis();
    byte currKeyState = checkButtonCombo();
    if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
      keyPress();
    } else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
      keyRelease();
      resetInProgress = false;
    } else if (currKeyState == LOW) {
      longKeyPressCount++;
      if (longKeyPressCount >= longKeyPressCountMax && !resetInProgress) {
        resetInProgress = true;
        performReset();
      }
    }
    prevKeyState = currKeyState;
  }
}
