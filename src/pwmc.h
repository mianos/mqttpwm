#pragma once

// RC_FAST_CLK
//  Test on Lolin S2 Mini ESP32-S2FN4R2 on gpio15 (internal led) https://www.wemos.cc/en/latest/s2/s2_mini.html
//  bits freq min freq max (Hz)
//  1    19532    ?
//  2    9766     10019569
//  3    4883     5009784
//  4    2442     2504892
//  5    1221     1252446
//  6    611      626223
//  7    306      313111
//  8    153      156555
//  9    77       78277
//  10   39       39138
//  11   20       19569
//  12   10       9784
//  13   5        4892
//  14   3        2446

class PWMControl {
public:
    PWMControl(int pin=D0, int frequency=90, bool invert=true) : pin(pin), frequency(frequency), invert(invert) {
        ledcSetup(0, frequency, 10); 
        ledcAttachPin(pin, 0);     // Attach the channel to the specified pin
        ledcWrite(0, dutyCycle);
    }
 //    PWMControl() = delete;

    void setDutyCycle(float newDutyCycle) {
        if (newDutyCycle < 0.0) {
            newDutyCycle = 0.0;
        } else if (newDutyCycle > 100.0) {
            newDutyCycle = 100.0;
        }

        if (invert) {
          newDutyCycle = 100.0 - newDutyCycle;
        }
        int dutyValue = int((newDutyCycle / 100.0) * 1024);
//        printf("Write cycle value of %d\n", dutyValue);
        ledcWrite(0, dutyValue);
        dutyCycle = newDutyCycle;
    }

private:
    int pin=D0;
    int frequency=90;
public:
    float dutyCycle = 50.0;
    bool invert=true;
};
