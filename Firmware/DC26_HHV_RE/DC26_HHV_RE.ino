#include <SoftwareSerial.h>
#include <TinyWireM.h>

#define RX  2
#define TX  3
SoftwareSerial Serial(RX, TX);

#define PASSLENGTH  8 // length of passcode

#define EEPROM_ADDR 0x50 // 7 bit I2C address for EEPROM

bool keyPressed = false;            // debouncing 
uint16_t lastKeyState = 1023;       // the previous reading from the input pin
uint8_t keysEntered = 0;            // keep track on passcode keys entered
uint8_t userPass[PASSLENGTH] = {0}; // passcode entered by user
bool wrongPass = false;             // flag
uint8_t password[PASSLENGTH] = {1, 4, 4, 3, 1, 2, 1, 2}; // the correct passcode

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // initialize serial
  TinyWireM.begin();  // initialize I2C
  delay(100);
  Serial.println("welcome to the DC 26 HHV Challenge!");

  // set IO directions
  pinMode(0, INPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);

  // set IO initial states
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(10, LOW);

  // check if eeprom corrected
  if (checkEEPROM()) {
    digitalWrite(5, HIGH);
    Serial.println("Challenge 2: complete");
    Serial.println("flag: 0h_1_r3m3mb3r_n0w!");
  }

  // prompt for challenge 1
  Serial.println("Challenge 1:");
  Serial.println("Enter the correct passcode");
}

void loop() {
  // put your main code here, to run repeatedly:
  // watch for button input (aka A0 != 1023)
  uint16_t key = analogRead(0);
  
  if (key < 1000) {
    // poor man's debounce
    lastKeyState = key;
    delay(50);
    if ((key == lastKeyState) & (!keyPressed)) {
      // new button pressed
      keyPressed = true;
      uint8_t value = 0;
      if ((key > 740) & (key < 800)) {
        // A pressed; value should be ~769
        value = 1;
        Serial.println("A");
      } else if ((key > 480) & (key < 550)) {
        // B pressed; value should be ~512
        value = 2;
        Serial.println("B");
      } else if ((key >= 0) & (key < 50)) {
        // C pressed; value should be ~0
        value = 3;
        Serial.println("C");
      } else if ((key > 650) & (key < 730)) {
        // D pressed; value should be ~684
        value = 4;
        Serial.println("D");
      }

      // store and check passcode
      userPass[keysEntered] = value;
      checkPassword(value, keysEntered);
      keysEntered++;

      // reset once more than PASSLENGTH keys entered
      if (keysEntered >= PASSLENGTH) {
        keysEntered = 0;
        wrongPass = false;
        for (int i=0; i < 4; i++) {
          userPass[i] = 0;
        }
      }
    }
  } else {
    // key released
    keyPressed = false;
  }
  delay(100);
}

// checks latest key entered
void checkPassword(uint8_t key, uint8_t count) {
  if (!wrongPass) {
    // everything has been correct so far
    if (key == password[count]) {
      // small delay to make it a little easier to pick
      // up on correct vs incorrect key presses
      delay(20);
    } else {
      wrongPass = true;
    }
  }

  // LED feedback to show button was pressed
  digitalWrite(10, HIGH);
  delay(20);
  digitalWrite(10, LOW);

  if (count >= (PASSLENGTH - 1)) {
    // all keys entered, check if passcode is still correct
    if (wrongPass) {
      digitalWrite(10, HIGH);
      delay(1000);
      digitalWrite(10, LOW);
    } else {
      // winner winner!
      digitalWrite(7, HIGH);
      Serial.println("Challenge 1: complete");
      Serial.println("flag: h3y_wh4ts_t3h_p4ssw0rd?");
    }
  }
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  TinyWireM.beginTransmission(deviceaddress);
  TinyWireM.send((int)(eeaddress >> 8));   // MSB
  TinyWireM.send((int)(eeaddress & 0xFF)); // LSB
  TinyWireM.endTransmission();
 
  TinyWireM.requestFrom(deviceaddress,1);
 
  if (TinyWireM.available()) rdata = TinyWireM.receive();
 
  return rdata;
}

// read a specific eeprom address and check if it was
// changed to the correct value
uint8_t checkEEPROM() {
  byte readData = readEEPROM(EEPROM_ADDR, 0x5A);

  if (readData == 0x23) {
    return 1;
  }
  
  return 0;
}

