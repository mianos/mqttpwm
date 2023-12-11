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

    void setDutyCycle(float dutyCycle) {
        if (dutyCycle < 0.0) {
            dutyCycle = 0.0;
        } else if (dutyCycle > 100.0) {
            dutyCycle = 100.0;
        }

        int dutyValue = int((dutyCycle / 100.0) * 1024);
        ledcWrite(0, dutyValue);
    }

private:
    int pin;
    int frequency;
};
