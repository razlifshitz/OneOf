Encoder encoder(ENCODER1_PIN, ENCODER2_PIN);
VarSpeedServo myServo;

// SERVO

void attachServo()
{
    Serial.println("attachServo()");
    myServo.attach(SERVO_PIN); // attaches the servo on pin 9 to the servo object
}

void detachServo()
{
    Serial.println("detachServo()");
    myServo.detach();
}

// ENCODER

void attachEncoder(Encoder *encoder)
{
    Serial.println("Starting encoder");
    pinMode(DIR1_PWM_PIN, OUTPUT);
    pinMode(DIR2_PWM_PIN, OUTPUT);
    encoder->write(0);
    isEncoderMoving = false;
}

boolean detachEncoder()
{
    Serial.println("Stopping encoder");
    //stop motor
    digitalWrite(DIR1_PWM_PIN, LOW);
    digitalWrite(DIR2_PWM_PIN, LOW);

    delay(1000);

    // release motor hold
    pinMode(DIR1_PWM_PIN, INPUT);
    pinMode(DIR2_PWM_PIN, INPUT);
}

// GENERAL

void finishPain()
{
    detachServo();
    detachEncoder();
    active = false;
}