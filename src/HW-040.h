#define CLK 8 // CLK pin connected to digital pin 2
#define DT 3  // DT pin connected to digital pin 3
#define SW 18 // SW pin connected to digital pin 4

int counter = 0;
int currentState;
int lastState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;
bool lastSwitchState = HIGH;

void Knobinit()
{
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);
    lastState = digitalRead(CLK);
}
