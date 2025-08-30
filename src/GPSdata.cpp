#include <Arduino.h>
#include <ArduinoJson.h>
#include <M5Unified.h>
#include "screenBase.cpp"
#ifndef GPS_
#define GPS_

class GPSdata: public ScreenBase {
    private:
        String gps_pos;
        char lat_char, lon_char;
        int lon_deg, lat_deg;
        float lon_decimal, lat_decimal;
        void display_GPSdata() {
            M5.Lcd.setTextColor(YELLOW, BLACK);
            M5.Lcd.setCursor(0, 70);
            M5.Lcd.print("lat : ");
            M5.Lcd.setCursor(130,70);
            M5.Lcd.printf("%c %u %.2f'", lat_char, lat_deg, lat_decimal);
            M5.Lcd.setCursor(0, 90);
            M5.Lcd.print("long: ");
            M5.Lcd.setCursor(130,90);
            M5.Lcd.printf("%c %u %.2f'", lon_char, lon_deg, lon_decimal);
        }
    public:
        GPSdata() {
            set_title("position");

        }
        void init() {

        }
        void setData(String GPSin) {
            JsonDocument json_pos;
            deserializeJson(json_pos, GPSin);
            float latitude = json_pos["latitude"];
            float longitude = json_pos["longitude"];
            lat_char = (latitude > 0.0) ? 'N' : 'S';
            lat_deg = int(latitude);
            lat_decimal = fmod(latitude,1.0)*60;
            lon_char = (longitude > 0.0) ? 'E' : 'W';
            lon_deg = int(longitude);
            lon_decimal = fmod(longitude,1.0)*60;
        }
        void show(bool newch) {
            if (newch) {
                M5.Lcd.clearDisplay();
                display_title();
                display_bottom();
            }
            display_GPSdata();
        }
};
#endif