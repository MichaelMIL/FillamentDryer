#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class ControlsMenuDisplay : public Adafruit_SSD1306
{
public:
    ControlsMenuDisplay(TwoWire &wire, uint8_t i2c_addr = 0x3C) : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire)

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
    unsigned long lastUpdate = 0;
    void CW()
    {
        if (!subMenu)
        {
            selectedItem++;
            if (selectedItem > items)
            {
                selectedItem = 0;
            }
            update();
        }
        else
        {
            increaseSelectedItemSpeed();
            if (millis() - lastUpdate > 50)
            {
                lastUpdate = millis();
                update();
            }
        }
    }
    void CCW()
    {
        if (!subMenu)
        {
            selectedItem--;
            if (selectedItem < 0)
            {
                selectedItem = items;
            }
            update();
        }
        else
        {
            decreaseSelectedItemSpeed();
            if (millis() - lastUpdate > 50)
            {
                lastUpdate = millis();
                update();
            }
        }
    }
    void click()
    {
        if (subMenu)
        {
            setNewState();
            if (selectedItem == 5)
            {
                isAutoMode = true;
            }
        }
        subMenu = !subMenu;
        update();
    }
    void printMenuItem(String txt, bool inverted = false)
    {
        setTextSize(1);
        if (inverted)
        {
            // create black text on white square
            setTextColor(BLACK, WHITE);
            println(txt);
        }
        else
        {
            setTextColor(WHITE);
            println(txt);
        }
    }
    void progressBar(int x, int y, int w, int h, int percent)
    {
        int width = w;
        int height = h;
        int barWidth = map(percent, 0, 100, 0, width);
        fillRect(x, y, barWidth, height, WHITE);
        drawRect(x, y, width, height, WHITE);
    }

    void printSubMenu()
    {
        setTextSize(2);
        setTextColor(WHITE);
        setCursor(0, 0);
        println(menuItems[selectedItem]);
        setCursor(0, 20);
        if (selectedItem == 0)
        {
            setTextSize(1);
            println("Speed");
            setCursor(0, 35);
            println(String(heaterFanSpeed));
            progressBar(50, 35, 70, 5, heaterFanSpeed);
        }
        else if (selectedItem == 1)
        {
            setTextSize(1);
            println("Speed");
            setCursor(0, 35);
            println(String(coolerFanSpeed));
            progressBar(50, 35, 70, 5, coolerFanSpeed);
        }
        else if (selectedItem == 2)
        {
            setTextSize(1);
            println("Speed");
            setCursor(0, 35);
            println(String(ventFanSpeed));
            progressBar(50, 35, 70, 5, ventFanSpeed);
        }
        else if (selectedItem == 3)
        {
            setTextSize(1);
            println("State");
            setCursor(0, 35);
            printMenuItem("On", heaterState == true);
            setCursor(50, 35);
            printMenuItem("Off", heaterState == false);
        }
        else if (selectedItem == 4)
        {
            setTextSize(1);
            println("State");
            setCursor(0, 35);
            printMenuItem("On", coolerState == true);
            setCursor(50, 35);
            printMenuItem("Off", coolerState == false);
        }
        else if (selectedItem == 5)
        {
            setTextSize(1);
            println("Auto runtime (Hours)");
            setCursor(0, 35);
            println(String(autoModeTimer));
            progressBar(50, 35, 70, 5, autoModeTimer / 20.0 * 100.0);
        }
    }

    void update()
    {
        clearDisplay();
        if (!subMenu)
        {
            setTextColor(WHITE);
            setTextSize(2);
            setCursor(0, 0);
            println("FANS");
            setCursor(0, 20);
            printMenuItem("Heater " + String(heaterFanSpeed), selectedItem == 0);
            setCursor(0, 35);
            printMenuItem("Cooler " + String(coolerFanSpeed), selectedItem == 1);
            setCursor(0, 50);
            printMenuItem("Vent " + String(ventFanSpeed), selectedItem == 2);
            setTextSize(2);
            setTextColor(WHITE);
            setCursor(65, 0);
            println("ELM.");
            setTextSize(1);
            setCursor(65, 20);
            printMenuItem(heaterState ? String("Heater ON") : String("Heater OFF"), selectedItem == 3);
            setCursor(65, 35);
            printMenuItem(coolerState ? String("Cooler ON") : String("Cooler OFF"), selectedItem == 4);
            setCursor(65, 50);
            printMenuItem("Auto " + String(autoModeTimer), selectedItem == 5);
        }
        else
        {
            printSubMenu();
        }
        display();
    }

    int getHeaterFanSpeed()
    {
        return heaterFanSpeed;
    }
    void setHeaterFanSpeed(int value)
    {
        heaterFanSpeed = value;
    }
    int getCoolerFanSpeed()
    {
        return coolerFanSpeed;
    }

    void setCoolerFanSpeed(int value)
    {
        coolerFanSpeed = value;
    }
    int getVentFanSpeed()
    {
        return ventFanSpeed;
    }

    void setVentFanSpeed(int value)
    {
        ventFanSpeed = value;
    }
    bool getHeaterState()
    {
        return heaterState;
    }

    void setHeaterState(bool value)
    {
        heaterState = value;
    }
    bool getCoolerState()
    {
        return coolerState;
    }
    void setCoolerState(bool value)
    {
        coolerState = value;
    }
    bool getNewState()
    {
        return isNewState;
    }

    void clearNewState()
    {
        isNewState = false;
    }
    float getAutoModeTimer()
    {
        return autoModeTimer;
    }
    void setAutoModeTimer(float value)
    {
        autoModeTimer = value;
    }
    bool getIsAutoMode()
    {
        return isAutoMode;
    }

    void setIsAutoMode(bool value)
    {
        isAutoMode = value;
    }

