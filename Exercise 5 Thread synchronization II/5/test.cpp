#include <stdio.h>
#include <stdlib.h>
#include <RaspberryDLL.h>

bool isDark()
{
    int lightIntensity = getIntensity();
    return lightIntensity < 50; // Adjust the threshold as per your requirement
}

int main(void)
{
    if (!Open())
    {
        printf("Error with connection\n");
        exit(1);
    }

    printf("Connected to Raspberry Pi\n");

    // Get temperature
    double temperature = getTemperature();
    printf("Temperature in home: %.2f°C\n", temperature);

    // Check if it's dark
    if (isDark())
    {
        printf("It's dark. Turning on LEDs...\n");
        ledOn(LD5);
        ledOn(LD6);
    }
    else
    {
        printf("It's not dark. Keeping LEDs off.\n");
        ledOff(LD5);
        ledOff(LD6);
    }

    return 0;
}
