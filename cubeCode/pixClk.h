#define MS_IN_12HRS         43200000
class PixClk
{
  private:
    Adafruit_NeoPixel* strip;
    uint8_t maxBrightness;
    boolean started = false;
    uint16_t ledCount = 0;
    unsigned long clkResetTime;
    unsigned long clkReferenceTimeSec;
    void setPixel(float fpix, uint16_t hue);
        
  public:
    PixClk(Adafruit_NeoPixel* strip, uint8_t maxBrightness);
    void start();
    void stop();
    void loop(unsigned long now, uint16_t* pclkReset);
    void setClkReferenceTime(uint16_t hours, uint16_t minutes);
    void setClkResetTime(unsigned long now);

};

PixClk::PixClk(Adafruit_NeoPixel* strip, uint8_t maxBrightness)
{
  this->strip = strip;
  this->maxBrightness = maxBrightness;
  this->ledCount = strip->numPixels();
}

void PixClk::start()
{
  strip->clear();
  strip->show();
  started = true;
}
void PixClk::stop()
{
  strip->clear();
  strip->show();
  started = false;
}
void PixClk::loop(unsigned long now, uint16_t* pclkReset)
{
  unsigned long plus12 = clkResetTime + MS_IN_12HRS;
  if (plus12 < clkResetTime) *pclkReset = 1; 
  if (now > plus12) clkResetTime = plus12;
  if (!started) return;
  
  float secRef = ((float) (now - clkResetTime)) / 1000;
  secRef = secRef + (float) clkReferenceTimeSec;
  
  float hours =    secRef / 3600;
  float minutes = (secRef / 60) - ((float) ((int) hours) * 60);
  float seconds =  secRef       - ((float) ((int) hours) * 3600) - ((float) ((int) minutes) * 60);
  if (hours > 12.0) hours = hours - 12.0;

  float maxPix = (float) (ledCount - 1);

  float ipixSec = (seconds / 60.0)  * maxPix + maxPix / 2.0;
  if (ipixSec > (float) (ledCount - 1)) ipixSec = ipixSec - maxPix;

  float ipixMin = (minutes / 60.0) * maxPix + maxPix / 2.0;
  if (ipixMin > (float) (ledCount - 1)) ipixMin = ipixMin - maxPix;
  
  float ipixHr   = (hours  / 12.0) * maxPix + maxPix / 2.0;
  if (ipixHr > (float) (ledCount - 1)) ipixHr = ipixHr - maxPix;

  for (int ipix = 0; ipix < ledCount; ++ipix) strip->setPixelColor(ipix, strip->ColorHSV(20000,255,0));
  setPixel(ipixSec, 20000);
  setPixel(ipixMin, 40000);
  setPixel(ipixHr,      0);
  strip->show();
}
void PixClk::setPixel(float fpix, uint16_t hue)
{
  int ipixLow = (int) fpix;
  int ipixHigh = fpix + 1;
  if (ipixHigh >= ledCount) ipixHigh = 0;
  float brightLow = 1.0 - (fpix - (float) ipixLow);
  float brightHigh = 1.0 - brightLow;
  brightLow = brightLow * ((float) maxBrightness);
  brightHigh = brightHigh * ((float) maxBrightness);
  strip->setPixelColor( ipixLow,  strip->ColorHSV(hue,255,(uint8_t) brightLow ));
  strip->setPixelColor( ipixHigh, strip->ColorHSV(hue,255,(uint8_t) brightHigh));
}
void PixClk::setClkReferenceTime(uint16_t hours, uint16_t minutes)
{
  clkReferenceTimeSec = hours * 3600 + minutes * 60;
}
void PixClk::setClkResetTime(unsigned long now)
{
  clkResetTime = now;
}
