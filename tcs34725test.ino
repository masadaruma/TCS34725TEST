#include <Wire.h>
#include "Adafruit_TCS34725softi2c.h"

// You can use any digital pin for emulate SDA / SCL
#define SDA_Lpin 8
#define SCL_Lpin 9
#define SDA_Rpin 10
#define SCL_Rpin 11

// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];

Adafruit_TCS34725softi2c tcs_L = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDA_Lpin, SCL_Lpin);
Adafruit_TCS34725softi2c tcs_R = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDA_Rpin, SCL_Rpin);

void setup()
{
    Serial.begin(9600);
    Serial.println("Color View Test!");

    if (tcs_L.begin())
    {
        Serial.println("Found sensor");
    }
    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1)
            ; // halt!
    }
    if (tcs_R.begin())
    {
        Serial.println("Found sensor");
    }
    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1)
            ; // halt!
    }

    // thanks PhilB for this gamma table!
    // it helps convert RGB colors to what humans see
    for (int i = 0; i < 256; i++)
    {
        float x = i;
        x /= 255;
        x = pow(x, 2.5);
        x *= 255;

        if (commonAnode)
        {
            gammatable[i] = 255 - x;
        }
        else
        {
            gammatable[i] = x;
        }
        // Serial.println(gammatable[i]);
    }
}

void loop()
{
    uint16_t clear_L, red_L, green_L, blue_L;
    uint16_t clear_R, red_R, green_R, blue_R;

    tcs_L.setInterrupt(false); // turn on LED
    tcs_R.setInterrupt(false); // turn on LED

    delay(60); // takes 50ms to read

    tcs_L.getRawData(&red_L, &green_L, &blue_L, &clear_L);
    tcs_R.getRawData(&red_R, &green_R, &blue_R, &clear_R);

    tcs_L.setInterrupt(true); // turn off LED
    tcs_R.setInterrupt(true); // turn off LED

    Serial.print("C_L:\t");
    Serial.print(clear_L);
    Serial.print("\tR_L:\t");
    Serial.print(red_L);
    Serial.print("\tG_L:\t");
    Serial.print(green_L);
    Serial.print("\tB_L:\t");
    Serial.print(blue_L);
    Serial.println("");

    Serial.print("C_R:\t");
    Serial.print(clear_R);
    Serial.print("\tR_R:\t");
    Serial.print(red_R);
    Serial.print("\tG_R:\t");
    Serial.print(green_R);
    Serial.print("\tB_R:\t");
    Serial.print(blue_R);
    Serial.println("");

    // Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );
}