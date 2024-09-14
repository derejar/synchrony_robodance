#include <RCSwitch.h>
RCSwitch rcSwitch = RCSwitch();

#define RF_TRANSMITTER_PIN 10 // порт передатчика
#define POS_X_PIN 0 // Порт для определения координаты Х для стика
#define POS_Y_PIN 1 // Порт для определения координаты Y для стика
#define START_RECORDING_PIN 2 // Порт для кнопки начала записи
#define STOP_DANCE_PLAYBACK_PIN 3 // Порт для кнопки конца танца
#define START_DANCE_PLAYBACK_PIN 4 // Порт для кнопки начала танца
#define STOP_RECORDING_PIN 5 // Порт для кнопки остановки записи

#define X_UP_BORDER 900 // Верхняя граница по Х
#define X_DOWN_BORDER 100 // Нижняя граница по Х
#define Y_UP_BORDER 900 // Верхняя граница по Y
#define Y_DOWN_BORDER 100 // Нижняя граница по Y

#define START_RECORDING 11
#define STOP_RECORDING 10
#define START_DANCE_PLAYBACK 21
#define STOP_DANCE_PLAYBACK 20
#define STICK_UP 31
#define STICK_DOWN 30
#define STICK_RIGHT 33
#define STICK_LEFT 32

void setup() {
    rcSwitch.enableTransmit(RF_TRANSMITTER_PIN);
    rcSwitch.setRepeatTransmit(4);

    pinMode(START_RECORDING_PIN, INPUT);
    pinMode(STOP_DANCE_PLAYBACK_PIN, INPUT);
    pinMode(START_DANCE_PLAYBACK_PIN, INPUT);
    pinMode(STOP_RECORDING_PIN, INPUT);
    Serial.begin(9600);
}

boolean xblocker = false;
boolean yblocker = false;

void loop()
{
    // Начало записи
    if(!digitalRead(START_RECORDING_PIN))
    {
        while(!digitalRead(START_RECORDING_PIN))
            delay(50);
        rcSwitch.send(START_RECORDING, 8);
    }

    // Конец записи
    if(!digitalRead(STOP_RECORDING_PIN))
    {
        while(!digitalRead(STOP_RECORDING_PIN))
            delay(50);
        rcSwitch.send(STOP_RECORDING, 8);
    }

    // Начало воспроизведения танца
    if(!digitalRead(START_DANCE_PLAYBACK_PIN))
    {
        while(!digitalRead(START_DANCE_PLAYBACK_PIN))
            delay(50);
        rcSwitch.send(START_DANCE_PLAYBACK, 8);
    }

    // Остановка воспроизведения танца
    if(!digitalRead(STOP_DANCE_PLAYBACK_PIN))
    {
        while(!digitalRead(STOP_DANCE_PLAYBACK_PIN))
            delay(50);
        rcSwitch.send(STOP_DANCE_PLAYBACK, 8);
    }

    int x = analogRead(POS_X_PIN);
    int y = analogRead(POS_Y_PIN);

    if(xblocker && (x > X_UP_BORDER || x < X_DOWN_BORDER) && (y < Y_UP_BORDER && y > Y_DOWN_BORDER))
    {
        if(x < X_DOWN_BORDER)
        {
            rcSwitch.send(STICK_LEFT, 8);
        }
        else
        {
            rcSwitch.send(STICK_RIGHT, 8);
        }
        xblocker = false;
    }


    if(yblocker && (y > Y_UP_BORDER || y < Y_DOWN_BORDER) && (x < X_UP_BORDER && x > X_DOWN_BORDER))
    {
        if(y < Y_DOWN_BORDER)
        {
            rcSwitch.send(STICK_DOWN, 8);
        }
        else
        {
            rcSwitch.send(STICK_UP, 8);
        }
        yblocker = false;
    }

    // Засчитывает движение стика единожды
    if(x < X_UP_BORDER && x > X_DOWN_BORDER)
        xblocker = true;
    if(y < Y_UP_BORDER && y > Y_DOWN_BORDER)
        yblocker = true;
    delay(100);
}
