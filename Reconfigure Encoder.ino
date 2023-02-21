#include "Arduino.h"
#include "NewEncoder.h"

// Pins 2 and 3 should work for many processors, including Uno. See README for meaning of constructor arguments.
// Use FULL_PULSE for encoders that produce one complete quadrature pulse per detnet, such as: https://www.adafruit.com/product/377
// Use HALF_PULSE for endoders that produce one complete quadrature pulse for every two detents, such as: https://www.mouser.com/ProductDetail/alps/ec11e15244g1/?qs=YMSFtX0bdJDiV4LBO61anw==&countrycode=US&currencycode=USD
NewEncoder encoder(5, 18, 1, 8, 1, FULL_PULSE);
int16_t prevEncoderValue;
int button = 19;
int selected = false;
int16_t prevSpinValue = 0;

void Button() {
  selected != selected;
  Serial.println(selected);
}

void setup() {
  NewEncoder::EncoderState state;

  Serial.begin(115200);
  delay(100);
  Serial.println("Starting");

  if (!encoder.begin()) {
    encoder.getState(state);
    Serial.print("Encoder Successfully Started at value = ");
    prevEncoderValue = state.currentValue;
    Serial.println(prevEncoderValue);
  }
  attachInterrupt(digitalPinToInterrupt(button), Button, FALLING);
}

void loop() {
  int16_t currentValue;
  int16_t spinValue;
  if (selected == true) {
    NewEncoder::EncoderState encoderState;
    encoder.newSettings(-20, 20, 0, NewEncoder::EncoderState::currentEncoderState)
    if (encoder.getState(currentEncoderState)) {
      Serial.print("Encoder: ");
      spinValue = currentEncoderState.currentValue;
      if (spinValue != prevSpinValue) {
        Serial.println(spinValue);
        prevSpinValue = spinValue;
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
    if (selected == false) {
      encoder newSettings(1, 8, prevEncoderValue, EncoderState currentEncoderState);
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