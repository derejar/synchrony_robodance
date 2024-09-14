#include <RCSwitch.h>
RCSwitch RFReceiver = RCSwitch();
#define LEFT_ENGINE_FORWARD_PIN 7
#define LEFT_ENGINE_BACKWARD_PIN 4
#define LEFT_ENGINE_SPEED_PIN 5
#define RIGHT_ENGINE_FORWARD_PIN 9
#define RIGHT_ENGINE_BACKWARD_PIN 8
#define RIGHT_ENGINE_SPEED_PIN 6

#define START_RECORDING 11
#define STOP_RECORDING 10
#define START_DANCE_PLAYBACK 21
#define STOP_DANCE_PLAYBACK 20
#define STICK_UP 31
#define STICK_DOWN 30
#define STICK_RIGHT 33
#define STICK_LEFT 32

#define SPEED 255

boolean recordingStarted = false;
boolean danceStarted = false;

const int DANCE_LENGTH = 64;
int dance[DANCE_LENGTH];
int movesCount = 0;
int currentMove = 0;

unsigned long lastMoveTime = millis();
const unsigned long timeOnMove = 1000;

void turnLeft()
{
    digitalWrite(LEFT_ENGINE_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_ENGINE_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_ENGINE_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_ENGINE_BACKWARD_PIN, LOW);
    analogWrite(LEFT_ENGINE_SPEED_PIN, SPEED / 4);
    analogWrite(RIGHT_ENGINE_SPEED_PIN, SPEED * 0.66);
}

void turnRight()
{
    digitalWrite(LEFT_ENGINE_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_ENGINE_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_ENGINE_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_ENGINE_BACKWARD_PIN, LOW);
    analogWrite(LEFT_ENGINE_SPEED_PIN, SPEED * 0.77);
    analogWrite(RIGHT_ENGINE_SPEED_PIN, SPEED / 4);
}

void goForward()
{
    digitalWrite(LEFT_ENGINE_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_ENGINE_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_ENGINE_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_ENGINE_BACKWARD_PIN, LOW);
    analogWrite(LEFT_ENGINE_SPEED_PIN, SPEED);
    analogWrite(RIGHT_ENGINE_SPEED_PIN, SPEED);
}

void goBackward()
{
    digitalWrite(LEFT_ENGINE_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_ENGINE_FORWARD_PIN, LOW);
    digitalWrite(LEFT_ENGINE_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_ENGINE_BACKWARD_PIN, HIGH);
    analogWrite(LEFT_ENGINE_SPEED_PIN, SPEED);
    analogWrite(RIGHT_ENGINE_SPEED_PIN, SPEED);
}

void makeMove(int robotMove)
{
    switch(robotMove)
    {
        case STICK_UP: goForward(); break;
        case STICK_DOWN: goBackward(); break;
        case STICK_RIGHT: turnRight(); break;
        case STICK_LEFT: turnLeft(); break;
    }
}

void stopMoving()
{
    digitalWrite(LEFT_ENGINE_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_ENGINE_FORWARD_PIN, LOW);
    digitalWrite(LEFT_ENGINE_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_ENGINE_BACKWARD_PIN, LOW);
    analogWrite(LEFT_ENGINE_SPEED_PIN, 0);
    analogWrite(LEFT_ENGINE_SPEED_PIN, 0);
}
 
void setup() {
    RFReceiver.enableReceive(0);
    pinMode(LEFT_ENGINE_FORWARD_PIN, OUTPUT);
    pinMode(LEFT_ENGINE_BACKWARD_PIN, OUTPUT);
    pinMode(LEFT_ENGINE_SPEED_PIN, OUTPUT);
    pinMode(RIGHT_ENGINE_FORWARD_PIN, OUTPUT);
    pinMode(RIGHT_ENGINE_BACKWARD_PIN, OUTPUT);
    pinMode(RIGHT_ENGINE_SPEED_PIN, OUTPUT);

    stopMoving();
    Serial.begin(9600);
}

void loop() {
    unsigned long ms = millis();
    if(RFReceiver.available())
    {
        int value = RFReceiver.getReceivedValue();
        if(value == START_RECORDING && !recordingStarted && !danceStarted)
        {
            recordingStarted = true; 
            movesCount = 0;
        }
        else if(value == STOP_RECORDING && recordingStarted && !danceStarted)
            recordingStarted = false;
        else if(value == START_DANCE_PLAYBACK && !recordingStarted && !danceStarted && movesCount > 0)
            danceStarted = true;
        else if(value == STOP_DANCE_PLAYBACK && !recordingStarted && danceStarted)
            danceStarted = false;
        if((value == STICK_UP || value == STICK_DOWN || value == STICK_LEFT || value == STICK_RIGHT) && !danceStarted)
        {
            if(recordingStarted && movesCount < 64)
                dance[movesCount++] = value;
            makeMove(value); // Убрать для второго робота
            lastMoveTime = millis();
        }
        RFReceiver.resetAvailable();
    }
    else if(danceStarted && (ms - lastMoveTime) > timeOnMove)
    {
        makeMove(dance[currentMove++]);
        lastMoveTime = millis();
        if(currentMove == movesCount)
            danceStarted = false, currentMove = 0;
    }
    else if((ms - lastMoveTime) > timeOnMove)
    {
        stopMoving();
    }
    delay(100);
}
