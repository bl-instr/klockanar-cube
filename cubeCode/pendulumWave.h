class PendulumWave
{
  private:
    Adafruit_NeoPixel* strip;
    int ipixel = 0;
    uint16_t value;
    uint16_t hueStep;
    uint16_t mixMult;
    boolean started = false;
    uint16_t ledCount = 0;
    uint16_t ledHue[LED_COUNT];
        
  public:
    PendulumWave(Adafruit_NeoPixel* strip);
    void start();
    void stop();
    void loop();
    void setHueStep(uint16_t hueStep, uint16_t mixMult, uint16_t value);
};

PendulumWave::PendulumWave(Adafruit_NeoPixel* strip)
{
  this->strip = strip;
  this->ledCount = strip->numPixels();
  this->hueStep = 1;
  this->mixMult = 1;
  for (int ipix = 0; ipix < ledCount; ++ipix) ledHue[ipix] = 0;
}

void PendulumWave::start()
{
  strip->clear();
  strip->show();
  started = true;
  for (int ipix = 0; ipix < ledCount; ++ipix) ledHue[ipix] = 0;
}
void PendulumWave::stop()
{
  strip->clear();
  strip->show();
  started = false;
}
void PendulumWave::setHueStep(uint16_t hueStep, uint16_t mixMult, uint16_t value) 
{
  this->hueStep = 1;
  this->hueStep = this->hueStep << hueStep; // mult by 2
  this->mixMult = mixMult;
  this->value = value;
  for (int ipix = 0; ipix < ledCount; ++ipix) ledHue[ipix] = 0;
}

void PendulumWave::loop()
{
  if (!started) return;
  for (uint16_t ipix = 0; ipix < ledCount; ++ipix)
  {
    strip->setPixelColor(ipix, strip->ColorHSV(ledHue[ipix],255,(uint8_t) value));
    ledHue[ipix] = ledHue[ipix]  + hueStep + mixMult * ipix;
  }
  strip->show();
}
