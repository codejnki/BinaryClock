
// These variables control the button state
const int hourButton = 3;
const int minuteButton = 2;
int hourButtonState;
int minButtonState;
int lastHourButtonState = LOW;
int lastMinButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

// this array holds our hour led pin positions
int hourLeds[4] = { 7, 6, 5, 4 };
int hourLightArray[32] = {0};

// this array holds our minute led pin positions
int minuteLeds[6] = { 32 , 30, 28, 26, 24, 22 };
int minuteLightArray[32] = {0};

// time global variables
long previousMillis = 0;
int newMin = 0;
int newHour = 1;
int second = 0;
int minute = 0;
int hour = 12;

void setup()
{
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

	// setup the buttons to read
	pinMode(hourButton, INPUT);
	pinMode(minuteButton, INPUT);
}

void loop()
{
	unsigned long currentMillis = millis();

	// read our buttons
	int hourRead = digitalRead(hourButton);
	int minuteRead = digitalRead(minuteButton);

	if(hourRead != lastHourButtonState || minuteRead != lastMinButtonState)
	{
		// set the debounce
		lastDebounceTime = currentMillis;
	}

	if((currentMillis - lastDebounceTime) > debounceDelay)
	{
		// something has been pressed and crossed the debounce threshold
		if(hourRead != hourButtonState)
		{
			hourButtonState = hourRead;

			if(hourButtonState == HIGH)
			{
				// increase the hour, reset the seconds, and set the new hour flag
				// so that the hour light array is updated
				hour++;
				second = 0;
				newHour = 1;
			}
		}

		if(minuteRead != minButtonState)
		{
			// increase the minute, reset the seconds, and set the new minute flag
			// so that the minute array is updated
			minButtonState = minuteRead;

			if(minButtonState == HIGH)
			{
				minute++;
				second = 0;
				newMin = 1;
			}
		}
	}

	

	if(currentMillis - previousMillis >= 1000) 
	{
		previousMillis = currentMillis;
		second++;
	}
	
	if(second >= 60)
	{
		minute++;
		second = 0;
		newMin = 1;
	}

	if (minute >= 60)
	{
		hour++;
		minute = 0;
		newHour = 1;
	}

	// The clock currently reads in 12 hour format
	// change here to go to 24 hour format
	if (hour >= 13)
	{
		hour = 1;
		minute = 0;
	}
		
	if(newMin)
	{
		// if a new minute has happened update our light array 
		convertToBitArray(minuteLightArray, minute);
		newMin = 0;
	}

	if(newHour)
	{
		// if a new hour has happened update the light array
		convertToBitArray(hourLightArray, hour);
		newHour = 0;
	}
	
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

	// lastly capture the button state
	lastHourButtonState = hourRead;
	lastMinButtonState = minuteRead;
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