private:
    uint8_t i2c_addr;
    int items = 6;
    int selectedItem = 0;
    String menuItems[6] = {"Heater", "Cooler", "Vent", "Heater", "Cooler", "auto"};
    bool subMenu = false;
    int heaterFanSpeed = 0;
    int coolerFanSpeed = 0;
    int ventFanSpeed = 0;
    bool coolerState = false;
    bool heaterState = false;
    bool isNewState = false;
    float autoModeTimer = 0;
    bool isAutoMode = false;

    void setNewState()
    {
        isNewState = true;
    }

    void increaseSelectedItemSpeed()
    {
        if (selectedItem == 0)
        {
            heaterFanSpeed++;
            if (heaterFanSpeed > 100)
            {
                heaterFanSpeed = 100;
            }
        }
        else if (selectedItem == 1)
        {
            coolerFanSpeed++;
            if (coolerFanSpeed > 100)
            {
                coolerFanSpeed = 100;
            }
        }
        else if (selectedItem == 2)
        {
            ventFanSpeed++;
            if (ventFanSpeed > 100)
            {
                ventFanSpeed = 100;
            }
        }
        else if (selectedItem == 3)
        {
            heaterState = !heaterState;
        }
        else if (selectedItem == 4)
        {
            coolerState = !coolerState;
        }
        else if (selectedItem == 5)
        {
            autoModeTimer = autoModeTimer + 0.5;
            if (autoModeTimer > 20.0)
            {
                autoModeTimer = 20.0;
            }
        }
    }
    void decreaseSelectedItemSpeed()
    {
        if (selectedItem == 0)
        {
            heaterFanSpeed--;
            if (heaterFanSpeed < 0)
            {
                heaterFanSpeed = 0;
            }
        }
        else if (selectedItem == 1)
        {
            coolerFanSpeed--;
            if (coolerFanSpeed < 0)
            {
                coolerFanSpeed = 0;
            }
        }
        else if (selectedItem == 2)
        {
            ventFanSpeed--;
            if (ventFanSpeed < 0)
            {
                ventFanSpeed = 0;
            }
        }
        else if (selectedItem == 3)
        {
            heaterState = !heaterState;
        }
        else if (selectedItem == 4)
        {
            coolerState = !coolerState;
        }
        else if (selectedItem == 5)
        {
            autoModeTimer = autoModeTimer - 0.5;
            if (autoModeTimer <= 0.0)
            {
                autoModeTimer = 0.0;
            }
        }
    }
};
