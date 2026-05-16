class Collide
{
  private:
    Adafruit_NeoPixel* strip;
    int ipixel = 0;
    uint16_t hue = 0;
    uint8_t brightness;
    uint8_t maxBrightness;
    boolean started = false;
    uint16_t ledCount = 0;
    int cometLength = 1;
    int forward = 1;
    uint16_t reverseHue = 0;

  public:
    Collide(Adafruit_NeoPixel* strip, uint8_t maxBrightness, int cometLength);
    void start();
    void stop();
    void loop();
};

Collide::Collide(Adafruit_NeoPixel* strip, uint8_t maxBrightness, int cometLength)
{
  this->strip = strip;
  this->maxBrightness = maxBrightness;
  this->ledCount = strip->numPixels();
  this->cometLength = cometLength;
}

void Collide::start()
{
  hue = 0;
  ipixel = 0;
  brightness = maxBrightness;
  strip->clear();
  strip->show();
  started = true;
  forward = 1;
}
void Collide::stop()
{
  strip->clear();
  strip->show();
  started = false;
  forward = 1;
}
void Collide::loop()
{
 if (!started) return;
  int iipix = ipixel;
  for (int ii = 0; ii < cometLength; ++ii)
  {
    int iibright = (cometLength - 1 - ii) * (cometLength - 1 - ii) * ((int) maxBrightness);
    iibright = iibright / ((cometLength - 1) * (cometLength - 1));
    if ((0 <= iipix) && (iipix <= (ledCount / 2)))
    {
      if (ii == 0)
      {
        strip->setPixelColor(iipix, maxBrightness, maxBrightness, maxBrightness);
        strip->setPixelColor(ledCount - 1 - iipix, maxBrightness, maxBrightness, maxBrightness);
      }
      else
      {
        if (forward > 0)
        {
          if (iipix ==  (ledCount / 2))
          {
            strip->setPixelColor(iipix, maxBrightness, maxBrightness, maxBrightness);
            strip->setPixelColor(ledCount - 1 - iipix, maxBrightness, maxBrightness, maxBrightness);
          }
          else
          {
            strip->setPixelColor(iipix, strip->ColorHSV((uint16_t) random(65535),255,(uint8_t) iibright));
            strip->setPixelColor(ledCount - 1 - iipix, strip->ColorHSV((uint16_t) random(65535),255,(uint8_t) iibright));
          }
        }
        else
        {
          strip->setPixelColor(iipix, strip->ColorHSV(reverseHue,255,(uint8_t) iibright));
          strip->setPixelColor(ledCount - 1 - iipix, strip->ColorHSV(reverseHue,255,(uint8_t) iibright));
        }
      }
      strip->show();
    }
    iipix = iipix - forward;
  }
  if (forward > 0)
  {
    if (iipix < (ledCount / 2))
    {
      if (iipix >= 0)
      {
        strip->setPixelColor(iipix, strip->ColorHSV(hue,255,0));
        strip->setPixelColor(ledCount - 1 - iipix, strip->ColorHSV(hue,255,0));
      }
      strip->show();
      ipixel = ipixel + forward;
    }
    else
    {
      ipixel = ledCount / 2;
      forward = -1;
      reverseHue = (uint16_t) random(65535);
    }
  }
  else
  {
    if (iipix >= 0)
    {
      if (iipix < ledCount / 2) 
      {
        strip->setPixelColor(iipix, strip->ColorHSV(hue,255,0));
        strip->setPixelColor(ledCount - 1 - iipix, strip->ColorHSV(hue,255,0));
      }
      strip->show();
      ipixel = ipixel + forward;
    }
    else
    {
      ipixel = -1;
      forward = 1;
    }
  }
}
