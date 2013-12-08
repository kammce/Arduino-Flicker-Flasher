#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

char buffer[50] = { 0 };

void setup() {
	Serial.begin(9600);

	if (tcs.begin()) {
		Serial.println("Found sensor");
	} else {
		Serial.println("No TCS34725 found ... check your connections");
		while (1); // halt!
	}
	
	// thanks PhilB for this gamma table!
	// it helps convert RGB colors to what humans see
	for (int i=0; i<256; i++) {
		float x = i;
		x /= 255;
		x = pow(x, 2.5);
		x *= 255;
			
		if (commonAnode) {
			gammatable[i] = 255 - x;
		} else {
			gammatable[i] = x;      
		}
		//Serial.println(gammatable[i]);
	}
}


void loop() {
	uint16_t clear, red, green, blue;

	tcs.setInterrupt(false);      // turn on LED

	delay(60);  // takes 50ms to read 
	
	tcs.getRawData(&red, &green, &blue, &clear);

	tcs.setInterrupt(true);  // turn off LED
	
	// Figure out some basic hex code for visualization
	uint32_t sum = clear;
	float r, g, b;
	r = red; r /= sum;
	g = green; g /= sum;
	b = blue; b /= sum;
	r *= 256; g *= 256; b *= 256;

	int gr = gammatable[(int)r];
	int gg = gammatable[(int)g];
	int gb = gammatable[(int)b];

	char rr = (int)red >> 8;
	char rg = (int)green >> 8;
	char rb = (int)blue >> 8;

	//sprintf(buffer, "C:%d\t\tR:%d\t\tG:%d\t\tB:%d\t\t %x%x%x\n", clear, gr, gg, gb, gr, gg, gb);
	sprintf(buffer, "C:%d\t\tR:%d\t\tG:%d\t\tB:%d\t\t %x%x%x\n", clear, rr, rg, rb, rr, rg, rb);
	//sprintf(buffer, "C:%d\t\tR:%d\t\tG:%d\t\tB:%d\t\t %x%x%x\n", clear, r, g, b, r, g, b);

	Serial.print(buffer);
}
