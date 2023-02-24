#include "Arduino.h"
#include "NewEncoder.h"

// Define a struct to store encoder settings (upper and lower limits and starting position)
struct EncoderSettings {
  int16_t low;    // lower limit of encoder values
  int16_t high;   // upper limit of encoder values
  int16_t start;  // starting position of encoder
};

// Define an index to keep track of current encoder setting
uint16_t settingIndex = 0;

// Define an array of encoder settings
const EncoderSettings settings[] = {
  { -20, 20, 0 },     // Setting 0: lower limit -20, upper limit 20, starting position 0
  { 40, 50, 45 },     // Setting 1: lower limit 40, upper limit 50, starting position 45
  { 100, 150, 100 },  // Setting 2: lower limit 100, upper limit 150, starting position 100
  { 200, 250, 210 }   // Setting 3: lower limit 200, upper limit 250, starting position 210
};

//Create a NewEncoder object with pin assignments and initial settings from the settings array
NewEncoder encoder(5, 18, settings[settingIndex].low, settings[settingIndex].high, settings[settingIndex].start, FULL_PULSE);

int16_t prevEncoderValue;  //Initialize a variable to store the previous encoder value

//Define the button pin and initialize time variables
const uint8_t button = 19;
int TimeOfLastButtonReading = 0;
int TimeSinceLastButtonReading;

//Set the debounce threshold for the button
int debounceThreshold = 20;

//Initialize variables for button reading and state
int buttonReading;
bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  NewEncoder::EncoderState state;  //Create a NewEncoder::EncoderState object to hold the state of the encoder

  Serial.begin(115200);  //Start serial communication

  delay(2000);  //Delay for 2 seconds

  Serial.println("Starting");

  //If the encoder fails to start, print an error message to the serial monitor and enter an infinite loop
  if (!encoder.begin()) {
    Serial.println("Encoder Failed to Start. Check pin assignments and available interrupts. Aborting.");
    while (1) {
      yield();
    }
  }
  //Otherwise, get the current state of the encoder and print the starting value to the serial monitor
  else {
    encoder.getState(state);
    Serial.print("Encoder Successfully Started at value = ");
    prevEncoderValue = state.currentValue;
    Serial.println(prevEncoderValue);
  }

  pinMode(button, INPUT_PULLUP);  //Set the button pin as an input with internal pull-up resistor
}

void loop() {

  //Declare variables for current encoder value and encoder state
  int16_t currentValue;
  NewEncoder::EncoderState currentEncoderState;

  //Calculate the time since the last button reading
  TimeSinceLastButtonReading = (millis() - TimeOfLastButtonReading);  //If enough time has passed for debounce, read the button state
  if (TimeSinceLastButtonReading > debounceThreshold) {
    buttonReading = digitalRead(button);
    if (buttonReading != buttonState) {  //If the button state has changed, update the button state variable
      buttonState = buttonReading;
      if (buttonState == LOW) {                 //If the button is pressed, toggle through the different encoder settings
        encoder.getState(currentEncoderState);  //Get the current state of the encoder and set the setting index based on the current value
        currentValue = currentEncoderState.currentValue;
        if (currentValue == 1) {
          settingIndex = 1;
        } else if (currentValue == 2) {
          settingIndex = 2;
        } else if (currentValue == 3) {
          settingIndex = 3;
        } else {
          settingIndex = 0;
        }
        encoder.newSettings(settings[settingIndex].low, settings[settingIndex].high, settings[settingIndex].start, currentEncoderState);
        Serial.print("Encoder Settings Toggled to Index ");
        Serial.println(settingIndex);
      }
      TimeOfLastButtonReading = millis();
    }
  }

  if (encoder.getState(currentEncoderState)) {
    Serial.print("Encoder: ");
    currentValue = currentEncoderState.currentValue;
    if (currentValue != prevEncoderValue) {
      Serial.println(currentValue);
      prevEncoderValue = currentValue;
    } else
      switch (currentEncoderState.currentClick) {
        case NewEncoder::UpClick:
          Serial.println("at upper limit.");
          break;

        case NewEncoder::DownClick:
          Serial.println("at lower limit.");
          break;

        default:
          break;
      }
  }
}
