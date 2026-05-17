#define MS_IN_12HRS         43200000
class PixClk
{
  private:
    Adafruit_NeoPixel* strip;
    boolean started = false;
    uint16_t ledCount = 0;
    unsigned long clkResetTime;
    unsigned long clkReferenceTimeSec;
    uint16_t secHue;
    uint16_t minHue;
    uint16_t hourHue;
    uint16_t backgroundHue;
    uint16_t backgroundValue;
    uint16_t topPixel;
    uint16_t handValue;
    void setPixel(float fpix, uint16_t hue);
        
  public:
    PixClk(Adafruit_NeoPixel* strip);
    void start();
    void stop();
    void loop(unsigned long now, uint16_t* pclkReset);
    void setClkReferenceTime(uint16_t hours, uint16_t minutes);
    void setClkResetTime(unsigned long now);

    void setClkSecHue(uint16_t secHue) {this->secHue = secHue;}
    void setClkMinHue(uint16_t minHue) {this->minHue = minHue;}
    void setClkHourHue(uint16_t hourHue) {this->hourHue = hourHue;}
    void setClkBackgroundHue(uint16_t backgroundHue) {this->backgroundHue = backgroundHue;}
    void setClkBackgroundValue(uint16_t backgroundValue) {this->backgroundValue = backgroundValue;}
    void setClkTopPixel(uint16_t topPixel) {this->topPixel = topPixel;}
    void setClkHandValue(uint16_t handValue) {this->handValue = handValue;}
};

PixClk::PixClk(Adafruit_NeoPixel* strip)
{
  this->strip = strip;
  this->ledCount = strip->numPixels();
  this->secHue = 20000;
  this->minHue = 40000;
  this->hourHue = 0;
  this->backgroundHue = 51882;
  this->backgroundValue = 20;
  this->topPixel = 24;
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

  float maxPix = (float) (ledCount - 0);
  float ftopPixel = (float) topPixel;

  float ipixSec = (seconds / 60.0)  * maxPix + ftopPixel;
  if (ipixSec >= maxPix) ipixSec = ipixSec - maxPix;

  float ipixMin = (minutes / 60.0) * maxPix  + ftopPixel;
  if (ipixMin > maxPix) ipixMin = ipixMin - maxPix;
  
  float ipixHr   = (hours  / 12.0) * maxPix  + ftopPixel;
  if (ipixHr > maxPix) ipixHr = ipixHr - maxPix;

  for (int ipix = 0; ipix < ledCount; ++ipix) strip->setPixelColor(ipix, strip->ColorHSV(backgroundHue,255,backgroundValue));
  setPixel(ipixSec, secHue );
  setPixel(ipixMin, minHue);
  setPixel(ipixHr,  hourHue);
  strip->show();
}
void PixClk::setPixel(float fpix, uint16_t hue)
{
  int ipixLow = (int) fpix;
  int ipixHigh = ipixLow + 1;
  if (ipixHigh >= ledCount) ipixHigh = 0;
  float brightLow = 1.0 - (fpix - (float) ipixLow);
  float brightHigh = 1.0 - brightLow;
  brightLow  = brightLow  * (((float) handValue) - ((float) backgroundValue)) + ((float) backgroundValue);
  brightHigh = brightHigh * (((float) handValue) - ((float) backgroundValue)) + ((float) backgroundValue);
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
