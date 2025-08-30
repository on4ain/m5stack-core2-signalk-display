#include <Arduino.h>
#include <M5Unified.h>
#ifndef SCREENBASE_
#define SCREENBASE_
class ScreenBase {
    private:
        String title = "Aubine";
    public:
        ScreenBase() {
            M5.Lcd.clearDisplay();
            display_title();
            display_bottom();
        }
        void set_title(String titleIn) {
            title = titleIn;
        }
        void display_title(){
            M5.Lcd.setTextSize(2);
            M5.Lcd.setTextColor(GREEN, BLACK);
            M5.Lcd.setCursor(50, 1);
            M5.Lcd.println(title);
        }
        void display_bottom(){
            M5.Lcd.setTextSize(2);
            M5.Lcd.setTextColor(GREEN, BLACK);
            M5.Lcd.setCursor(40,215);
            M5.Lcd.print("<");
            M5.Lcd.setCursor(255,215);
            M5.Lcd.print(">");
        }

};
#endif