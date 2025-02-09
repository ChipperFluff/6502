#include "ClockControl.h"

ClockControl::ClockControl(int clockPin, int signalPin, int readWritePin, int stepDelay)
    : _clockPin(clockPin), _signalPin(signalPin), _readWritePin(readWritePin), _stepDelay(stepDelay), _autoStep(false) {}

void ClockControl::setup() {
    pinMode(_clockPin, OUTPUT);
    pinMode(_signalPin, OUTPUT);
    pinMode(_readWritePin, INPUT);
    digitalWrite(_clockPin, LOW);
    digitalWrite(_signalPin, LOW);
    Serial.begin(57600);
}

bool ClockControl::isStep() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command.equals("start")) {
            _autoStep = true;
            return false;
        }
        if (command.equals("stop")) {
            _autoStep = false;
            return false;
        }
        if (command.equals("step")) {
            return true;
        }
    }

    // Auto step mode enabled
    if (_autoStep) {
        delay(_stepDelay);
        return true;
    }
    return false;
}

void ClockControl::pulse() {
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_signalPin, HIGH);
    delay(100);
    digitalWrite(_clockPin, LOW);
    digitalWrite(_signalPin, LOW);
    delay(50);
}
