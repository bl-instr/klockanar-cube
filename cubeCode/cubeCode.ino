#define BLINKY_DIAG         0
#define CUBE_DIAG           0
#define COMM_LED_PIN       16             //LED_BUILTIN 
#define RST_BUTTON_PIN     17             //16
#define LED_PIN    15
#define LED_COUNT          49             //48
#include <BlinkyPicoW.h>
#include <Adafruit_NeoPixel.h>
#include "glowRing.h"
#include "comet.h"
#include "collide.h"
#include "explode.h"
#include "pixClk.h"
#include "mood.h"
#include "pendulumWave.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);

GlowRing glowRing(&strip);
Comet comet(&strip, 255,8);
Collide collide(&strip, 255,8);
Explode explode(&strip, 255,8,2);
PixClk pixClk(&strip);
Mood mood(&strip);
PendulumWave pendulumWave(&strip);

struct CubeSetting
{
  uint16_t pattern;
  uint16_t clkHours;
  uint16_t clkMinutes;
  uint16_t updateClk;
  uint16_t clkSecHue;
  uint16_t clkMinHue;
  uint16_t clkHourHue;
  uint16_t clkBackgroundHue;
  uint16_t clkBackgroundValue;
  uint16_t clkTopPixel;
  uint16_t clkHandValue;
  uint16_t moodHue;
  uint16_t moodValue;
  uint16_t glowRingHueStep;
  uint16_t glowRingValue;
  uint16_t pendulumWaveHueStep;
  uint16_t pendulumWaveValue;
  uint16_t pendulumWaveMixMult;
};
CubeSetting setting;

struct CubeReading
{
  uint16_t clkReset;
};
CubeReading reading;
unsigned long lastPublishTime;
unsigned long publishInterval;
unsigned long lastBaseUpdate;
unsigned long baseInterval;

void setupBlinky()
{
  if ((BLINKY_DIAG > 0) || (CUBE_DIAG > 0))
  {
    Serial.begin(9600);
    delay(5000);
  }
  
  boolean  useFlashStorage = false;
/**/  
  BlinkyPicoW.setSsid("ssid");
  BlinkyPicoW.setWifiPassword("ssid_pw");
  BlinkyPicoW.setMqttServer("hub.bl-mc.com");
  BlinkyPicoW.setMqttUsername("klockanar");
  BlinkyPicoW.setMqttPassword("areallybadpassword");
  BlinkyPicoW.setBox("klockanar");
  BlinkyPicoW.setTrayType("klar");
  BlinkyPicoW.setTrayName("ghulla");  
  BlinkyPicoW.setCubeType("blinky-hub");
/**/ 
  
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
  baseInterval = 100;
  randomSeed(analogRead(0));
  strip.begin();
  delay(10);
  strip.clear();
  strip.show(); 
  glowRing.stop();
  comet.stop();
  collide.stop();
  explode.stop();
  pixClk.stop();
  pendulumWave.stop();

  setting.pattern = 0;
  setting.clkHours = 0;
  setting.clkMinutes = 0;
  setting.updateClk = 0;
  setting.clkSecHue = 0;
  setting.clkMinHue = 0;
  setting.clkHourHue = 0;
  setting.clkBackgroundHue = 0;
  setting.clkBackgroundValue = 0;
  setting.clkTopPixel = 24;
  setting.moodHue = 0;
  setting.moodValue = 0;
  reading.clkReset = 0;

  lastPublishTime = millis();
  pixClk.setClkResetTime(lastPublishTime);
  lastBaseUpdate = lastPublishTime;
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
    glowRing.stop();
    comet.stop();
    collide.stop();
    explode.stop();
    mood.stop();
    pixClk.stop();
    pendulumWave.stop();

    switch(setting.pattern)
    {
      case 1:
        glowRing.start();
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
        mood.start();
        break;
      case 6:
        pendulumWave.start();
        break;
      case 7:
        pixClk.start();
        break;
      default:
        break;
    }
    pixClk.setClkSecHue(setting.clkSecHue);
    pixClk.setClkMinHue(setting.clkMinHue);
    pixClk.setClkHourHue(setting.clkHourHue);
    pixClk.setClkBackgroundHue(setting.clkBackgroundHue);
    pixClk.setClkBackgroundValue(setting.clkBackgroundValue);
    pixClk.setClkTopPixel(setting.clkTopPixel);
    pixClk.setClkHandValue(setting.clkHandValue);
    mood.setMoodValue(setting.moodValue);
    mood.setMoodHue(setting.moodHue);
    glowRing.setValue(setting.glowRingValue);
    glowRing.setHueStep(setting.glowRingHueStep);
    pendulumWave.setHueStep(setting.pendulumWaveHueStep, setting.pendulumWaveMixMult, setting.pendulumWaveValue);
  }
  glowRing.loop();
  pendulumWave.loop();
  if ((now - lastBaseUpdate) > baseInterval)
  {
    comet.loop();
    collide.loop();
    explode.loop();
    mood.loop();
    pixClk.loop(now, &reading.clkReset);
  }
  if (setting.updateClk > 0)
  {
    setting.updateClk = 0;
    pixClk.setClkReferenceTime(setting.clkHours, setting.clkMinutes);
    pixClk.setClkResetTime(now);
  }

}
