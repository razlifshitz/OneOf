#include <Encoder.h>
Encoder myEnc(2, 3);

void setup()
{
	Serial.begin(115200);
	pinMode(5, OUTPUT);

	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	// digitalWrite(6, LOW);
	// analogWrite(5, 30);

	Serial.println("Basic Encoder Test:");
}

long oldPosition = -999;

void loop()
{
	long newPosition = myEnc.read();
	if (newPosition != oldPosition)
	{
		oldPosition = newPosition;
		Serial.println(newPosition);
	}
}
