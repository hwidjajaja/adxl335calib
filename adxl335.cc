// analog input pins for the arduino
const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

// define the raw min and max
// arduino nano has 1024 unique analog inputs
// therefore min is 0 and max is 1023
int MinRaw = 0;
int MaxRaw = 1023;

// take more than one sample
const int sampleSize = 10;

void setup() 
{
	// self explanatory what this does
	// connect to pc and baud rate
	analogReference(EXTERNAL);
	Serial.begin(9600);
}

void loop() 
{
	// read raw values
	// AxisAverage takes the average of the 10 samples
	// see below for the custom function
	int xRaw = AxisAverage(xPin);
	int yRaw = AxisAverage(yPin);
	int zRaw = AxisAverage(zPin);

	// this converts the raw values from whatever they are
	// 		to a scale of -3000 to 3000 since the ADXL335
	//		only measures from -3 to +3g.
	//		by adding the thousands, it creates a better
	//		accuracy.
	long xMapped = map(xRaw, MinRaw, MaxRaw, -3000, 3000);
	long yMapped = map(yRaw, MinRaw, MaxRaw, -3000, 3000);
	long zMapped = map(zRaw, MinRaw, MaxRaw, -3000, 3000);

	// re-scale the mapped value to fractional Gs
	float xAccel = xMapped / 1000.0;
	float yAccel = yMapped / 1000.0;
	float zAccel = zMapped / 1000.0;

	// prints as following:
	//
	// xRaw/yRaw/zRaw
	// xAccel/yAccel/zAccel
	//
	// Raw values represent output voltages of ADXL
	//		from 0 -> 3.3v, mapped from 0 -> 1023
	// Accel values represent the calculated G
	Serial.print(xRaw);
	Serial.print("/");
	Serial.print(yRaw);
	Serial.print("/");
	Serial.print(zRaw);
	Serial.println(xAccel,0);
	Serial.print("G/");
	Serial.print(yAccel,0);
	Serial.print("G/");
	Serial.print(zAccel,0);
	Serial.println("G");

	// 1 result/0.2 sec or 5 result/sec
	delay(200);
}

// Defines the AxisAverage function as the following
// 		to take samples and return the average
int AxisAverage(int axisPin)
{
	// set reading to 0
	long reading = 0;
	// read the analog output of the pin
	analogRead(axisPin);
	// wait 1ms to not overload
	delay(1);
	// for loop
	// set var i, add 1 to i until i = samplesize
	for (int i = 0; i < sampleSize; i++)
	{
	// add each of 10 readings to the reading var
	reading += analogRead(axisPin);
	}
	// divide the added total by the samplesize
	return reading/sampleSize;
}