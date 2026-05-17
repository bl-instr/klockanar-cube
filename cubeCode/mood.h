class Mood
{
  private:
    Adafruit_NeoPixel* strip;
    uint16_t value;
    uint16_t hue;
    boolean started = false;
    uint16_t ledCount = 0;
        
  public:
    Mood(Adafruit_NeoPixel* strip);
    void start();
    void stop();
    void loop();
    void setMoodValue(uint16_t value) {this->value = value;}
    void setMoodHue(uint16_t hue) {this->hue = hue;}

};

Mood::Mood(Adafruit_NeoPixel* strip)
{
  this->strip = strip;
  this->ledCount = strip->numPixels();
}

void Mood::start()
{
  strip->clear();
  strip->show();
  started = true;
}
void Mood::stop()
{
  strip->clear();
  strip->show();
  started = false;
}
void Mood::loop()
{
  if (!started) return;
  for (int ipix = 0; ipix < ledCount; ++ipix) strip->setPixelColor(ipix, strip->ColorHSV(hue,255,(uint8_t) value));
  strip->show();
}
