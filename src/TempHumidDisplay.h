#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class TempHumidDisplay : public Adafruit_SSD1306
{
public:
    TempHumidDisplay(TwoWire &wire, uint8_t i2c_addr = 0x3C) : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire)
    {
        this->i2c_addr = i2c_addr;
    }

    void init()
    {
        if (!begin(SSD1306_SWITCHCAPVCC, i2c_addr))
        { // Address 0x3C for 128x64
            Serial.println(F("SSD1306 allocation failed"));
        }
        clearDisplay();
    }

    void update()
    {

        clearDisplay();
        setTextSize(1);
        setTextColor(WHITE);
        setCursor(0, 10);
        println("POS");
        setCursor(35, 10);
        println("TEMP(C)");
        setCursor(90, 10);
        println("HUM(%)");
        setCursor(0, 25);
        println("HET");
        setCursor(35, 25);
        println(String(top_temp));
        setCursor(90, 25);
        println(String(top_humid));
        setCursor(0, 40);
        println("INT");
        setCursor(35, 40);
        println(String(mid_temp));
        setCursor(90, 40);
        println(String(mid_humid));
        setCursor(0, 55);
        println("AMB");
        setCursor(35, 55);
        println(String(bot_temp));
        setCursor(90, 55);
        println(String(bot_humid));
        display();
    }

    void setTopTemp(float temp)
    {
        top_temp = temp;
    }

    void setTopHumid(float humid)
    {
        top_humid = humid;
    }

    void setMidTemp(float temp)
    {
        mid_temp = temp;
    }

    void setMidHumid(float humid)
    {
        mid_humid = humid;
    }

    void setBotTemp(float temp)
    {
        bot_temp = temp;
    }

    void setBotHumid(float humid)
    {
        bot_humid = humid;
    }

private:
    float top_temp = 0;
    float top_humid = 0;
    float mid_temp = 0;
    float mid_humid = 0;
    float bot_temp = 0;
    float bot_humid = 0;
    uint8_t i2c_addr;
};