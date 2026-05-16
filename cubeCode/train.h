class Train
{
  private:
    Adafruit_NeoPixel* strip;
    int ipixel = 0;
    uint16_t hue = 0;
    uint8_t brightness;
    uint8_t maxBrightness;
    boolean started = false;
    uint16_t ledCount = 0;
        
  public:
    Train(Adafruit_NeoPixel* strip, uint8_t maxBrightness);
    void start();
    void stop();
    void loop();

};

Train::Train(Adafruit_NeoPixel* strip, uint8_t maxBrightness)
{
  this->strip = strip;
  this->maxBrightness = maxBrightness;
  this->ledCount = strip->numPixels();
}

void Train::start()
{
  hue = (uint16_t) random(65535);
  ipixel = 0;
  brightness = maxBrightness;
  strip->clear();
  strip->show();
  started = true;
}
void Train::stop()
{
  strip->clear();
  strip->show();
  started = false;
}
void Train::loop()
{
  if (!started) return;
  strip->setPixelColor(ipixel, strip->ColorHSV(hue,255,brightness));
  strip->show();
  ipixel = ipixel + 1;
  if (ipixel >=  ledCount) 
  {
    ipixel = 0;
    if(brightness > 0)
    {
      brightness = 0;
    }
    else
    {
      hue = (uint16_t) random(65535);
      brightness = maxBrightness;
    }
  }
}
