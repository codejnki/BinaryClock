
// button variables
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

	Serial.begin(9600);
	Serial.println("Hello world");
}

void loop()
{
	unsigned long currentMillis = millis();

	int hourRead = digitalRead(hourButton);
	int minuteRead = digitalRead(minuteButton);

	if(hourRead != lastHourButtonState || minuteRead != lastMinButtonState)
	{
		lastDebounceTime = currentMillis;
	}

	if((currentMillis - lastDebounceTime) > debounceDelay)
	{
		if(hourRead != hourButtonState)
		{
			hourButtonState = hourRead;

			if(hourButtonState == HIGH)
			{
				hour++;
				second = 0;
				newHour = 1;
			}
		}

		if(minuteRead != minButtonState)
		{
			minButtonState = minuteRead;

			if(minButtonState == HIGH)
			{
				minute++;
				second = 0;
				newMin = 1;
			}
		}
	}

	
	// Serial.println(currentMillis);

	if(currentMillis - previousMillis >= 1000) 
	{
		previousMillis = currentMillis;
		second++;
		//Serial.print("Second: ");
		//Serial.println(second);
	}
	
	if(second >= 60)
	{
		minute++;
		second = 0;
		newMin = 1;

		Serial.print("Minute: ");
		Serial.println(minute);
	}

	if (minute >= 60)
	{
		hour++;
		minute = 0;
		newHour = 1;

		Serial.print("Hour: ");
		Serial.println(hour);
	}

	if (hour >= 13)
	{
		hour = 1;
		minute = 0;
	}
		
	if(newMin)
	{
		convertToBitArray(minuteLightArray, minute);
		newMin = 0;
	}

	if(newHour)
	{
		convertToBitArray(hourLightArray, hour);
		newHour = 0;
	}
	
	int lightCounter = 26;
	for (int i = 0; i < 6; i++)
	{
		digitalWrite(minuteLeds[i],minuteLightArray[lightCounter]);
		lightCounter++;
	}

	lightCounter = 28;
	for (int i = 0; i < 4; i++)
	{
		digitalWrite(hourLeds[i], hourLightArray[lightCounter]);
		lightCounter++;
	}

	lastHourButtonState = hourRead;
	lastMinButtonState = minuteRead;
}

/**
*	Converts a value in to a byte array
*	
*	
*	byteArray The array (by ref) to fill
* value The integer value to convert
*
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
