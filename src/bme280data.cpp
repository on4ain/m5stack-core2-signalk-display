#include <Arduino.h>
#include <M5Unified.h>
#include "screenBase.cpp"
#ifndef ENVIRONMENT_
#define ENVIRONMENT_

class bme280data: public ScreenBase {
    private:
        float pressure;
        float temp;
        void display_bme280data() {
            M5.Lcd.setTextColor(YELLOW, BLACK);
            M5.Lcd.setCursor(10, 50);
            M5.Lcd.printf("HPa: %.3f", pressure);
            M5.Lcd.setCursor(10, 70);
            M5.Lcd.printf("C: %.1f",temp);
        }
    public:
        bme280data(){
            set_title("environment");
        }
        void init() {
            
         }
         void setData(float pres, float tmp) {
            pressure = pres / 100;
            temp = tmp - 273.15;       
        }
        void setPress(float pres) {
            pressure = pres / 100;
        }
        void setTmp(float tmp) {
            temp = tmp - 273.15; 
        }
        void show(bool newch) {
            if (newch) {
                M5.Lcd.clearDisplay();
                display_title();
                display_bottom();
            }
            display_bme280data();
        }

};
#endif