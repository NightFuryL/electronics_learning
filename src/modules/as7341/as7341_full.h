#ifndef AS7341_FULL_H
#define AS7341_FULL_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>

class AS7341Module
{
private:
    Adafruit_AS7341 sensor;
    bool isReady = false;

public:
    struct Spectrum
    {
        uint16_t f1_415nm; // Фиолетовый
        uint16_t f2_445nm; // Индиго
        uint16_t f3_480nm; // Синий
        uint16_t f4_515nm; // Голубой
        uint16_t f5_555nm; // Зеленый
        uint16_t f6_590nm; // Желтый
        uint16_t f7_630nm; // Оранжевый
        uint16_t f8_680nm; // Красный
        uint16_t clear;    // Полный спектр (без фильтра)
        uint16_t nir;      // Ближний ИК
    };

    bool begin()
    {
        // На RP2040 Pico по умолчанию используются пины GP4 (SDA) и GP5 (SCL)
        Wire.begin();

        if (!sensor.begin())
        {
            isReady = false;
            return false;
        }

        isReady = true;

        // Настройка усиления (Gain) и АЦП
        sensor.setGain(AS7341_GAIN_256X);
        sensor.setATIME(100);
        sensor.setASTEP(999);

        return true;
    }

    void setLED(bool state, uint8_t currentmA = 4)
    {
        if (!isReady)
            return;
        sensor.setLEDCurrent(currentmA);
        sensor.enableLED(state);
    }

    bool readSpectrum(Spectrum &spec)
    {
        if (!isReady)
            return false;

        uint16_t readings[12];
        if (!sensor.readAllChannels(readings))
            return false;

        // Индексы АЦП для библиотеки Adafruit AS7341
        spec.f1_415nm = readings[0];
        spec.f2_445nm = readings[1];
        spec.f3_480nm = readings[2];
        spec.f4_515nm = readings[3];
        spec.f5_555nm = readings[6];
        spec.f6_590nm = readings[7];
        spec.f7_630nm = readings[8];
        spec.f8_680nm = readings[9];
        spec.clear = readings[10];
        spec.nir = readings[11];

        return true;
    }
};

#endif