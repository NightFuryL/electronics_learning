#include <Arduino.h>
#include <stdarg.h>
#include "modules/as7341/as7341_full.h"

AS7341Module specSensor;
uint32_t lastUpdate = 0;

// true = выводить красивый дашборд в терминал, false = выводить данные для графопостроителя
bool dashboardMode = false;

// Универсальная функция форматированного вывода для Serial
void serialPrintf(const char *fmt, ...)
{
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  Serial.print(buf);
}

void clearConsole()
{
  // \033[2J - очистить весь экран
  // \033[1;1H - переместить курсор строго на 1-ю строку, 1-ю колонку
  Serial.print("\033[2J\033[1;1H");
}
void setup()
{
  Serial.begin(115200);
  delay(2000);

  if (!specSensor.begin())
  {
    Serial.println("Ошибка: Датчик AS7341 не обнаружен!");
    while (1)
      delay(100);
  }
}

void loop()
{
  if (millis() - lastUpdate > 300)
  {
    lastUpdate = millis();

    AS7341Module::Spectrum data;
    if (!specSensor.readSpectrum(data))
      return;

    if (dashboardMode)
    {
      clearConsole();
      Serial.println("===================================================");
      Serial.println("        AS7341 СПЕКТРОМЕТР — ПОЛНЫЙ АНАЛИЗ          ");
      Serial.println("===================================================");
      serialPrintf(" [415nm Фиолетовый] : %d\n", data.f1_415nm);
      serialPrintf(" [445nm Индиго]     : %d\n", data.f2_445nm);
      serialPrintf(" [480nm Синий]      : %d\n", data.f3_480nm);
      serialPrintf(" [515nm Голубой]    : %d\n", data.f4_515nm);
      serialPrintf(" [555nm Зеленый]    : %d\n", data.f5_555nm);
      serialPrintf(" [590nm Желтый]     : %d\n", data.f6_590nm);
      serialPrintf(" [630nm Оранжевый]  : %d\n", data.f7_630nm);
      serialPrintf(" [680nm Красный]    : %d\n", data.f8_680nm);
      Serial.println("---------------------------------------------------");
      serialPrintf(" [Clear Видимый]    : %d\n", data.clear);
      serialPrintf(" [NIR ИК-канал]     : %d\n", data.nir);
      Serial.println("===================================================");
    }
    else
    {
      serialPrintf(">415nm:%d\n", data.f1_415nm);
      serialPrintf(">480nm:%d\n", data.f3_480nm);
      serialPrintf(">555nm:%d\n", data.f5_555nm);
      serialPrintf(">680nm:%d\n", data.f8_680nm);
      serialPrintf(">NIR:%d\n", data.nir);
    }
  }
}