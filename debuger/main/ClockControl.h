#ifndef CLOCKCONTROL_H
#define CLOCKCONTROL_H

#include <Arduino.h>

class ClockControl {
public:
    ClockControl(int clockPin, int signalPin, int readWritePin, int stepDelay);
    void setup();
    bool isStep();
    void pulse();

private:
    int _clockPin;
    int _signalPin;
    int _readWritePin;
    int _stepDelay;
    bool _autoStep;
};

#endif // CLOCKCONTROL_H
