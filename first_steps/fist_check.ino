#define LED_PIN 13
#define D_3_PIN 3
#define D_11_PIN 11

long last_time = millis();

void setup() {
  pinMode(LED_PIN, OUTPUT); // Светодиод
  pinMode(D_3_PIN, OUTPUT); // Для ШИМ
  pinMode(D_11_PIN, OUTPUT); // Для ШИМ
  Serial.begin(9600);
  randomSeed(analogRead(0));
  attachInterrupt(0, function_interrupt, RISING); //Сработает при сигнале с 0 на 1 на пине D2
}

void loop() {
  Serial.println("new_loop!");
  digitalWrite(LED_PIN, HIGH);
  analogWrite(D_3_PIN, 127);
  analogWrite(D_11_PIN, 201);
  delay(2000);

  digitalWrite(LED_PIN, LOW);
  analogWrite(D_3_PIN, 0);
  analogWrite(D_11_PIN, 0);
  delay(2000);

  if (millis() - last_time >= 5000) // Сработывает раз в ~5 секунд
  {
    last_time = millis();
    Serial.println("5 seconds прошло");
  }
}

void function_interrupt()
{
  bool flag = random(999) & 1;
  flag ? Serial.println("0") : Serial.println("1");

  return;
}
