#define BLINKY_DIAG         0
#define CUBE_DIAG           0
#define COMM_LED_PIN       LED_BUILTIN
#define RST_BUTTON_PIN     16
#include <BlinkyPicoW.h>
#include <Adafruit_NeoPixel.h>
#include "train.h"
#include "comet.h"
#include "collide.h"
#include "explode.h"
#include "pixClk.h"
#define LED_PIN    15
#define LED_COUNT 48

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);

Train train(&strip, 32);
Comet comet(&strip, 255,8);
Collide collide(&strip, 255,8);
Explode explode(&strip, 255,8,2);
PixClk pixClk(&strip, 32);

struct CubeSetting
{
  uint16_t pattern;
  uint16_t clkHours;
  uint16_t clkMinutes;
  uint16_t updateClk;
};
CubeSetting setting;

struct CubeReading
{
  uint16_t clkReset;
};
CubeReading reading;
unsigned long lastPublishTime;
unsigned long publishInterval;

void setupBlinky()
{
  if ((BLINKY_DIAG > 0) || (CUBE_DIAG > 0))
  {
    Serial.begin(9600);
    delay(5000);
  }
  
  boolean  useFlashStorage = true;
/*  
  BlinkyPicoW.setSsid("Blinky-Lite");
  BlinkyPicoW.setWifiPassword("SwedishChef");
  BlinkyPicoW.setMqttServer("hub.bl-mc.com");
  BlinkyPicoW.setMqttUsername("blinkybox-demo");
  BlinkyPicoW.setMqttPassword("areallybadpassword");
  BlinkyPicoW.setBox("blinkybox-demo");
  BlinkyPicoW.setTrayType("picoW");
  BlinkyPicoW.setTrayName("klar-04-8d-ae");
  BlinkyPicoW.setCubeType("blinky-hub");
*/ 
  
  BlinkyPicoW.setMqttKeepAlive(15);
  BlinkyPicoW.setMqttSocketTimeout(4);
  BlinkyPicoW.setMqttPort(1883);
  BlinkyPicoW.setMqttLedFlashMs(100);
  BlinkyPicoW.setHdwrWatchdogMs(8000);
  BlinkyPicoW.setRouterDelay(10000);

  BlinkyPicoW.begin(BLINKY_DIAG, COMM_LED_PIN, RST_BUTTON_PIN, useFlashStorage, sizeof(setting), sizeof(reading));
}


void setupCube() 
{
  if ((BLINKY_DIAG == 0) || (CUBE_DIAG > 0))
  {
    Serial.begin(9600);
    delay(5000);
  }

  publishInterval = 5000;
  randomSeed(analogRead(0));
  strip.begin();
  delay(10);
  strip.clear();
  strip.show(); 
  train.stop();
  comet.stop();
  collide.stop();
  explode.stop();
  pixClk.stop();

  setting.pattern = 0;
  setting.clkHours = 5;
  setting.clkMinutes = 52;
  setting.updateClk = 0;
  reading.clkReset = 0;

  lastPublishTime = millis();
  pixClk.setClkResetTime(lastPublishTime);
}

void loopCube() 
{
  unsigned long now = millis();
  
  if ((now - lastPublishTime) > publishInterval)
  {
    lastPublishTime = now;
    BlinkyPicoW.publishCubeData((uint8_t*) &setting, (uint8_t*) &reading, false);
  }
  if (BlinkyPicoW.retrieveCubeSetting((uint8_t*) &setting))
  {
    train.stop();
    comet.stop();
    collide.stop();
    explode.stop();
    pixClk.stop();
    switch(setting.pattern)
    {
      case 1:
        train.start();
        break;
      case 2:
        comet.start();
        break;
      case 3:
        collide.start();
        break;
      case 4:
        explode.start();
        break;
      case 5:
        pixClk.start();
        break;
      default:
        break;
    }
  }
  train.loop();
  comet.loop();
  collide.loop();
  explode.loop();
  pixClk.loop(now, &reading.clkReset);
  delay(5);
  if (setting.updateClk > 0)
  {
    setting.updateClk = 0;
    pixClk.setClkReferenceTime(setting.clkHours, setting.clkMinutes);
    pixClk.setClkResetTime(now);
  }
}
