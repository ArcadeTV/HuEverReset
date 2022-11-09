/* 
ATtiny84 ----------------
                          _____
            VCC  (  ) 01-|°    |-14 (  ) GND
Jmp: Use Speaker (10) 02-|     |-13 (00) ENABLE (MiniDin 7)
     Speaker (+) (09) 03-|     |-12 (01) D3: Run, Left (MiniDin 5)
            N.C. (11) 04-|     |-11 (02) D2: Select, Down (MiniDin 4)
             LED (08) 05-|     |-10 (03) D1: II, Right (MiniDin 3)
            N.C. (07) 06-|     |-09 (04) SELECT (MiniDin 6)
          /RESET (06) 07-|_____|-08 (05) D0: I, Up (MiniDin 2)

| Fuses: 
| -----------
| low:   0xE2
| high:  0xD7
| ext.:  0xFF
| lock:  0xFF

----------------------------------------------------------------------------------------------------------

Description:
Source Code for the ATtiny84 avr of the project "HuEverReset".
https://github.com/ArcadeTV/HuEverReset/
The avr is listening for joypad inputs.
When the button-combo I+II+RUN+SELECT is held for 2 seconds,
an active-low reset signal is sent from pin 7.

*/

// S-E-T-U-P: --------------------------------------------------------------------------------------------
// Change these settings to your needs

bool testOnArduino = false; // For development purposes set this to true.
                            // Useful for testing outside a real PC Engine console.
                            // Pins 9 (SELECT) and 13 (ENABLE) will become OUTPUTS and will always be LOW!
bool enableSerial = false;

// P-I-N-S: ----------------------------------------------------------------------------------------------
const byte CTRL2 = 5;  // D0: I, Up
const byte CTRL3 = 3;  // D1: II, Right
const byte CTRL4 = 2;  // D2: Select, Down
const byte CTRL5 = 1;  // D3: Run, Left
const byte CTRL6 = 4;  // SELECT
const byte CTRL7 = 0;  // ENABLE

const byte RESET = 6;  // RESET OUT (active low)

const byte useBuzzer = 10;
const int buzzer = 9;

const int LED = 8;
// -------------------------------------------------------------------------------------------------------

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;  // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;

byte prevKeyState = HIGH;  // combo is active low

bool resetInProgress = false;
bool comboActive = false;
int buzzing = HIGH;

void setup() {
  if (enableSerial) {
    Serial.begin(115200);
    Serial.flush();
    Serial.println("Initialized!\n");
  }

  pinMode(useBuzzer, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

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
  if (enableSerial) {
    Serial.println(" -> Combo too short!");
  }
}


// called when button is kept pressed for more than 2 seconds
void longKeyPress() {
  if (enableSerial) {
    Serial.println(" -> RESET has been triggered!");
  }
}


// called when key goes from not pressed to pressed
void keyPress() {
  if (enableSerial) {
    Serial.flush();
    Serial.print("Combo detected!");
  }
  longKeyPressCount = 0;
  comboActive = true;
}


// called when key goes from pressed to not pressed
void keyRelease() {
  if (enableSerial) {
    Serial.flush();
    Serial.print("\nCombo released.");
  }

  if (longKeyPressCount >= longKeyPressCountMax) {
    longKeyPress();
  } else {
    shortKeyPress();
  }

  if (!resetInProgress) {
    digitalWrite(LED, HIGH);
  }

  comboActive = false;
}

void performReset() {
  if (enableSerial) {
    Serial.println("\nRESETTING!");
  }

  buzzing = digitalRead(useBuzzer);

  if (buzzing == LOW) {
    // alert upcoming reset:
    digitalWrite(LED, LOW);
    tone(buzzer, 2220);
    delay(100);

    digitalWrite(LED, HIGH);
    tone(buzzer, 1400);
    delay(100);

    digitalWrite(LED, LOW);
    tone(buzzer, 1675);
    delay(100);

    digitalWrite(LED, HIGH);
    tone(buzzer, 2220);
    delay(100);

    noTone(buzzer);
  }

  digitalWrite(LED, LOW);

  // send /RESET signal:
  pinMode(RESET, OUTPUT);
  delay(2000);
  pinMode(RESET, INPUT);

  digitalWrite(LED, HIGH);
}

void loop() {
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

      // LED flashing while holding button combo:
      if (longKeyPressCount <= 10) {
        digitalWrite(LED, LOW);
      } else if (longKeyPressCount > 10 && longKeyPressCount <= 20) {
        digitalWrite(LED, HIGH);
      } else if (longKeyPressCount > 20 && longKeyPressCount <= 30) {
        digitalWrite(LED, LOW);
      } else if (longKeyPressCount > 30 && longKeyPressCount <= 40) {
        digitalWrite(LED, HIGH);
      } else if (longKeyPressCount > 40 && longKeyPressCount <= 50) {
        digitalWrite(LED, LOW);
      } else if (longKeyPressCount > 50 && longKeyPressCount <= 60) {
        digitalWrite(LED, HIGH);
      } else if (longKeyPressCount > 60 && longKeyPressCount <= 70) {
        digitalWrite(LED, LOW);
      } else if (longKeyPressCount > 70 && longKeyPressCount <= 80) {
        digitalWrite(LED, HIGH);
      }

      if (longKeyPressCount >= longKeyPressCountMax && !resetInProgress) {

        resetInProgress = true;
        performReset();
      }
    }
    prevKeyState = currKeyState;
  }
}
