#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class FansControlDisplay : public Adafruit_SSD1306
{
public:
    FansControlDisplay(TwoWire &wire, uint8_t i2c_addr = 0x3C) : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire)

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

    void progressBar(int x, int y, int w, int h, int percent)
    {
        int width = w;
        int height = h;
        int barWidth = map(percent, 0, 100, 0, width);
        fillRect(x, y, barWidth, height, WHITE);
        drawRect(x, y, width, height, WHITE);
    }

    void update()
    {
        // Serial.println("Updating display");
        clearDisplay();
        setTextSize(1);
        setTextColor(WHITE);
        setCursor(0, 10);
        println("FANS");
        setCursor(50, 10);
        println("SPEED(%)");
        setCursor(0, 25);
        println("HEATER");
        setCursor(50, 25);
        println(String(heaterFanSpeed));
        progressBar(70, 25, 50, 5, heaterFanSpeed);
        setCursor(0, 40);
        println("COOLER");
        setCursor(50, 40);
        println(String(coolerFanSpeed));
        progressBar(70, 40, 50, 5, coolerFanSpeed);
        setCursor(0, 55);
        println("VENT");
        setCursor(50, 55);
        println(String(ventFanSpeed));
        progressBar(70, 55, 50, 5, ventFanSpeed);
        display();
    }

    void setHeaterFanSpeed(int speed)
    {
        heaterFanSpeed = speed;
    }

    void setCoolerFanSpeed(int speed)
    {
        coolerFanSpeed = speed;
    }

    void setVentFanSpeed(int speed)
    {
        ventFanSpeed = speed;
    }

private:
    int heaterFanSpeed = 0;
    int coolerFanSpeed = 0;
    int ventFanSpeed = 0;
    uint8_t i2c_addr;
};