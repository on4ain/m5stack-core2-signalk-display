#include <Arduino.h>
#include <M5Unified.h>
#include "screenBase.cpp"
#ifndef WINDSCREEN_
#define WINDSCREEN_

class WindScreen: public ScreenBase {
    private:
        int32_t centerx, centery;
        int32_t oldendx, oldendy;
        float winddir;
        float windforce;
        int innerradius = 90;
        int outerradius = 95; 

        void erase_oldWindArrow() {
            M5.Lcd.drawWideLine(centerx, centery, oldendx, oldendy, 6, BLACK);
        }
        void display_windarc() { 
            M5.Lcd.drawArc(centerx, centery, innerradius, outerradius, 320, 220, GREEN);
            M5.Lcd.drawArc(centerx, centery, innerradius, outerradius, 220, 320, RED);
        }
        void display_WindArrow() {
            M5.Lcd.setTextSize(2);
            int32_t endx = cos(winddir) * 70 + centerx;
            int32_t endy = sin(winddir) * 70 + centery;
            erase_oldWindArrow();
            M5.Lcd.drawWideLine(centerx, centery, endx, endy, 5, WHITE);
            oldendx = endx;
            oldendy = endy;
        }
        void display_WindSpeed() {
            M5.Lcd.setTextColor(YELLOW, BLACK);
            M5.Lcd.setCursor(0, 195);
            M5.Lcd.print("Knt");
            M5.Lcd.setCursor(60,195);
            M5.Lcd.printf("%.f", windforce);
        }
    public:
        WindScreen(){
            centerx = M5.Lcd.width()/2;
            centery = M5.Lcd.height()/2;
            set_title("Wind");
        }
        void init() {
            centerx = M5.Lcd.width()/2;
            centery = M5.Lcd.height()/2;
        }
        void setData(float winddirIn, float windforceIn) {
            winddir = winddirIn - 1.5708;
            windforce = windforceIn * 1.94384;
        }
        void setWinddir(float winddirIn) {
            winddir = winddirIn - 1.5708;
        }
        void setWindforce(float windforceIn) {
            windforce = windforceIn  * 1.94384;
        }
        void show(bool newch) {
            if (newch) {
                M5.Lcd.clearDisplay();
                display_title();
                display_bottom();
                display_windarc();
            }
            display_WindArrow();
            display_WindSpeed();
        }
        
};
#endif