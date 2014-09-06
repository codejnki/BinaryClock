// much more compact version using a DS1307 RTC module
// Wire library and RTC library are needed
#include <Wire.h>
#include <RTClib.h>

// this array holds our hour led pin positions
int hourLeds[4] = { 7, 6, 5, 4 };
int hourLightArray[32] = {0};

// this array holds our minute led pin positions
int minuteLeds[6] = { 32 , 30, 28, 26, 24, 22 };
int minuteLightArray[32] = {0};

// variables to hold current time to try and be a little
// more power efficient
int currentMinute = 0;
int currentHour = 0;
long previousMillis = 0;
long interval = 1;

// Create our RTC class
RTC_DS1307 RTC;

void setup()
{
	// Setup the RTC 
	Wire.begin();
	RTC.begin();

	// adjusts the RTC to the current time
	RTC.adjust(DateTime(__DATE__,__TIME__));

	// initialize our hour pins
	for (int i = 0; i < 4; i++)
	{
		pinMode(hourLeds[i], OUTPUT);
	}

	// initialize our minute pins
	for (int i = 0; i < 6; i++)
	{
		pinMode(minuteLeds[i], OUTPUT);
	}
}

void loop()
{
	DateTime now = RTC.now();
	int hour = now.hour();
	int minute = now.minute();

	// The clock currently reads in 12 hour format
	// change here to go to 24 hour format
	if (hour == 0)
	{
		hour = 12;
	}

	if (hour >= 13)
	{
		hour = hour - 12;
	}

	if (currentHour != hour)
	{
		// update the current hour and then update the LED array
		currentHour = hour;
		convertToBitArray(hourLightArray, currentHour);
	}

	if (currentMinute != minute)
	{
		// update the current min and then updat the LED array
		currentMinute = minute;
		convertToBitArray(minuteLightArray, minute);
	}

	unsigned long currentMillis = millis();
	if(currentMillis - previousMillis > interval)
	{
		previousMillis = currentMillis;
		// so we converted the minutes in to a 32 bit length array
		// but we only are going to need the last 6 array elements
		int lightCounter = 26;
		for (int i = 0; i < 6; i++)
		{
			digitalWrite(minuteLeds[i],minuteLightArray[lightCounter]);
			lightCounter++;
		}

		// we converted the hours in to a 32 bit length array
		// but we are only going to need the last 4 array elements
		lightCounter = 28;
		for (int i = 0; i < 4; i++)
		{
			digitalWrite(hourLeds[i], hourLightArray[lightCounter]);
			lightCounter++;
		}
	}
	else
	{
		int lightCounter = 26;
		for (int i = 0; i < 6; i++)
		{
			digitalWrite(minuteLeds[i],LOW);
			lightCounter++;
		}

		// we converted the hours in to a 32 bit length array
		// but we are only going to need the last 4 array elements
		lightCounter = 28;
		for (int i = 0; i < 4; i++)
		{
			digitalWrite(hourLeds[i], LOW);
			lightCounter++;
		}
	}
}

/**
*	Converts a value in to a 32 byte array
*	
*	
*	byteArray The array (by ref) to fill
* value The integer value to convert
* mostly copied from:
* http://www.programmingsimplified.com/c/source-code/c-program-convert-decimal-to-binary
**/
void convertToBitArray(int* byteArray, int value)
{
	int n, c, k;
	int counter;

	n = value;

	for (c = 31; c >= 0; c--)
	{
		k = n >> c;

		if(k & 1)
		{
			byteArray[counter] = 1;
		}
		else
		{
			byteArray[counter] = 0;
		}
			
		counter++;
	}
}
