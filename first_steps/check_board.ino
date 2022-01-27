// Проверка платы миганием светодиода

long last_time = millis();

void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  if (millis() - last_time > 5000)
  {
    digitalWrite(13, !digitalRead(13));
    last_time = millis();
  }
}
