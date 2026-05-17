class GlowRing
{
  private:
    Adafruit_NeoPixel* strip;
    int ipixel = 0;
    uint16_t hue = 0;
    uint16_t value;
    uint16_t hueStep;
    boolean started = false;
    uint16_t ledCount = 0;
        
  public:
    GlowRing(Adafruit_NeoPixel* strip);
    void start();
    void stop();
    void loop();
    void setValue(uint16_t value) {this->value = value;}
    void setHueStep(uint16_t hueStep);
};

GlowRing::GlowRing(Adafruit_NeoPixel* strip)
{
  this->strip = strip;
  this->ledCount = strip->numPixels();
  this->hueStep = 1;
}

void GlowRing::start()
{
  hue = 0;
  strip->clear();
  strip->show();
  started = true;
}
void GlowRing::stop()
{
  strip->clear();
  strip->show();
  started = false;
}
void GlowRing::setHueStep(uint16_t hueStep2) 
{
  this->hueStep = 1;
  this->hueStep = this->hueStep << hueStep2;
  hue = 0;
}

void GlowRing::loop()
{
  if (!started) return;
  for (int ipix = 0; ipix < ledCount; ++ipix) strip->setPixelColor(ipix, strip->ColorHSV(hue,255,(uint8_t) value));
  uint16_t oldhue = hue;
  hue = hue + hueStep;
  if (hue < oldhue)
  {
    Serial.println(millis());
  }
  strip->show();
}
