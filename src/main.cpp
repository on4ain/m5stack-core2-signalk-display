#include <Arduino.h>
#include <M5Unified.h>
#include <ArduinoJson.h>
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_listener.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/signalk/signalk_value_listener.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp/system/system_status_led.h"
#include "sensesp_app_builder.h"

#include "screenBase.cpp"
#include "WindScreen.cpp"
#include "bme280data.cpp"
#include "GPSdata.cpp"

using namespace sensesp;

// vars for internal use
const int listen_delay = 600;
int choice = 0;
bool newchoice = true;
int currentscreen = 1;
void DisplayScreen(int scr);

// SignalK input variables and routines
const char* sk_pressure = "environment.outside.pressure";
float bme280_press;   // Pascal
const char* sk_temp = "environment.outside.temperature";
float bme280_temp;    // Kelvin
const char* sk_position = "navigation.position";
String gps_pos;       // json sting {"longitude": <float>, "latitude": <float>}
const char* sk_winddir = "environment.wind.directionTrue";
float wind_dir = 0.872665;       // radians, ex: 0.24478806506151
const char* sk_windspeed = "environment.wind.speedTrue";
float wind_speed = 3.59;     // m/s, ex: 1.697667096742331

// vars and objects for output screens
int totalscreens = 3;
WindScreen windscr;
bme280data envdata;
GPSdata GPSposition;

void setup() {
  SetupLogging(ESP_LOG_DEBUG);
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.setTextSize(2);
  windscr.init();

  SensESPAppBuilder builder;
  sensesp_app = (&builder)
    // Set a custom hostname for the app.
    ->set_hostname("aubine-Core2")
    ->set_wifi_client("avnav1", "avnav-secret")
    ->set_sk_server("192.168.30.10", 3000)              
    ->get_app();
  //*****************************************************
  // environment.pressure
  auto* pressure = new FloatSKListener(sk_pressure, listen_delay);
  pressure->connect_to(new LambdaConsumer<float>([](float press) {
    bme280_press = press;
  }));
  // environment.temperature
  auto* temperature = new FloatSKListener(sk_temp, listen_delay);
  temperature->connect_to(new LambdaConsumer<float>([](float temp) {
    bme280_temp = temp;
  }));
  // navigation.position
  auto* position = new StringSKListener(sk_position, listen_delay);
  position->connect_to(new LambdaConsumer<String>([](String pos) {
    gps_pos = pos;
  }));
  // wind.direction
  auto* winddir = new FloatSKListener(sk_winddir, listen_delay);
  winddir->connect_to(new LambdaConsumer<float>([](float wdir) {
    wind_dir = wdir;
  }));
  // wind.speed
  auto* windspeed = new FloatSKListener(sk_windspeed, listen_delay);
  windspeed->connect_to(new LambdaConsumer<float>([](float wspeed) {
    wind_speed = wspeed;
  }));
  //*****************************************************
  sensesp::event_loop()->onRepeat(1000, [](){
    if (newchoice) {
      currentscreen += choice;
      if (currentscreen > totalscreens) {
        currentscreen = 1;
      }
      if (currentscreen < 1) {
        currentscreen = totalscreens;
      }
    }
    Serial.println("currentscreen: " + String(currentscreen));
    DisplayScreen(currentscreen);
  });
}
//*****************************************************
void loop() {
  event_loop()->tick();
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    choice = -1;
    newchoice = true;
  }
  if (M5.BtnC.wasPressed()) {
    choice = 1;
    newchoice = true;
  }

}
void DisplayScreen(int scr) {
  if (scr == 1) {
    windscr.setData(wind_dir, wind_speed);
    windscr.show(newchoice);
    newchoice = false;
  }
  if (scr == 2) {
    envdata.setData(bme280_press, bme280_temp);
    envdata.show(newchoice);
    newchoice = false;
  }
  if (scr == 3) {
    GPSposition.setData(gps_pos);
    GPSposition.show(newchoice);
    newchoice = false;
  }
}