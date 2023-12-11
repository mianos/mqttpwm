#pragma once

class PWMControl {
public:
    PWMControl(int pin=16, int frequency=90) {
        this->pin = pin;
        this->frequency = frequency;
        ledcSetup(0, frequency, 10); 
        ledcAttachPin(pin, 0);     // Attach the channel to the specified pin
    }
 //    PWMControl() = delete;

    void setDutyCycle(float newDutyCycle) {
        if (newDutyCycle < 0.0) {
            newDutyCycle = 0.0;
        } else if (newDutyCycle > 100.0) {
            newDutyCycle = 100.0;
        }

        int dutyValue = int((newDutyCycle / 100.0) * 1024);
        ledcWrite(0, dutyValue);
        dutyCycle = newDutyCycle;
    }

private:
    int pin;
    int frequency;
public:
    float dutyCycle = 0.0;
};
